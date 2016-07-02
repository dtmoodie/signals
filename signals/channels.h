#pragma once
#include "Defs.h"
#include <future>
#include <functional>
#include "inter_thread.h"

namespace Signals
{
    template<class T> class Channel {};
    template<class T> class QueuedChannel : public Channel<T> {};

    template<class R, class...T> struct executor
    {
        std::promise<R> promise;
        void exec_on_thread(const std::function<R(T...)> f, T... args)
        {
            promise.set_value(f(args...));
            delete this;
        }
        void exec(const std::function<R(T...)> f, T... args)
        {
            promise.set_value(f(args...));
        }
    };
    template<class...T> struct executor<void, T...>
    {
        std::promise<void> promise;
        void exec_on_thread(const std::function<void(T...)> f, T... args)
        {
            f(args...);
            promise.set_value();
        }
        void exec(const std::function<void(T...)> f, T... args)
        {
            f(args...);
            promise.set_value();
        }
    };

    template<class R, class...T> struct Channel<R(T...)>
    {

        virtual std::future<R> exec(const std::function<R(T...)>& f, T... args)
        {
            executor<R, T...> exec;
            exec.exec(f, args...);
            return exec.promise.get_future();
        }
    };

    template<class...T> struct Channel<void(T...)>
    {
        virtual std::future<void> exec(const std::function<void(T...)>& f, T... args)
        {
            executor<void, T...> exec;
            exec.exec(f, args...);
            return exec.promise.get_future();
        }
    };

    template<class R, class...T> struct QueuedChannel<R(T...)> : public Channel<R(T...)>
    {
        size_t destination_thread;
        QueuedChannel(size_t dest) : destination_thread(dest) {}

        virtual std::future<R> exec(const std::function<R(T...)>& f, T... args)
        {
            std::shared_ptr<executor<R,T...>> exec(new executor<R, T...>());
            
            //thread_specific_queue::push(std::bind(&executor<R, T...>::exec_on_thread, exec, f, args...), destination_thread);
            thread_specific_queue::push(std::bind([f, exec](T... args_)->void
            {
                exec->exec_on_thread(f, args_...);
            }, args...), destination_thread);

            return exec->promise.get_future();
        }
    };
    template<class...T> struct QueuedChannel<void(T...)> : public Channel<void(T...)>
    {
        size_t destination_thread;
        QueuedChannel(size_t dest) : destination_thread(dest) {}
        ~QueuedChannel()
        {
            thread_specific_queue::remove_from_queue(this);
        }
        virtual std::future<void> exec(const std::function<void(T...)>& f, T... args)
        {
            std::shared_ptr<executor<void,T...>> exec(new executor<void, T...>());

            thread_specific_queue::push(std::bind([f, exec](T... args_)->void
            {
                exec->exec_on_thread(f, args_...);
            }, args...), destination_thread, this);

            return exec->promise.get_future();
        }
    };
}
