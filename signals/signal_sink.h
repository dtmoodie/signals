#pragma once
#include "signal_sink_base.h"
namespace Signals
{
    template<class...T> class signal_sink : public signal_sink_base
    {

    }; 
    template<class R, class...T> class signal_sink<R(T...)>: public signal_sink_base
    {
    public:
        static const char* sink_type()
        {
            return "null_sink";
        }
        virtual void handle_signal(T... args)
        {
            
        }
        virtual void handle_return(R ret)
        {

        }
    };
    template<class...T> class signal_sink<void(T...)> : public signal_sink_base
    {
    public:
        static const char* sink_type()
        {
            return "null_sink";
        }
        virtual void handle_signal(T... args)
        {

        }
        virtual void handle_return()
        {

        }
    };
}
  