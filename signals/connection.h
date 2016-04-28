#pragma once
#include "Defs.h"

#include <memory>
#include <boost/signals2/connection.hpp>
namespace Signals
{
    typedef boost::signals2::scoped_connection connection;
    
} // namespace Signals