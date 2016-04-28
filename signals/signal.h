#pragma once
#include "Defs.h"
#include "placeholder.h"
#include "thread_registry.h"
#include "signal_base.h"
#include "channels.h"
#include "signal_sink_base.h"
#include "signal_sink.h"
#include "connection.h"
#include "combiner.h"
#include "signal_sink_factory.h"
#include "serialization.h"
#include "meta_signal.hpp"
#include <boost/signals2.hpp>
#include <list>
namespace Signals
{
	template<class R, class...T, template<class>class Combiner> class typed_signal_base<R(T...), Combiner> : public signal_base, public meta_signal<R(T...)>, public boost::signals2::signal<R(T...)>
    {
    protected:
        size_t _owning_thread;
        std::string _signal_description;
    public:
		typed_signal_base(const std::string& description = "", size_t owning_thread_ = get_this_thread()) :
			_signal_description(description),
			_owning_thread(owning_thread_)
        {
        }
		~typed_signal_base()
		{
		}
        virtual void add_log_sink(std::shared_ptr<signal_sink_base> sink, size_t destination_thread = get_this_thread())
        {
            
        }

		std::shared_ptr<connection> connect(const std::function<R(T...)>& f, size_t destination_thread = get_this_thread(), bool force_queue = false)
        {
            if(destination_thread != get_this_thread() || force_queue)
            {
                auto f_ = [f, destination_thread](T... args)
                {
                    // Lambda function receives the call from the boost signal and then pipes the actual function call
                    // over a queue to the correct thread
                    thread_specific_queue::push(std::bind([f](T... args_)->void
                    {
                        f(args_...);
                    },args...), destination_thread);
                };
                return std::shared_ptr<connection>(new connection(boost::signals2::signal<R(T...)>::connect(f_)));
            }else
            {
                return std::shared_ptr<connection>(new connection(boost::signals2::signal<R(T...)>::connect(f)));
            }
        }

		std::shared_ptr<connection> connect_log_sink(const std::function<void(T...)>& f, size_t destination_thread = get_this_thread())
		{
            return connect(f, destination_thread);
		}

        std::shared_ptr<connection> connect(const std::function<R(T...)>& f, int dest_thread_type, bool force_queued = false)
        {
            return connect(f);
        }

		//Combiner<typename boost::function_traits<R(T...)>::result_type> operator()(T... args)
        void operator()(T... args)
        {
            boost::signals2::signal<R(T...)>::operator()(args...);
        }
        virtual Loki::TypeInfo get_signal_type()
        {
            return Loki::TypeInfo(typeid(R(T...)));
        }
    };
	template<class Signature,
		template<class> class Combiner = default_combiner,
		template<class...>class Sink = signal_sink
	> class typed_signal : public typed_signal_base<Signature, Combiner>{ };

    template<class R, class...T, template<class>class Combiner, template<class...>class Sink> class typed_signal<R(T...), Combiner, Sink> : public typed_signal_base<R(T...), Combiner>
	{
	public:
		static sink_constructor<R(T...), Sink<R(T...)>> _sink_constructor;
		typed_signal(const std::string& description = ""):
			typed_signal_base<R(T...), Combiner>(description)
		{
			(void)&_sink_constructor;
		}
	};

	template<class R, class...T, 
        template<class> class Combiner,
        template<class...> class Sink>
    sink_constructor<R(T...), Sink<R(T...)>> typed_signal<R(T...), Combiner, Sink>::_sink_constructor;
}
