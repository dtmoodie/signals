#pragma once

#include "LokiTypeInfo.h"
#include <memory>

namespace Signals
{
    class signal_sink_base;
    class connection;

    class signal_base
    {
    protected:
        friend class connection;
        virtual void remove_receiver(int index) = 0;
        virtual void add_log_sink(std::shared_ptr<signal_sink_base> sink) = 0;
        virtual Loki::TypeInfo get_signal_type() = 0;
    };
}
