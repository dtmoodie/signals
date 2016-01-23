#include "signal_sink_base.h"
#include "connection.h"

using namespace Signals;

void signal_sink_base::set_connection(std::shared_ptr<connection> connection) 
{ 
    _connection = connection; 
}