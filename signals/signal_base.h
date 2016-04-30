#pragma once

#include "LokiTypeInfo.h"
#include "thread_registry.h"
#include "connection.h"
#include <memory>

namespace Signals
{
    class signal_sink_base;

    class signal_base
    {
    protected:
		std::string _description;
		size_t _owning_thread;
	public:
		signal_base(std::string description = "", size_t owning_thread = get_this_thread()):
			_description(description), _owning_thread(owning_thread)
		{
		}
		virtual void add_log_sink(std::shared_ptr<signal_sink_base> sink, size_t id = get_this_thread()) = 0;
        virtual Loki::TypeInfo get_signal_type() = 0;
		virtual std::string get_description() const
		{
			return _description;
		}
		virtual size_t get_owning_thread() const
		{
			return _owning_thread;
		}
		virtual void set_description(std::string description)
		{
			_description = description;
		}
		virtual void set_owning_thread(size_t thread)
		{
			_owning_thread = thread;
		}
    };
}
