#include "boost_thread.h"
#include <sstream>

size_t Signals::get_thread_id(const boost::thread::id& id)
{
    std::stringstream ss;
    ss << std::hex << id;
    size_t output;
    ss >> output;
    return output;
}