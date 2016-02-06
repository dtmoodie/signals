#pragma once
#include "signal.h"
namespace Signals
{
    template<template<class, class...> class SINK, class R, class...T> struct sink_creator
    {
        static signal_sink_base* create()
        {
            return new SINK<R, T...>();
        }
    };
    
    template<template<class, class...> class S, class T> class signal_with_sink {};

    template<template<class, class...> class SINK, class R, class...T> class signal_with_sink<SINK, R(T...)> : public signal<R(T...)>
    {
    public:
        signal_with_sink() : signal<R(T...)>()
        {
            signal_sink_factory::instance()->register_creator(&sink_creator<SINK, R, T...>::create, signal<R(T...)>::get_signal_type());
        }
    };    
}
