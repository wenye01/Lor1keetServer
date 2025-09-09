#pragma once

#include <memory>

#include "asio.hpp"
#include "../parser/request_meta.h"
namespace netFrame
{
    class Connection : public std::enable_shared_from_this<Connection>
    {
    public:
        Connection(asio::io_context& io);
        ~Connection();
    public:
        void Start();
    
    private:
        void Stop();

        void Disconnect();

    public:
        asio::ip::tcp::socket& GetSocket()
        {
            return *socket_;
        }

    private:
        std::unique_ptr<asio::ip::tcp::socket> socket_;
        std::vector<char> buffers_;
        request_meta meta_;
    };
}