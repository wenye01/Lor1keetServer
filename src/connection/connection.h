#pragma once

#include <memory>

#include "asio.hpp"

namespace netFrame
{
    class Connection : public std::enable_shared_from_this<Connection>
    {
    public:
        Connection();
        ~Connection();
    public:
        void Start();
    
    private:
        void Stop();

    private:
        std::unique_ptr<asio::ip::tcp::socket> socket_;
        std::vector<char> buffers_;
    };
}