#pragma once
#if (defined WIN32 || defined _WIN32 || defined WINCE || defined __CYGWIN__) && defined signals_EXPORTS
#  define SIGNAL_EXPORTS __declspec(dllexport)
#elif defined __GNUC__ && __GNUC__ >= 4
#  define SIGNAL_EXPORTS __attribute__ ((visibility ("default")))
#else
#  define SIGNAL_EXPORTS
#endif