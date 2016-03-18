#include "std_thread.h"

#include <sstream>
size_t Signals::get_thread_id(const std::thread::id& id)
{
    std::stringstream ss;
    ss << id;
    size_t output;
    ss >> output;
    return output;
}