#include <iostream>
#include <functional>
#include <list>
#include <future>
#include <vector>
#include <memory>
#include <map>
#include <mutex>
#include "CudaUtils.hpp"
#include <boost/function.hpp>
#include <boost/thread/thread.hpp>
#include <type_traits>
#include <cstddef>
#include "LokiTypeInfo.h"
#include <boost/preprocessor.hpp>

template<class T> class Signal { };
template<template<class, class...> class S, class T> class SignalWithSink {};
template<class T> class Channel {};
template<class T> class QueuedChannel: public Channel<T> {};
class connection;

// *************************************************************************
// http://stackoverflow.com/questions/21192659/variadic-templates-and-stdbind
template<int...> struct int_sequence {};
template<int N, int... Is> struct make_int_sequence : make_int_sequence<N - 1, N - 1, Is...> {};
template<int... Is> struct make_int_sequence<0, Is...> : int_sequence<Is...> {};
template<int> struct placeholder_template{};
namespace std {
    template<int N> struct is_placeholder< placeholder_template<N> >: integral_constant<int, N + 1> { };
}
template<class R, class... Args, int... Is>
std::function<R(Args...)> my_bind(R(*p)(Args...), int_sequence<Is...>)
{    return std::bind(p, placeholder_template<Is>{}...);}

template<class R, class C, class... Args, int... Is>
std::function<R(Args...)> my_bind(R(C::*p)(Args...), C* ptr, int_sequence<Is...>)
{    return std::bind(p, ptr, placeholder_template<Is>{}...);}
// *************************************************************************



class thread_specific_queue
{
protected:
    static std::map<boost::thread::id, EagleLib::concurrent_queue<std::function<void(void)>>> thread_queues;
public:
    static void push(const std::function<void(void)>& f, boost::thread::id id = boost::this_thread::get_id())
    {
        thread_queues[id].push(f);
    }
    static void run()
    {
       std::function<void(void)> f;
       auto& queue = thread_queues[boost::this_thread::get_id()];
       while(queue.try_pop(f))
       {
            f();
       }
    }
};
std::map<boost::thread::id, EagleLib::concurrent_queue<std::function<void(void)>>> thread_specific_queue::thread_queues;
class ISignalLogSink
{
    std::shared_ptr<connection> _connection;
public:
    virtual void set_connection(std::shared_ptr<connection> connection) { _connection = connection; }
};
class ISignal
{
protected:
    friend class connection;
    virtual void remove_receiver(int index) = 0;
    virtual void add_log_sink(std::shared_ptr<ISignalLogSink> sink) = 0;
    virtual Loki::TypeInfo get_signal_type() = 0;
};

class connection
{
    int _index;
    ISignal* _parent;
public:
    connection(int index, ISignal* parent): _index(index), _parent(parent)   {           }
    ~connection()
    {
        if(_parent)
        {
            _parent->remove_receiver(_index);
        }
        
    }
};

template<class R, class...T> struct Channel<R(T...)>
{
    virtual std::future<R> exec(const std::function<R(T...)>& f, T... args)
    {
        return std::future<R>(f(args...));
    }
};
template<class...T> struct Channel<void(T...)>
{
    virtual std::future<void> exec(const std::function<void(T...)>& f, T... args)
    {
        f(args...);
        return std::future<void>();
    }
};

template<class R, class...T> struct executor
{
    std::promise<R> promise;
    void exec_on_thread(const std::function<R(T...)> f, T... args)
    {
        promise.set_value(f(args...));
    }
};
template<class...T> struct executor<void, T...>
{
    std::promise<void> promise;
    void exec_on_thread(const std::function<void(T...)> f, T... args)
    {
        f(args...);
        promise.set_value();
    }
};

template<class R, class...T> struct QueuedChannel<R(T...)>: public Channel<R(T...)>
{
    boost::thread::id destination_thread;
    QueuedChannel(boost::thread::id dest): destination_thread(dest){}
    

    virtual std::future<R> exec(const std::function<R(T...)>& f, T... args)
    {
        auto exec = new executor<R,T...>();
        thread_specific_queue::push(std::bind(&executor<R, T...>::exec_on_thread,exec, f, args...), destination_thread);
        
        return exec->promise.get_future();
    }
};


template<class... T> class Serializer
{
public:
    static void serialize(T... args)
    {
    }
};
template<class T1, class ... Ts> class Serializer<T1, Ts...>: public Serializer<Ts...>
{
public:
    static void serialize(T1 arg1, Ts... args)
    {
        std::cout << arg1 << " ";
        Serializer<Ts...>::serialize(args...);
    }
    static void serialize_ss(std::stringstream& ss, T1 arg1, Ts... args)
    {
        ss << arg1 << ", ";
        Serializer<Ts...>::serialize_ss(ss, args...);
    }
    static void deserialize(std::stringstream& ss, T1& arg1, Ts&... args)
    {
        ss >> arg1;
        Serializer<Ts...>::deserialize(ss, args...);
    }
};


