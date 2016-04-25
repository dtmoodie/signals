#pragma once
#include "Defs.h"

#include <memory>

namespace Signals
{
    class signal_base;
    class SIGNAL_EXPORTS connection
    {
        signal_base* _parent;
    public:
		typedef std::shared_ptr<connection> Ptr;
        connection(signal_base* parent);
		void set_parent(signal_base* parent);
        ~connection();
    };
} // namespace Signals