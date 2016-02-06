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
