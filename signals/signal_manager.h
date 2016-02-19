#pragma once
#include "Defs.h"
#include <mutex>
#include <boost/thread.hpp>
#include "signal_base.h"
#include "LokiTypeInfo.h"
#include <memory>
#include "signal.h"
#include "combiner.h"
#include "boost/type_traits/function_traits.hpp"
#include <boost/preprocessor.hpp>

/*
#define CONNECT_3(type,name,function)connect<type>(name, function, boost::this_thread::get_id(), "", __LINE__, __FILE__)
#define CONNECT_4(type,name,function,thread)connect<type>(name, function, thread, "", __LINE__, __FILE__)
#define CONNECT_5(type,name,function,thread,description)connect<type>(name, function, thread, description, __LINE__, __FILE__)

//#define CONNECT(type, name, function, thread, description)connect<type>(name, function, thread, description, __LINE__, __FILE__)

#ifdef _MSC_VER
#define CONNECT(...) BOOST_PP_CAT( BOOST_PP_OVERLOAD(CONNECT_, __VA_ARGS__ )(__VA_ARGS__), BOOST_PP_EMPTY() )
#else
#define CONNECT(...) BOOST_PP_CAT( BOOST_PP_OVERLOAD(CONNECT_, __VA_ARGS__ )(name, __VA_ARGS__), BOOST_PP_EMPTY() )
#endif
*/
namespace Signals
{
	template<class T, template<class> class C> class typed_signal_base;
	template<class T, template<class> class C, template<class...> class S> class typed_signal;

    class SIGNAL_EXPORTS signal_manager
    {
    public:
        signal_manager();
        static signal_manager* get_instance();
        static void set_instance(signal_manager* inst);

		template<typename T, template<class> class combiner = default_combiner, template<class...> class Sink = signal_sink> typed_signal_base<T, combiner>* get_signal(const std::string& name, const std::string& description = "", std::string file_name = "", int line_number = -1)
        {
            std::lock_guard<std::mutex> lock(mtx);
			if (line_number != -1 && file_name.size())
				register_sender(Loki::TypeInfo(typeid(T)), name, description, file_name, line_number);
            auto&sig = get_signal(name, Loki::TypeInfo(typeid(typed_signal_base<T,combiner>)));
            if (!sig)
                sig.reset(new typed_signal<T, combiner, Sink>(description));
			
            return std::dynamic_pointer_cast<typed_signal_base<T, combiner>>(sig).get();
        }
		template<typename T, template<class> class combiner = default_combiner, template<class...> class Sink = signal_sink, class C> typed_signal_base<T, combiner>* get_signal(const std::string& name, C* This, const std::string& description = "")
		{
			std::lock_guard<std::mutex> lock(mtx);
			register_sender(Loki::TypeInfo(typeid(T)), name, Loki::TypeInfo(typeid(C)), This, description);
			auto&sig = get_signal(name, Loki::TypeInfo(typeid(typed_signal_base<T, combiner>)));
			if (!sig)
			{
				static sink_constructor<T, Sink<T>> sink_constructor;
				sig.reset(new typed_signal_base<T, combiner>(description));
			}
				
			auto typed_sig = dynamic_cast<typed_signal_base<T, combiner>*>(sig.get());
			return typed_sig;
		}
		template<typename T, template<class> class combiner = default_combiner> std::shared_ptr<Signals::connection> connect(const std::string& name, std::function<T> f, boost::thread::id destination_thread = boost::this_thread::get_id(), const std::string& receiver_description = "", int line_number = -1, const std::string& filename = "")
		{
			auto sig = get_signal<T, combiner>(name);
			if (filename.size() && line_number != -1)
				register_receiver(Loki::TypeInfo(typeid(T)), name, line_number, filename, receiver_description);
			return sig->connect(f, destination_thread);
		}
		template<typename T, typename C> std::shared_ptr<Signals::connection> connect(const std::string& name, std::function<T> f, C* receiver, boost::thread::id destination_thread = boost::this_thread::get_id(), const std::string& receiver_description = "")
		{
			auto sig = get_signal<T>(name);
			register_receiver(Loki::TypeInfo(typeid(T)), name, Loki::TypeInfo(typeid(C)), receiver, receiver_description);
			return sig->connect(f, destination_thread);
		}
		void register_sender(Loki::TypeInfo signal_signature, std::string signal_name, Loki::TypeInfo sender_type, void* sender_ptr, std::string desc);
		void register_sender(Loki::TypeInfo signal_signature, std::string signal_name, std::string desc, std::string file_name, int line_number);
		void register_receiver(Loki::TypeInfo signal_signature, std::string signal_name, Loki::TypeInfo receiver_type, void* receiver_ptr, std::string desc);
		void register_receiver(Loki::TypeInfo signal_signature, std::string signal_name, int line_number, std::string file_name, std::string desc);

