#include "inter_thread.h"
#include <mutex>
#include "concurrent_queue.hpp"
using namespace Signals;
struct impl
{
	std::map<size_t,  // Thread id
		std::pair<
			concurrent_queue<std::pair<std::function<void(void)>, void*>>,  // queue of function + obj*
			std::function<void(void)>> // callback on push to queue
	> thread_queues;
	std::mutex mtx;
	static impl* inst()
	{
		static impl g_inst;
		return &g_inst;		
	}
	void register_notifier(const std::function<void(void)>& f, size_t id)
	{
		std::lock_guard<std::mutex> lock(mtx);
		thread_queues[id].second = f;
	}
	void push(const std::function<void(void)>& f, size_t id, void* obj)
	{
		std::pair<concurrent_queue<std::pair<std::function<void(void)>, void*>>, std::function<void(void)>>* queue = nullptr;
		{
			std::lock_guard<std::mutex> lock(mtx);
			queue = &thread_queues[id];
		}
        if(queue->first.size() > 100)
            LOG(warning) << "Queue overflow " << queue->first.size() << " for thread " << id;
		queue->first.push(std::pair<std::function<void(void)>, void*>(f, obj));
		if (queue->second)
			queue->second();
	}
	void run()
	{
		std::pair<concurrent_queue<std::pair<std::function<void(void)>, void*>>, std::function<void(void)>>* queue = nullptr;
		{
			std::lock_guard<std::mutex> lock(mtx);
			queue = &thread_queues[get_this_thread()];
		}
		std::pair<std::function<void(void)>, void*> f;
		while (queue->first.try_pop(f))
		{
			f.first();
		}
	}
    void run_once()
    {
		std::pair<concurrent_queue<std::pair<std::function<void(void)>, void*>>, std::function<void(void)>>* queue = nullptr;
		{
			std::lock_guard<std::mutex> lock(mtx);
			queue = &thread_queues[get_this_thread()];
		}
		std::pair<std::function<void(void)>, void*> f;
		if(queue->first.try_pop(f))
		{
			f.first();
		}
    }
	void remove_from_queue(void* obj)
	{
		std::lock_guard<std::mutex> lock(mtx);
		for (auto& queue: thread_queues)
		{
			for (auto itr = queue.second.first.begin(); itr != queue.second.first.end(); )
			{
				if(itr->second == obj)
				{
					itr = queue.second.first.erase(itr);
				}
				else
				{
					++itr;
				}
			}
		}
	}
};
void thread_specific_queue::push(const std::function<void(void)>& f, size_t id, void* obj)
{
	impl::inst()->push(f, id, obj);
}
void thread_specific_queue::run()
{
	impl::inst()->run();
}
void thread_specific_queue::register_notifier(const std::function<void(void)>& f, size_t id)
{
	impl::inst()->register_notifier(f, id);
}
void thread_specific_queue::run_once()
{
	impl::inst()->run_once();
}
void thread_specific_queue::remove_from_queue(void* obj)
{
	impl::inst()->remove_from_queue(obj);
}