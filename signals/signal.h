#pragma once
#include "Defs.h"
#include "placeholder.h"
#include "signal_manager.h"
#include "signal_base.h"
#include "channels.h"
#include "signal_sink_base.h"
#include "signal_sink.h"
#include "connection.h"

namespace Signals
{
    template<class T> class signal { };
    template<class R, class...T> class signal<R(T...)> : public signal_base
    {
    protected:
        std::map<int, std::function<R(T...)> > receivers;
        std::map<int, std::shared_ptr<Channel<R(T...)>>> channels;
        std::list<int> unused_indexes;
        std::mutex mtx;
        std::shared_ptr<connection> serializer;

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
            }
        }
    public:
        signal()
        {
            for (int i = 0; i < 256; ++i)
                unused_indexes.push_back(i);
        }
        virtual void add_log_sink(std::shared_ptr<signal_sink_base> sink)
        {
            auto typed_sink = std::dynamic_pointer_cast<signal_sink<R, T...>>(sink);
            typed_sink->set_connection(connect(my_bind(&signal_sink<R, T...>::handle_log, typed_sink.get(), make_int_sequence<sizeof...(T) >{})));
        }

        std::shared_ptr<connection> connect(const std::function<R(T...)>& f, boost::thread::id destination_thread = boost::this_thread::get_id())
        {
            std::lock_guard<std::mutex> lock(mtx);
            int index = unused_indexes.back();
            unused_indexes.pop_back();
            receivers[index] = f;
            if (destination_thread != boost::this_thread::get_id())
                channels[index] = std::shared_ptr<Channel<R(T...)>>(new QueuedChannel<R(T...)>(destination_thread));
            else
                channels[index] = std::shared_ptr<Channel<R(T...)>>(new Channel<R(T...)>());

            return std::shared_ptr<connection>(new connection(index, this));
        }
        std::shared_ptr<connection> connect(const std::function<R(T...)>& f, int dest_thread_type)
        {
            std::lock_guard<std::mutex> lock(mtx);
            int index = unused_indexes.back();
            unused_indexes.pop_back();
            receivers[index] = f;
            auto destination_thread = signal_manager::get_thread(dest_thread_type);

            if (destination_thread != boost::this_thread::get_id())
                channels[index] = std::shared_ptr<Channel<R(T...)>>(new QueuedChannel<R(T...)>(destination_thread));
            else
                channels[index] = std::shared_ptr<Channel<R(T...)>>(new Channel<R(T...)>());

            return std::shared_ptr<connection>(new connection(index, this));
        }

        R operator()(T... args)
        {
            std::lock_guard<std::mutex> lock(mtx); // Lock against adding new connections while executing
            for (auto& itr : receivers)
            {
                channels[itr.first]->exec(itr.second, args...);
            }
        }
        virtual Loki::TypeInfo get_signal_type()
        {
            return Loki::TypeInfo(typeid(R(T...)));
        }
    };
}