template<class R, class...T> class SignalLogSink: public ISignalLogSink
{
public:
    void handle_log(T... args)
    {
    
    }
};
class SignalLogSinkFactory
{
    std::map<Loki::TypeInfo, std::function<ISignalLogSink*(void)> > registry;
public:
    static SignalLogSinkFactory* instance();

    void register_creator(std::function<ISignalLogSink*(void)> f, Loki::TypeInfo type)
    {
        registry[type] = f;
    }
    std::shared_ptr<ISignalLogSink> create_log_sink(Loki::TypeInfo type)
    {
        auto itr = registry.find(type);
        if(itr != registry.end())
        {
            return std::shared_ptr<ISignalLogSink>(itr->second());
        }
        return std::shared_ptr<ISignalLogSink>();
    }
};
SignalLogSinkFactory* SignalLogSinkFactory::instance()
{
    static SignalLogSinkFactory* inst = nullptr;
    if(inst == nullptr)
    {
        inst = new SignalLogSinkFactory();
    }
    return inst;
}

template<template<class, class...> class SINK, class R, class...T> struct SinkCreator
{
    static ISignalLogSink* create()
    {
        return new SINK<R,T...>();
    }
};
template<class R, class...T> class Signal<R(T...)>: public ISignal
{
protected:
    std::map<int, std::function<R(T...)> > receivers;
    std::map<int, std::shared_ptr<Channel<R(T...)>>> channels;
    std::list<int> unused_indexes;
    std::mutex mtx;
    std::shared_ptr<connection> serializer;

    friend class connection;
    virtual void remove_receiver(int index)
    {
        std::lock_guard<std::mutex> lock(mtx);
        auto itr = receivers.find(index);
        if(itr != receivers.end())
        {
            receivers.erase(index);
            auto itr2 = channels.find(index);
            channels.erase(itr2);
            unused_indexes.push_back(index);
        }
    }
public:
    Signal()
    {
        for(int i = 0; i < 256; ++i)
            unused_indexes.push_back(i);
        //SignalLogSinkFactory::instance()->register_creator(&SinkCreator<SignalLogSink, R,T...>::create, get_signal_type());
    }
    virtual void add_log_sink(std::shared_ptr<ISignalLogSink> sink)
    {
        auto typed_sink = std::dynamic_pointer_cast<SignalLogSink<R,T...>>(sink);
        typed_sink->set_connection(connect(my_bind(&SignalLogSink<R,T...>::handle_log, typed_sink.get(), make_int_sequence<sizeof...(T) >{})));
    }

    std::shared_ptr<connection> connect(const std::function<R(T...)>& f, boost::thread::id destination_thread = boost::this_thread::get_id())
    {
        std::lock_guard<std::mutex> lock(mtx);
        int index = unused_indexes.back();
        unused_indexes.pop_back();
        receivers[index] = f;
        if(destination_thread != boost::this_thread::get_id())
            channels[index] = std::shared_ptr<Channel<R(T...)>>(new QueuedChannel<R(T...)>(destination_thread));
        else
            channels[index] = std::shared_ptr<Channel<R(T...)>>(new Channel<R(T...)>());

        return std::shared_ptr<connection>(new connection(index, this));
    }


    R operator()(T... args)
    {
        for(auto& itr: receivers)
        {
            channels[itr.first]->exec(itr.second, args...);
        }
    }
    virtual Loki::TypeInfo get_signal_type()
    {
        return Loki::TypeInfo(typeid(R(T...)));
    }
};

template<template<class,class...> class SINK, class R, class...T> class SignalWithSink<SINK, R(T...)>: public Signal<R(T...)>
{
public:
    SignalWithSink(): Signal<R(T...)>()
    {
        SignalLogSinkFactory::instance()->register_creator(&SinkCreator<SINK, R,T...>::create, get_signal_type());
    }
};

struct test_class
{
    void func(int x, int y)
    {
        std::cout << x << ", " << y;
    }
};

void function(int x, int y)
{
    std::cout << x << ": " << y;
}

class signal_registery
{
public:
    static void register_sender(const std::string& signal_name, Loki::TypeInfo type, void*)
    {
        
    }
};
template<typename T> class signal_registerer
{
public:
    signal_registerer(T* This, const std::string& signal_name)
    {
        signal_registery::register_sender(signal_name, Loki::TypeInfo(typeid(T)), (void*)This);
    }
};
template<typename T> signal_registerer<T> register_signal(T* This, const std::string& signal_name)
{
    return signal_registerer<T>(This, signal_name);
}

#define SIG_DEF_1(name, ARG1) \
inline void on_##name(ARG1 arg1) {static auto registerer = register_signal(this, #name); emit(#name, arg1); }

