#pragma once

namespace Signals
{
    class ISignalLogSink
    {
        std::shared_ptr<connection> _connection;
    public:
        virtual void set_connection(std::shared_ptr<connection> connection) { _connection = connection; }
    };
}