#pragma once
#include "Defs.h"
#include <boost/thread.hpp>
#include <functional>

namespace Signals
{
    class SIGNAL_EXPORTS thread_specific_queue
    {
    public:
        static void push(const std::function<void(void)>& f, boost::thread::id id = boost::this_thread::get_id());
        static void run();
        static void run_once();
		// Register a notifier function to signal new data input onto a queue
		static void register_notifier(const std::function<void(void)>& f, boost::thread::id id = boost::this_thread::get_id());
    };
} // namespace Signals
