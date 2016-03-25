#pragma once
#include "Defs.h"

#include <memory>

namespace Signals
{
    class signal_base;
    class SIGNAL_EXPORTS connection
    {
        int _index;
        signal_base* _parent;
    public:
		typedef std::shared_ptr<connection> Ptr;
        connection(int index, signal_base* parent);
        ~connection();
    };
} // namespace Signals