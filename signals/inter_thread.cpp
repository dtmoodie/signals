#include "inter_thread.h"
#include <mutex>
#include "concurrent_queue.hpp"
using namespace Signals;
struct impl
{
	std::map<boost::thread::id, std::pair<concurrent_queue<std::function<void(void)>>, std::function<void(void)>>> thread_queues;
	std::mutex mtx;
	static impl* inst()
	{
		static impl g_inst;
		return &g_inst;		
	}
	void register_notifier(const std::function<void(void)>& f, boost::thread::id id)
	{
		std::lock_guard<std::mutex> lock(mtx);
		thread_queues[id].second = f;
	}
	void push(const std::function<void(void)>& f, boost::thread::id id)
	{
		std::pair<concurrent_queue<std::function<void(void)>>, std::function<void(void)>>* queue = nullptr;
		{
			std::lock_guard<std::mutex> lock(mtx);
			queue = &thread_queues[id];
		}
		queue->first.push(f);
		if (queue->second)
			queue->second();
	}
	void run()
	{
		std::pair<concurrent_queue<std::function<void(void)>>, std::function<void(void)>>* queue = nullptr;
		{
			std::lock_guard<std::mutex> lock(mtx);
			queue = &thread_queues[boost::this_thread::get_id()];
		}
		std::function<void(void)> f;
		while (queue->first.try_pop(f))
		{
			f();
		}
	}
};
void thread_specific_queue::push(const std::function<void(void)>& f, boost::thread::id id)
{
	impl::inst()->push(f, id);
}
void thread_specific_queue::run()
{
	impl::inst()->run();
}
void thread_specific_queue::register_notifier(const std::function<void(void)>& f, boost::thread::id id)
{
	impl::inst()->register_notifier(f, id);
}