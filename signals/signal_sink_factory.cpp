
#include "signal_sink_factory.h"
#include "signal_sink_base.h"
using namespace Signals;
signal_sink_factory* signal_sink_factory::instance()
{
    static signal_sink_factory* inst = nullptr;
    if (inst == nullptr)
    {
        inst = new signal_sink_factory();
    }
    return inst;
}

std::shared_ptr<signal_sink_base> signal_sink_factory::create_log_sink(Loki::TypeInfo type)
{
    auto itr = registry.find(type);
    if (itr != registry.end())
    {
        return std::shared_ptr<signal_sink_base>(itr->second());
    }
    return std::shared_ptr<signal_sink_base>();
}
void signal_sink_factory::register_creator(std::function<signal_sink_base*(void)> f, Loki::TypeInfo type)
{
    registry[type] = f;
}