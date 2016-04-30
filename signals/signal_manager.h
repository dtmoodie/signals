#pragma once
#include "Defs.h"
#include "signal.h"
#include "combiner.h"
#include "signal_base.h"
#include "logging.hpp"
#include "thread_registry.h"

#include <mutex>
#include <memory>

#include "LokiTypeInfo.h"


//#include <boost/thread.hpp>
//#include "boost/type_traits/function_traits.hpp"
//#include <boost/preprocessor.hpp>

namespace Signals
{
	template<class T> class typed_signal_base;
	class signaler;
    class SIGNAL_EXPORTS signal_manager
    {
    public:
        signal_manager();
        static signal_manager* get_instance();
        static void set_instance(signal_manager* inst);

		template<typename T, template<class...> class Sink = signal_sink> typed_signal_base<T>* get_signal(const std::string& name, const std::string& description = "", std::string file_name = "", int line_number = -1)
        {
            std::lock_guard<std::mutex> lock(mtx);
			if (line_number != -1 && file_name.size())
				register_sender(Loki::TypeInfo(typeid(T)), name, description, file_name, line_number);
			auto signature = Loki::TypeInfo(typeid(typed_signal_base<T>));
            auto&sig = get_signal(name, signature);
			if (!sig)
			{
				LOG(debug) << this << " Creating signal " << name << " <" << signature.name() << ">";
				static sink_constructor<T, Sink<T>> sink_constructor;
				sig.reset(new typed_signal_base<T>(description));
			}
            return std::dynamic_pointer_cast<typed_signal_base<T>>(sig).get();
        }
		template<typename T, template<class...> class Sink = signal_sink, class C> typed_signal_base<T>* get_signal(const std::string& name, C* This, const std::string& description = "")
		{
			std::lock_guard<std::mutex> lock(mtx);
			register_sender(Loki::TypeInfo(typeid(T)), name, Loki::TypeInfo(typeid(C)), This, description);
			auto signature = Loki::TypeInfo(typeid(typed_signal_base<T>));
			auto&sig = get_signal(name, signature);
			if (!sig)
			{
				static sink_constructor<T, Sink<T>> sink_constructor;
				LOG(debug) << this << " Creating signal " << name << " <" << signature.name() << ">";
				sig.reset(new typed_signal_base<T>(description));
			}
				
			auto typed_sig = dynamic_cast<typed_signal_base<T>*>(sig.get());
			return typed_sig;
		}
		template<typename T> std::shared_ptr<Signals::connection> connect(const std::string& name, std::function<T> f, size_t destination_thread = get_this_thread(), const std::string& receiver_description = "", int line_number = -1, const std::string& filename = "")
		{
			auto sig = get_signal<T>(name);
			if (filename.size() && line_number != -1)
				register_receiver(Loki::TypeInfo(typeid(T)), name, line_number, filename, receiver_description);
			return sig->connect(f, destination_thread);
		}
		template<typename T, typename C> std::shared_ptr<Signals::connection> connect(const std::string& name, std::function<T> f, C* receiver, size_t destination_thread = get_this_thread(), const std::string& receiver_description = "")
		{
			auto sig = get_signal<T>(name);
			register_receiver(Loki::TypeInfo(typeid(T)), name, Loki::TypeInfo(typeid(C)), receiver, receiver_description);
			return sig->connect(f, destination_thread);
		}
		void register_sender(Loki::TypeInfo signal_signature, std::string signal_name, Loki::TypeInfo sender_type, void* sender_ptr, std::string desc);
		void register_sender(Loki::TypeInfo signal_signature, std::string signal_name, Loki::TypeInfo sender_type, signaler* sender_ptr);
		void register_sender(Loki::TypeInfo signal_signature, std::string signal_name, std::string desc, std::string file_name, int line_number);

		void remove_sender(void* sender_ptr);
		void remove_sender(std::string file_name, int line_number);

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
    template<typename T> void register_sender(T* This, const std::string& signal_name, Loki::TypeInfo signal_signature, signal_manager* mgr = signal_manager::get_instance(), const std::string& description = "")
    {
		mgr->register_sender(signal_signature, signal_name, Loki::TypeInfo(typeid(T)), This, description);
    }
} // namespace Signals
