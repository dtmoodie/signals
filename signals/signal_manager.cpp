#include "signal_manager.h"
using namespace Signals;

signal_manager::signal_manager()
{
}

static signal_manager* g_instance = nullptr;


signal_manager* signal_manager::get_instance()
{
    
    if (g_instance == nullptr)
    {
        g_instance = new signal_manager();
    }
    return g_instance;
}
void signal_manager::set_instance(signal_manager* inst)
{
    g_instance = inst;
}

void signal_manager::register_thread(int type, boost::thread::id id)
{
    std::lock_guard<std::mutex> lock(mtx);
    auto& threads = _thread_map[type];
    if (std::count(threads.begin(), threads.end(), id) == 0)
        threads.push_back(id);
}
boost::thread::id signal_manager::get_thread(int type)
{
    std::lock_guard<std::mutex> lock(mtx);
    // TODO some kind of load balancing for multiple threads of a specific type
    auto current_thread = boost::this_thread::get_id();
    auto itr = _thread_map.find(type);
    if (type != 0 && itr != _thread_map.end())
    {
        if (itr->second.size())
        {
            if (std::count(itr->second.begin(), itr->second.end(), current_thread) == 0) // If the current thread is not of appropriate type
                return itr->second.back();
        }
    }
    return current_thread;
}
std::shared_ptr<signal_base>& signal_manager::get_signal(const std::string& name, Loki::TypeInfo type)
{
    return _signals[type][name];
}

void signal_registery::register_sender(const std::string& signal_name, Loki::TypeInfo type, void* sender, Loki::TypeInfo signal_signature)
{
    std::cout << "Registering singal emitter " << type.name() << " of signal " << signal_name << " with signature " << signal_signature.name() << std::endl;
}
void signal_registery::register_receiver(const std::string& signal_name, Loki::TypeInfo type, void* receiver, Loki::TypeInfo signal_signature)
{
    std::cout << "Registering singla receiver " << type.name() << " of signal " << signal_name << " with signature " << signal_signature.name() << std::endl;
}