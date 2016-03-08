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
#include "boost/type_traits/function_traits.hpp"
#include "meta_signal.hpp"

namespace Signals
{

	template<class Signature,
	template<class> class Combiner = default_combiner
	> class typed_signal_base { };


	template<class R, class...T, template<class>class Combiner> class typed_signal_base<R(T...), Combiner> : public signal_base, public meta_signal<R(T...)>
    {
    protected:
        std::map<int, std::function<R(T...)> >				receivers;
        std::map<int, std::shared_ptr<Channel<R(T...)>>>	channels;
		std::map<int, std::shared_ptr<Channel<void(T...)>>> log_channels;
		std::map<int, std::function<void(T...)> >			log_sinks;
		std::string											signal_description;
        std::list<int>										unused_indexes;
        std::mutex											mtx;
		
        friend class connection;
        virtual void remove_receiver(int index)
        {
            std::lock_guard<std::mutex> lock(mtx);
            auto itr = receivers.find(index);
            if (itr != receivers.end())
            {
                receivers.erase(index);
                auto itr2 = channels.find(index);
                channels.erase(itr2);
                unused_indexes.push_back(index);
				return;
            }
			auto itr2 = log_sinks.find(index);
			if (itr2 != log_sinks.end())
			{
				log_sinks.erase(itr2);
				auto chan_itr = log_channels.find(index);
				log_channels.erase(chan_itr);
				unused_indexes.push_back(index);
				return;
			}
        }
    public:
		typed_signal_base(const std::string& description = "") :
			signal_description(description)
        {
			for (int i = 0; i < 256; ++i)
                unused_indexes.push_back(i);
        }
        virtual void add_log_sink(std::shared_ptr<signal_sink_base> sink, boost::thread::id destination_thread = boost::this_thread::get_id())
        {
            auto typed_sink = std::dynamic_pointer_cast<signal_sink<R(T...)>>(sink);
			if (typed_sink)
			{
				typed_sink->set_connection(connect_log_sink(my_bind(&signal_sink<R(T...)>::handle_signal, typed_sink.get(), make_int_sequence<sizeof...(T) >{}), destination_thread));
			}
        }

		std::shared_ptr<connection> connect(const std::function<R(T...)>& f, boost::thread::id destination_thread = boost::this_thread::get_id(), bool force_queue = false)
        {
            std::lock_guard<std::mutex> lock(mtx);
            int index = unused_indexes.back();
            unused_indexes.pop_back();
            receivers[index] = f;
            if (destination_thread != boost::this_thread::get_id() || force_queue)
                channels[index] = std::shared_ptr<Channel<R(T...)>>(new QueuedChannel<R(T...)>(destination_thread));
            else
                channels[index] = std::shared_ptr<Channel<R(T...)>>(new Channel<R(T...)>());
            return std::shared_ptr<connection>(new connection(index, this));
        }

		std::shared_ptr<connection> connect_log_sink(const std::function<void(T...)>& f, boost::thread::id destination_thread = boost::this_thread::get_id())
		{
			std::lock_guard<std::mutex> lock(mtx);
			int index = unused_indexes.back();
			unused_indexes.pop_back();
			log_sinks[index] = f;
			if (destination_thread != boost::this_thread::get_id())
				log_channels[index] = std::shared_ptr<Channel<void(T...)>>(new QueuedChannel<void(T...)>(destination_thread));
			else
				log_channels[index] = std::shared_ptr<Channel<void(T...)>>(new Channel<void(T...)>());

			return std::shared_ptr<connection>(new connection(index, this));
		}

        std::shared_ptr<connection> connect(const std::function<R(T...)>& f, int dest_thread_type, bool force_queued = false)
        {
            std::lock_guard<std::mutex> lock(mtx);
            int index = unused_indexes.back();
            unused_indexes.pop_back();
            receivers[index] = f;
            auto destination_thread = thread_registry::get_instance()->get_thread(dest_thread_type);

            if (destination_thread != boost::this_thread::get_id() || force_queued)
                channels[index] = std::shared_ptr<Channel<R(T...)>>(new QueuedChannel<R(T...)>(destination_thread));
            else
                channels[index] = std::shared_ptr<Channel<R(T...)>>(new Channel<R(T...)>());

            return std::shared_ptr<connection>(new connection(index, this));
        }

		Combiner<typename boost::function_traits<R(T...)>::result_type> operator()(T... args)
        {
            std::lock_guard<std::mutex> lock(mtx); // Lock against adding new connections while executing
			Combiner<typename boost::function_traits<R(T...)>::result_type> combiner;
            for(auto& itr : log_sinks)
            {
                log_channels[itr.first]->exec(itr.second, args...);
            }
            for (auto& itr : receivers)
            {
                combiner(channels[itr.first]->exec(itr.second, args...));
            }
            return combiner;
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

    template<class R, class...T, template<class>class Combiner, template<class...>class Sink> class typed_signal<R(T...), Combiner, Sink> : public typed_signal_base<R(T...), Combiner>, public meta_signal<R(T...)>
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
