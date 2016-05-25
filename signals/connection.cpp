#include "connection.h"
#include "signal_base.h"
#include "inter_thread.h"
using namespace Signals;

connection::connection(const boost::signals2::connection& connection_):
	_connection(connection_)
{

}
connection::~connection()
{

}

class_connection::class_connection(const boost::signals2::connection& connection_, void* connecting_class_):
	connection(connection_), _connecting_class(connecting_class_)
{

}
class_connection::~class_connection()
{
	thread_specific_queue::remove_from_queue(_connecting_class);
}
