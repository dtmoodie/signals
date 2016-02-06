#include "thread_registry.h"

using namespace Signals;

thread_registry::thread_registry()
{

}

void thread_registry::register_thread(int type, boost::thread::id id)
{
    std::lock_guard<std::mutex> lock(mtx);
    auto& threads = _thread_map[type];
    if (std::count(threads.begin(), threads.end(), id) == 0)
        threads.push_back(id);
}

boost::thread::id thread_registry::get_thread(int type)
{
    std::lock_guard<std::mutex> lock(mtx);
    // TODO some kind of load balancing for multiple threads of a specific type
    auto current_thread = boost::this_thread::get_id();
    auto itr = _thread_map.find(type);
    if (type != 0 && itr != _thread_map.end())
    {
        if (itr->second.size())
        {
            if (std::count(itr->second.begin(), itr->second.end(), current_thread) == 0) // If the current thread is not of appropriate type
                return itr->second.back();
        }
    }
    return current_thread;
}

thread_registry* thread_registry::get_instance()
{
    thread_registry* inst = nullptr;
    if(inst == nullptr)
        inst = new thread_registry();
    return inst;
}
