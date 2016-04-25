#include "connection.h"
#include "signal_base.h"
using namespace Signals;

connection::connection(signal_base* parent):
	_parent(parent) 
{
}

connection::~connection()
{
    if (_parent)
    {
        _parent->remove_receiver(this);
    }
}

void connection::set_parent(signal_base* parent)
{
	_parent = parent;
}