#define SIG_DEF_2(name, ARG1, ARG2) \
inline void on_##name(ARG1 arg1, ARG2 arg2) {static auto registerer = register_signal(this, #name); emit(#name, arg1, arg2); }

#define SIG_DEF_3(name, ARG1, ARG2, ARG3) \
inline void on_##name(ARG1 arg1, ARG2 arg2, ARG3 arg3) {static auto registerer = register_signal(this, #name); emit(#name, arg1, arg2, arg3); }

#define SIG_DEF_4(name, ARG1, ARG2, ARG3, ARG4) \
inline void on_##name(ARG1 arg1, ARG2 arg2, ARG3 arg3, ARG4 arg4) {static auto registerer = register_signal(this, #name); emit(#name, arg1, arg2, arg3, arg4); }

#define SIG_DEF_5(name, ARG1, ARG2, ARG3, ARG4, ARG5) \
inline void on_##name(ARG1 arg1, ARG2 arg2, ARG3 arg3, ARG4 arg4, ARG5 arg5) {static auto registerer = register_signal(this, #name); emit(#name, arg1, arg2, arg3, arg4, arg5); }

#define SIG_DEF_6(name, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6) \
inline void on_##name(ARG1 arg1, ARG2 arg2, ARG3 arg3, ARG4 arg4, ARG5 arg5, ARG6 arg6) {static auto registerer = register_signal(this, #name);emit(#name, arg1, arg2, arg3, arg4, arg5, arg6); }

#define SIG_DEF_7(name, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7) \
inline void on_##name(ARG1 arg1, ARG2 arg2, ARG3 arg3, ARG4 arg4, ARG5 arg5, ARG6 arg6, ARG7 arg7) {static auto registerer = register_signal(this, #name);emit(#name, arg1, arg2, arg3, arg4, arg5, arg6, arg7); }

#define SIG_DEF_8(name, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7, ARG8) \
inline void on_##name(ARG1 arg1, ARG2 arg2, ARG3 arg3, ARG4 arg4, ARG5 arg5, ARG6 arg6, ARG7 arg7, ARG8 arg8) {static auto registerer = register_signal(this, #name);emit(#name, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8); }

#define SIG_DEF_9(name, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7, ARG8, ARG9) \
inline void on_##name(ARG1 arg1, ARG2 arg2, ARG3 arg3, ARG4 arg4, ARG5 arg5, ARG6 arg6, ARG7 arg7, ARG8 arg8, ARG9 arg9) {static auto registerer = register_signal(this, #name);emit(#name, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9); }

#define SIG_DEF_10(name, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7, ARG8, ARG9, ARG10) \
inline void on_##name(ARG1 arg1, ARG2 arg2, ARG3 arg3, ARG4 arg4, ARG5 arg5, ARG6 arg6, ARG7 arg7, ARG8 arg8, ARG9 arg9, ARG10 arg10) {static auto registerer = register_signal(this, #name);emit(#name, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10); }

// name / signature
#ifdef _MSC_VER
#define SIG_DEF(name, ...) BOOST_PP_CAT( BOOST_PP_OVERLOAD(SIG_DEF_, __VA_ARGS__ )(name, __VA_ARGS__), BOOST_PP_EMPTY() )
#else

#endif

class Signaler
{
protected:
    template<class... T> void emit(const std::string& signal_name, T&... args)
    {
        // Find the signal and send
    }
public:

};

class TestSignalerImpl: public Signaler
{
    typedef TestSignalerImpl this_class;
public:
    SIG_DEF(test1, int);
    SIG_DEF(test2, int, int);
    SIG_DEF(test3, int, int, double);
    SIG_DEF(test4, int, int, double, int);
    SIG_DEF(test5, int, int, double, double, double);
    SIG_DEF(test6, int, int, int, int, int, int);
    SIG_DEF(test7, int, int, int, int, int, int, int);
    SIG_DEF(test8, int, int, int, int, int, int, int, int);
    SIG_DEF(test9, int, int, int, int, int, int, int, int, int);
    SIG_DEF(test10, int, int, int, int, int, int, int, int, int, int);
};


int main()
{
    test_class clas;
    boost::thread work_thread(std::bind([]()->void{while(!boost::this_thread::interruption_requested()){thread_specific_queue::run();}}));
    TestSignalerImpl test2;
    test2.on_test1(5);
    test2.on_test2(5,6);
    test2.on_test3(5,6,7);
    SignalWithSink<SignalLogSink,void(int,int)> test;
    auto sink = SignalLogSinkFactory::instance()->create_log_sink(test.get_signal_type());
    test.add_log_sink(sink);
    {
        auto connection = test.connect(std::bind(&test_class::func, &clas, std::placeholders::_1, std::placeholders::_2), work_thread.get_id());
        test(5, 6);
    }
    test(5, 6);
    
    work_thread.interrupt();
    work_thread.join();
    return 0;    
}