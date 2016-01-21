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


template<class T> class Signal { };
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
class ISignal
{
protected:
    friend class connection;
    virtual void remove_receiver(int index) = 0;
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

template<class R, class...T> class ISignalLogSink
{
public:
    void log(T... args)
    {
        std::stringstream ss;
        Serializer<T...>::serialize(ss, args...);
        std::cout << ss.str();
    }
};


template<class R, class...T> class Signal<R(T...)>: public ISignal
{
    std::map<int, std::function<R(T...)> > receivers;
    std::map<int, std::shared_ptr<Channel<R(T...)>>> channels;
    std::list<int> unused_indexes;
    std::mutex mtx;
    std::shared_ptr<connection> serializer;
protected:
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
        serializer = connect(my_bind(&Serializer<T...>::serialize, make_int_sequence< sizeof...(T) >{}));
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

int main()
{
    test_class clas;
    boost::thread work_thread(std::bind([]()->void{while(!boost::this_thread::interruption_requested()){thread_specific_queue::run();}}));
    
    Signal<void(int,int)> test;
    //ISignalLogSink<void(int,int)> log_sink;
    //auto logging_connection = test.connect(my_bind(&ISignalLogSink<void(int,int)>::log, make_int_sequence< 1,2 >{}));
    {
        auto connection = test.connect(std::bind(&test_class::func, &clas, std::placeholders::_1, std::placeholders::_2), work_thread.get_id());
        test(5, 6);
    }
    test(5, 6);
    
    work_thread.interrupt();
    work_thread.join();
    return 0;    
}