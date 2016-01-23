#include "inter_thread.h"

using namespace Signals;
void thread_specific_queue::push(const std::function<void(void)>& f, boost::thread::id id)
{
    thread_queues[id].push(f);
}
void thread_specific_queue::run()
{
    std::function<void(void)> f;
    auto& queue = thread_queues[boost::this_thread::get_id()];
    while (queue.try_pop(f))
    {
        f();
    }
}

std::map<boost::thread::id, EagleLib::concurrent_queue<std::function<void(void)>>> thread_specific_queue::thread_queues;