#include "signaler.h"

using namespace Signals;
signaler::signaler()
{
	_sig_manager = nullptr;
}
void signaler::setup_signals(signal_manager* mgr)
{
	_sig_manager = mgr;
}