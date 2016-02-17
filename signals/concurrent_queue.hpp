#pragma once
#include "logging.hpp"

#include <boost/thread/recursive_mutex.hpp>
#include <queue>
#include <boost/thread.hpp>
template<typename T> void cleanup(T& ptr, typename std::enable_if< std::is_array<T>::value>::type* = 0) { /*delete[] ptr;*/ }
template<typename T> void cleanup(T& ptr, typename std::enable_if< std::is_pointer<T>::value && !std::is_array<T>::value>::type* = 0) { delete ptr; }
template<typename T> void cleanup(T& ptr, typename std::enable_if<!std::is_pointer<T>::value && !std::is_array<T>::value>::type* = 0) { return; }

template<typename Data>
class concurrent_notifier
{
private:
	boost::condition_variable the_condition_variable;
	std::vector<Data> the_data;
	mutable boost::mutex the_mutex;
public:
	void wait_for_data()
	{
		boost::mutex::scoped_lock lock(the_mutex);
		while (the_data.empty())
		{
			the_condition_variable.wait(lock);
		}
	}
	void wait_push(Data const& data)
	{
		boost::mutex::scoped_lock lock(the_mutex);
		while (!the_data.empty()) // Wait till the consumer pulls data from the queue
		{
			the_condition_variable.wait(lock);
		}
		the_data.push_back(data);
	}

	void push(Data const& data)
	{
		boost::mutex::scoped_lock lock(the_mutex);
		bool const was_empty = the_data.empty();
		if (the_data.size())
			the_data[0] = data;
		else
			the_data.push_back(data);

		lock.unlock(); // unlock the mutex

		if (was_empty)
		{
			the_condition_variable.notify_one();
		}
	}
	void wait_and_pop(Data& popped_value)
	{
		boost::mutex::scoped_lock lock(the_mutex);
		while (the_data.empty())
		{
			the_condition_variable.wait(lock);
		}

		popped_value = the_data[0];
		the_data.clear();
	}
	bool try_pop(Data& popped_value)
	{
		boost::mutex::scoped_lock lock(the_mutex);
		if (the_data.empty())
			return false;
		popped_value = the_data[0];
		the_data.clear();
		return true;
	}

	size_t size()
	{
		boost::mutex::scoped_lock lock(the_mutex);
		return the_data.size();
	}
	void clear()
	{
		boost::mutex::scoped_lock lock(the_mutex);
		the_data.clear();
	}
};

template<typename Data>
class concurrent_queue
{
private:
	boost::condition_variable the_condition_variable;
	std::queue<Data> the_queue;
	mutable boost::mutex the_mutex;
public:
	void wait_for_data()
	{
		boost::mutex::scoped_lock lock(the_mutex);
		while (the_queue.empty())
		{
			the_condition_variable.wait(lock);
		}
	}
	void wait_push(Data const& data)
	{
		boost::mutex::scoped_lock lock(the_mutex);
		while (!the_queue.empty()) // Wait till the consumer pulls data from the queue
		{
			the_condition_variable.wait(lock);
		}
		the_queue.push_back(data);
	}
	void push(Data const& data)
	{
		boost::mutex::scoped_lock lock(the_mutex);
		bool const was_empty = the_queue.empty();
		the_queue.push(data);

		lock.unlock(); // unlock the mutex

		if (was_empty)
		{
			the_condition_variable.notify_one();
		}
	}
	void wait_and_pop(Data& popped_value)
	{
		boost::mutex::scoped_lock lock(the_mutex);
		while (the_queue.empty())
		{
			the_condition_variable.wait(lock);
		}

		popped_value = the_queue.front();
		the_queue.pop();
	}
	bool try_pop(Data& popped_value)
	{
		boost::mutex::scoped_lock lock(the_mutex);
		if (the_queue.empty())
			return false;
		popped_value = the_queue.front();
		the_queue.pop();
		return true;
	}

	size_t size()
	{
		boost::mutex::scoped_lock lock(the_mutex);
		return the_queue.size();
	}
	void clear()
	{
		boost::mutex::scoped_lock lock(the_mutex);
		the_queue = std::queue<Data>();
	}
};