#pragma once
#if (defined WIN32 || defined _WIN32 || defined WINCE || defined __CYGWIN__) && defined signals_EXPORTS
#  define SIGNAL_EXPORTS __declspec(dllexport)
#elif defined __GNUC__ && __GNUC__ >= 4
#  define SIGNAL_EXPORTS __attribute__ ((visibility ("default")))
#else
#  define SIGNAL_EXPORTS
#endif

#ifndef signals_EXPORTS
#ifdef _DEBUG
#pragma comment(lib, "signalsd.lib")
#else
#pragma comment(lib, "signals.lib")
#endif
#endif
//#define THREAD_TYPE_ANY 0
//#define THREAD_TYPE_GUI 1
//#define THREAD_TYPE_PROCESSING 2

namespace Signals
{
    enum thread_type
    {
        ANY,
        GUI, 
        processing
    };
}