		std::vector<signal_base*> get_signals(std::string name);
		std::vector<signal_base*> get_signals(Loki::TypeInfo type);
		std::vector<signal_base*> get_signals(Loki::TypeInfo type, std::string name);
		std::vector<signal_base*> get_signals();
		
		void print_signal_map();
    protected:
        
        virtual std::shared_ptr<signal_base>& get_signal(const std::string& name, Loki::TypeInfo type);

		// Signal signature, signal name, pointer to signal
        std::map<Loki::TypeInfo, std::map<std::string, std::shared_ptr<signal_base>>> _signals;

		// Mapping of which objects emit signals
		std::map<Loki::TypeInfo, std::map<std::string, std::vector<std::tuple<Loki::TypeInfo, void*, std::string>>>> _registered_sender_objects;

		// Mapping of what signals are emitted from which files / lines
		std::map<Loki::TypeInfo, std::map<std::string, std::vector<std::tuple<int, std::string, std::string>>>> _registered_senders;

		// Mapping of who receives signals
		// stores type, pointer and description
		std::map<Loki::TypeInfo, std::map<std::string, std::vector<std::tuple<Loki::TypeInfo, void*, std::string>>>> _registered_receiver_objects;
		// Stores line number, file name and description of receiver
		std::map<Loki::TypeInfo, std::map<std::string, std::vector<std::tuple<int, std::string, std::string>>>> _registered_receivers;

        std::mutex mtx;
    };

    /*class SIGNAL_EXPORTS signal_registery
    {
    public:
        static void register_sender(const std::string& signal_name, Loki::TypeInfo type, void* sender, Loki::TypeInfo signal_signature, signal_manager* mgr = signal_manager::get_instance(), const std::string& description = "");
		static void register_receiver(const std::string& signal_name, Loki::TypeInfo type, void* receiver, Loki::TypeInfo signal_signature, signal_manager* mgr = signal_manager::get_instance(), const std::string& description = "");
    };


    template<typename T> class SIGNAL_EXPORTS signal_receiver_registerer
    {
    public:
        signal_receiver_registerer(T* This, const std::string& signal_name, Loki::TypeInfo signal_signature, const std::string description)
        {
            signal_registery::register_sender(signal_name, Loki::TypeInfo(typeid(T)), (void*)This, signal_signature, description);
        }
    };
    template<typename T> signal_receiver_registerer<T> register_receiver(T* This, const std::string& signal_name, Loki::TypeInfo signal_signature, const std::string description)
    {
        return signal_receiver_registerer<T>(This, signal_name, signal_signature, description);
    }
    template<typename T> class SIGNAL_EXPORTS signal_sender_registerer
    {
    public:
        signal_sender_registerer(T* This, const std::string& signal_name, Loki::TypeInfo signal_signature)
        {
            signal_registery::register_receiver(signal_name, Loki::TypeInfo(typeid(T)), (void*)This, signal_signature);
        }
    };*/
    template<typename T> void register_sender(T* This, const std::string& signal_name, Loki::TypeInfo signal_signature, signal_manager* mgr = signal_manager::get_instance(), const std::string& description = "")
    {
		mgr->register_sender(signal_signature, signal_name, Loki::TypeInfo(typeid(T)), This, description);
    }
} // namespace Signals
