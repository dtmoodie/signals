#pragma once
#include "Defs.h"

namespace signals
{
    class signal_base;
    class SIGNAL_EXPORTS connection
    {
        int _index;
        signal_base* _parent;
    public:
        connection(int index, signal_base* parent);
        ~connection();
    };
} // namespace signals