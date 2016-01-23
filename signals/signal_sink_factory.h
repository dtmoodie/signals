#pragma once
#include <functional>
#include <memory>
#include <map>
#include "LokiTypeInfo.h"
#include "Defs.h"

namespace Signals
{
    class SIGNAL_EXPORTS signal_sink_base;

    class SIGNAL_EXPORTS signal_sink_factory
    {
        std::map<Loki::TypeInfo, std::function<signal_sink_base*(void)> > registry;
    public:
        static signal_sink_factory* instance();

        void register_creator(std::function<signal_sink_base*(void)> f, Loki::TypeInfo type);
        std::shared_ptr<signal_sink_base> create_log_sink(Loki::TypeInfo type);
    };
}
