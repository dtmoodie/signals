#pragma once
#include <boost/thread.hpp>
#include <map>
#include <mutex>
#include <vector>
namespace Signals
{
    class thread_registry
    {
        std::map<int, std::vector<boost::thread::id>> _thread_map;
        thread_registry();
        std::mutex mtx;
    public:
        virtual void register_thread(int type, boost::thread::id id = boost::this_thread::get_id());

        virtual boost::thread::id get_thread(int type);
        static thread_registry* get_instance();
    };

}
