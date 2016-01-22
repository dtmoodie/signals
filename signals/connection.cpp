#include "connection.h"
#include "signal_base.h"
using namespace signals;

connection::connection(int index, signal_base* parent): 
    _index(index)
    , _parent(parent) 
{
}

connection::~connection()
{
    if (_parent)
    {
        _parent->remove_receiver(_index);
    }
}