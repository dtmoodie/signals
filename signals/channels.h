#pragma once
#include "Defs.h"
#include <future>
#include <functional>
#include "inter_thread.h"

namespace Signals
{
    template<class T> class Channel {};
    template<class T> class QueuedChannel : public Channel<T> {};

    template<class R, class...T> struct Channel<R(T...)>
    {
        virtual std::future<R> exec(const std::function<R(T...)>& f, T... args)
        {
            return std::future<R>(f(args...));
        }
    };

    template<class...T> struct Channel<void(T...)>
    {
        virtual std::future<void> exec(const std::function<void(T...)>& f, T... args)
        {
            f(args...);
            return std::future<void>();
        }
    };

    template<class R, class...T> struct executor
    {
        std::promise<R> promise;
        void exec_on_thread(const std::function<R(T...)> f, T... args)
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
            delete this;
        }
    };

    template<class R, class...T> struct QueuedChannel<R(T...)> : public Channel<R(T...)>
    {
        boost::thread::id destination_thread;
        QueuedChannel(boost::thread::id dest) : destination_thread(dest) {}

        virtual std::future<R> exec(const std::function<R(T...)>& f, T... args)
        {
            auto exec = new executor<R, T...>();
            thread_specific_queue::push(std::bind(&executor<R, T...>::exec_on_thread, exec, f, args...), destination_thread);

            return exec->promise.get_future();
        }
    };
}
