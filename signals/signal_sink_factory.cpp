
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
std::set<std::string> signal_sink_factory::list_available_sinks()
{
	std::set<std::string> output;
	for (auto& itr : registry)
	{
		output.insert(itr.first);
	}
	return output;
}
std::set<std::string> signal_sink_factory::list_available_sinks(Loki::TypeInfo type)
{
	std::set<std::string> output;
	for (auto& itr : registry)
	{
		if (itr.second.count(type))
			output.insert(itr.first);
	}
	return output;
}
std::shared_ptr<signal_sink_base> signal_sink_factory::create_log_sink(Loki::TypeInfo type, const std::string& sink_type)
{
    auto itr = registry.find(sink_type);
    if (itr != registry.end())
    {
		auto itr2 = itr->second.find(type);
        return std::shared_ptr<signal_sink_base>(itr2->second());
    }
    return std::shared_ptr<signal_sink_base>();
}
void signal_sink_factory::register_creator(std::function<signal_sink_base*(void)> f, Loki::TypeInfo type, const std::string& sink_type)
{
    registry[sink_type][type] = f;
}