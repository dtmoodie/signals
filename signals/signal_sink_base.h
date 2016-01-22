#pragma once
#include <memory>
#include "signal.h"
#pragma once
#include "Defs.h"
#include <memory>

namespace signals
{
    class connection;

    class SIGNAL_EXPORTS signal_sink_base
    {
        std::shared_ptr<connection> _connection;
    public:
        virtual void set_connection(std::shared_ptr<connection> connection);
    };
}