#pragma once
#include "signal_sink_base.h"
namespace signals
{
    template<class R, class...T> class signal_sink : public signal_sink_base
    {
    public:
        void handle_log(T... args)
        {

        }
    };

    template<template<class, class...> class SINK, class R, class...T> struct SinkCreator
    {
        static signal_sink_base* create()
        {
            return new SINK<R, T...>();
        }
    };
    template<template<class, class...> class S, class T> class SignalWithSink {};

    template<template<class, class...> class SINK, class R, class...T> class SignalWithSink<SINK, R(T...)> : public signal<R(T...)>
    {
    public:
        SignalWithSink() : signal<R(T...)>()
        {
            signal_sink_factory::instance()->register_creator(&SinkCreator<SINK, R, T...>::create, get_signal_type());
        }
    };
}
  