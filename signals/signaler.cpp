#include "signaler.h"
#include "signal_manager.h"
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
}
void signaler::setup_signals(signal_manager* mgr)
{
	_sig_manager = mgr;
}
void signaler::disconnect(Signals::signal_base* signal)
{
    for(auto itr = _connections.begin(); itr != _connections.end(); ++itr)
    {
        if(itr->first == signal)
        {
            _connections.erase(itr);
            return;
        }
    }
}
void signaler::disconnect(std::string name)
{
    
}