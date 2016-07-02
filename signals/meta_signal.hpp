#pragma once
#include "Defs.h"
#include "serialization.h"

namespace Signals
{
    template<class Sig> class SIGNAL_EXPORTS meta_signal
    {
    public:
        
    };

    template<class R, class... T> class SIGNAL_EXPORTS meta_signal<R(T...)>
    {
    public:
        meta_signal()
        {
            (void)&constructor;
        }
        static serialization::text::constructor<R(T...)> constructor;
    };
    template<class R, class... T> serialization::text::constructor<R(T...)> meta_signal<R(T...)>::constructor;
}