#pragma once
#include "Defs.h"
#include <map>
#include <boost/thread.hpp>
#include "CudaUtils.hpp"

namespace Signals
{
    class SIGNAL_EXPORTS thread_specific_queue
    {
    protected:
        static std::map<boost::thread::id, EagleLib::concurrent_queue<std::function<void(void)>>> thread_queues;
    public:
        static void push(const std::function<void(void)>& f, boost::thread::id id = boost::this_thread::get_id());
        static void run();
    };
} // namespace Signals
