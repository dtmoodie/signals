#pragma once

#include "LokiTypeInfo.h"
#include "thread_registry.h"
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
	public:
		virtual void add_log_sink(std::shared_ptr<signal_sink_base> sink, size_t id = get_this_thread()) = 0;
        virtual Loki::TypeInfo get_signal_type() = 0;
    };
}
