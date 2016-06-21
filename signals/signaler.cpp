#include "signaler.h"
#include "signal_manager.h"
#include "inter_thread.h"
using namespace Signals;
signaler::signaler()
{
	_sig_manager = nullptr;
}
signaler::~signaler()
{
	if (_sig_manager)
	{
		_sig_manager->remove_sender(this);
	}
	thread_specific_queue::remove_from_queue(this);
}
int signaler::setup_signals(signal_manager* mgr)
{
	_sig_manager = mgr;
	return 0;
}

bool signaler::disconnect_from_signal(Signals::signal_base* signal)
{
	return signaler::disconnect(signal);
}

bool signaler::disconnect(Signals::signal_base* signal)
{
	if(signal == nullptr)
		return false;
    for(auto itr = _connections.begin(); itr != _connections.end(); ++itr)
    {
        if(itr->first == signal)
        {
            _connections.erase(itr);
            return true;
        }
    }
	return false;
}

int signaler::disconnect()
{
	return disconnect(_sig_manager);
}

int signaler::disconnect(std::string name)
{
   return disconnect(name, _sig_manager);
}

int signaler::connect_by_name(const std::string& name)
{
	return connect_by_name(name, _sig_manager);}
