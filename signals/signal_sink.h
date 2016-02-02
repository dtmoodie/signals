#pragma once
#include "signal_sink_base.h"
namespace Signals
{
    template<class R, class...T> class signal_sink : public signal_sink_base
    {
    public:
        R handle_log(T... args)
        {
            return R();
        }
    };    
    template<class...T> class signal_sink<void,T...> : public signal_sink_base
    {
    public:
        void handle_log(T... args)
        {
            
        }
    };
}
  