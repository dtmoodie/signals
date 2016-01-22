#include "signal_sink_base.h"
#include "connection.h"

using namespace signals;

void signal_sink_base::set_connection(std::shared_ptr<connection> connection) 
{ 
    _connection = connection; 
}