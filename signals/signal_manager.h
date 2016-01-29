#pragma once
#include "Defs.h"
#include <mutex>
#include <boost/thread.hpp>
#include "signal_base.h"
#include "LokiTypeInfo.h"
#include <memory>
#include "signal.h"
namespace Signals
{
    template<class T> class signal;
    class SIGNAL_EXPORTS signal_manager
    {
    public:
        signal_manager();
        static signal_manager* get_instance();
        static void set_instance(signal_manager* inst);

        virtual void register_thread(int type, boost::thread::id id = boost::this_thread::get_id());

        virtual boost::thread::id get_thread(int type);

        template<typename T> signal<T>* get_signal(const std::string& name)
        {
            std::lock_guard<std::mutex> lock(mtx);
            auto&sig = get_signal(name, Loki::TypeInfo(typeid(T)));
            if (!sig)
                sig.reset(new signal<T>());
            return std::dynamic_pointer_cast<signal<T>>(sig).get();
        }
    protected:
        
        virtual std::shared_ptr<signal_base>& get_signal(const std::string& name, Loki::TypeInfo type);
        std::map<Loki::TypeInfo, std::map<std::string, std::shared_ptr<signal_base>>> _signals;
        std::map<int, std::vector<boost::thread::id>> _thread_map;

        std::mutex mtx;
    };

    class SIGNAL_EXPORTS signal_registery
    {
    public:
        static void register_sender(const std::string& signal_name, Loki::TypeInfo type, void* sender, Loki::TypeInfo signal_signature);
        static void register_receiver(const std::string& signal_name, Loki::TypeInfo type, void* receiver, Loki::TypeInfo signal_signature);
    };


    template<typename T> class SIGNAL_EXPORTS signal_receiver_registerer
    {
    public:
        signal_receiver_registerer(T* This, const std::string& signal_name, Loki::TypeInfo signal_signature)
        {
            signal_registery::register_sender(signal_name, Loki::TypeInfo(typeid(T)), (void*)This, signal_signature);
        }
    };
    template<typename T> signal_receiver_registerer<T> register_receiver(T* This, const std::string& signal_name, Loki::TypeInfo signal_signature)
    {
        return signal_receiver_registerer<T>(This, signal_name, signal_signature);
    }
    template<typename T> class SIGNAL_EXPORTS signal_sender_registerer
    {
    public:
        signal_sender_registerer(T* This, const std::string& signal_name, Loki::TypeInfo signal_signature)
        {
            signal_registery::register_receiver(signal_name, Loki::TypeInfo(typeid(T)), (void*)This, signal_signature);
        }
    };
    template<typename T> signal_sender_registerer<T> register_sender(T* This, const std::string& signal_name, Loki::TypeInfo signal_signature)
    {
        return signal_sender_registerer<T>(This, signal_name, signal_signature);
    }
} // namespace Signals
