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
    struct receiver
    {
        void* ptr; // Pointer to object   
        Loki::TypeInfo type; // Object type
        Loki::TypeInfo signature; // Signal signature
        std::string signal_name;// Signal name
        std::string description; // sender description
        std::string file; // File from which the sender is sending the signal, mostly used when not associated to an object
        int line; // Line from which the signal is sent
        std::string tooltip; // description of what happens when this signal is received
    };

    struct sender
    {
        void* ptr; // Pointer to object
        Loki::TypeInfo type; // Object type
        Loki::TypeInfo signature; // Signal signature
        std::string signal_name; // Signal name
        std::string description; // sender description
        std::string tooltip; // Description of why this signal is sent
        std::string file; // File from which the sender is sending the signal, mostly used when not associated to an object
        int line; // Line from which the signal is sent
    };
    // Lets you do static introspection of any signaler object even those not constructed
    class SIGNAL_EXPORTS signal_registry
    {
        std::map<Loki::TypeInfo, std::map<std::string, std::vector<Loki::TypeInfo>>> _static_signal_registry;
        std::map<Loki::TypeInfo, std::map<std::string, std::vector<Loki::TypeInfo>>> _static_slot_registry;
        std::map<Loki::TypeInfo, std::function<void(void)>> _static_python_registration_functions;
        signal_registry();
    public:
        static signal_registry* instance();
        void add_signal(Loki::TypeInfo sender, std::string name, Loki::TypeInfo signature);
        void add_slot(Loki::TypeInfo receiver, std::string name, Loki::TypeInfo signature);
        void add_python_registration_function(Loki::TypeInfo sender, std::function<void(void)> f);
        std::string print_signal_map();        
    };
    template<class Sig> class typed_signal_base;
	class signaler;

    // Manages shared ownership of signals so that multiple senders and receivers can exist, also allows for encapsulation of groups of signals based on subsystem
    class SIGNAL_EXPORTS signal_manager
    {
    public:
        signal_manager();
        static signal_manager* get_instance();
        static void set_instance(signal_manager* inst);
		
        template<typename T, template<class...> class Sink = signal_sink> typed_signal_base<T>* get_signal_optional(const std::string& name)
		{
			std::lock_guard<std::mutex> lock(mtx);
			auto signature = Loki::TypeInfo(typeid(typed_signal_base<T>));
			auto itr = _signals.find(signature);
			if(itr == _signals.end())
				return nullptr;
			auto itr2 = itr->second.find(name);
			if(itr2 == itr->second.end())
				return nullptr;

			auto&sig = itr2->second;
			if (!sig)
			{
				LOG(debug) << this << " Creating signal " << name << " <" << signature.name() << ">";
				static sink_constructor<T, Sink<T>> sink_constructor;
				sig.reset(new typed_signal_base<T>());
			}
			return std::dynamic_pointer_cast<typed_signal_base<T>>(sig).get();
		}

		template<typename T, template<class...> class Sink = signal_sink> typed_signal_base<T>* get_signal(const std::string& name)
        {
            std::lock_guard<std::mutex> lock(mtx);
			auto signature = Loki::TypeInfo(typeid(typed_signal_base<T>));
            auto&sig = get_signal(name, signature);
			if (!sig)
			{
				LOG(debug) << this << " Creating signal " << name << " <" << signature.name() << ">";
				static sink_constructor<T, Sink<T>> sink_constructor;
				sig.reset(new typed_signal_base<T>());
			}
            return std::dynamic_pointer_cast<typed_signal_base<T>>(sig).get();
        }

		template<typename T, template<class...> class Sink = signal_sink, class C> typed_signal_base<T>* get_signal_optional(const std::string& name)
		{
			std::lock_guard<std::mutex> lock(mtx);
			auto signature = Loki::TypeInfo(typeid(typed_signal_base<T>));
			auto itr = _signals.find(signature);
			if(itr == _signals.end())
				return nullptr;
			auto itr2 = itr->second.find(name);
			if(itr2 == itr->second.end())
				return nullptr;
			auto&sig = itr2->second;
			if (!sig)
			{
				static sink_constructor<T, Sink<T>> sink_constructor;
				LOG(debug) << this << " Creating signal " << name << " <" << signature.name() << ">";
				sig.reset(new typed_signal_base<T>());
			}
			auto typed_sig = dynamic_cast<typed_signal_base<T>*>(sig.get());
			return typed_sig;
		}

		template<typename T, template<class...> class Sink = signal_sink, class C> typed_signal_base<T>* get_signal(const std::string& name)
		{
			std::lock_guard<std::mutex> lock(mtx);
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
		
        template<typename T> std::shared_ptr<Signals::connection> connect(const std::string& name, std::function<T> f, void* ptr = nullptr, size_t destination_thread = get_this_thread())
		{
			auto sig = get_signal<T>(name);
			return sig->connect(f, destination_thread, false, ptr);
		}
		template<typename T, typename C> std::shared_ptr<Signals::connection> connect(const std::string& name, std::function<T> f, void* ptr = nullptr, size_t destination_thread = get_this_thread())
		{
			auto sig = get_signal<T>(name);
			return sig->connect(f, destination_thread, false, ptr);
		}
        

		
        virtual void register_sender(Loki::TypeInfo signal_signature, std::string signal_name, Loki::TypeInfo sender_type, void* sender_ptr, std::string object_description, std::string tooltip = "");
		virtual void register_sender(Loki::TypeInfo signal_signature, std::string signal_name, std::string desc, std::string file_name, int line_number, std::string tooltip = "");
        virtual void register_sender(Loki::TypeInfo signal_signature, std::string signal_name, signaler* sender_ptr);
        template<typename T> void register_sender(std::string name, signaler* sender)
        {
            register_sender(Loki::TypeInfo(typeid(T)), name, sender);
        }

		virtual void remove_sender(void* sender_ptr, std::string signal_name = "");
		virtual void remove_sender(std::string file_name, int line_number, std::string signal_name = "");

		virtual void register_receiver(Loki::TypeInfo signal_signature, const std::string& signal_name, Loki::TypeInfo receiver_type, void* receiver_ptr, const std::string& receiver_description = "", const std::string& tooltip = "");
		virtual void register_receiver(Loki::TypeInfo signal_signature, const std::string& signal_name, int line_number, const std::string& file_name, const std::string& receiver_description = "", const std::string& tooltip = "");
        virtual void register_receiver(Loki::TypeInfo signal_signature, const std::string& signal_name, signaler* receiver);
        template<typename T> void register_receiver(const std::string& name, signaler* receiver)
        {
            register_receiver(Loki::TypeInfo(typeid(T)), name, receiver);
        }

		virtual std::vector<signal_base*> get_signals(std::string name);
		virtual std::vector<signal_base*> get_signals(Loki::TypeInfo type);
		virtual std::vector<signal_base*> get_signals(Loki::TypeInfo type, std::string name);
		virtual std::vector<signal_base*> get_signals();
		virtual void print_signal_map();
        
        
        // Helps with dynamic introspection on what actually is connected
        virtual std::vector<receiver> get_receivers(Loki::TypeInfo type, std::string name);
        virtual std::vector<receiver> get_receivers(Loki::TypeInfo type);
        virtual std::vector<receiver> get_receivers(std::string name);
        virtual std::vector<receiver> get_receivers();

        virtual std::vector<sender> get_senders(Loki::TypeInfo type, std::string name);
        virtual std::vector<sender> get_senders(Loki::TypeInfo type);
        virtual std::vector<sender> get_senders(std::string name);
        virtual std::vector<sender> get_senders();
    protected:
        
        virtual std::shared_ptr<signal_base>& get_signal(const std::string& name, Loki::TypeInfo type);

		// Signal signature, signal name, pointer to signal
        std::map<Loki::TypeInfo, std::map<std::string, std::shared_ptr<signal_base>>> _signals;

		// Mapping of which objects emit signals
		std::map<Loki::TypeInfo, std::map<std::string, std::vector<sender>>> _registered_senders;
		std::map<Loki::TypeInfo, std::map<std::string, std::vector<receiver>>> _registered_receivers;

        std::mutex mtx;
    };
    class signaler;
    SIGNAL_EXPORTS void register_sender(signaler* sender, const std::string& signal_name, Loki::TypeInfo signal_signature, signal_manager* mgr);
    SIGNAL_EXPORTS void register_receiver(signaler* receiver, const std::string& signal_name, Loki::TypeInfo signal_signature, signal_manager* mgr);
    template<typename T> void register_sender(T* This, const std::string& signal_name, Loki::TypeInfo signal_signature, signal_manager* mgr = signal_manager::get_instance(), const std::string& description = "")
    {
		mgr->register_sender(signal_signature, signal_name, Loki::TypeInfo(typeid(T)), This, description);
    }
} // namespace Signals
