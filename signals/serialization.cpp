#include "serialization.h"
#include "signal_base.h"
#include "signal.h"
#include "logging.hpp"

using namespace Signals;
using namespace Signals::serialization;
using namespace Signals::serialization::text;
serialization_proxy_base::serialization_proxy_base()
{
    _dest = nullptr;
}
serialization_proxy_base::~serialization_proxy_base()
{

}

void serialization_proxy_base::set_output_iostream(std::ostream* stream)
{
    _dest = stream;
}
serialization_proxy_base* factory::get_proxy(signal_base* signal)
{
	return get_proxy(signal->get_signal_type());
}

serialization_proxy_base* factory::get_proxy(Loki::TypeInfo type)
{
	if (registry.count(type))
		return registry[type]();
	return nullptr;
}

void factory::register_proxy(std::function<serialization_proxy_base*()> function, Loki::TypeInfo type)
{
#ifndef _MSC_VER
	// Currently this funcitonality is broken on msvc for an unknown reason.
    registry[type] = function;
#endif
}

factory* factory::instance()
{
    static factory inst;
    return &inst;
}

void serialization_proxy_base::install(signal_base* signal)
{
	LOG(debug) << "Default install called for " << signal->get_signal_type().name();
}
void serialization_proxy_base::send(signal_base* signal, std::string string)
{
	LOG(debug) << "Default emit called for " << signal->get_signal_type().name();
}
