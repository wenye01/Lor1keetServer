#include "connection.h"

namespace netFrame
{
    Connection::Connection()
        : socket_(nullptr)
    {
    }
    Connection::~Connection()
    {
        
    }
    void Connection::Start()
    {
        //
        auto handler = this->shared_from_this();
        socket_->async_read_some(asio::buffer(&buffers_[0], 1024),
            [handler, this](std::error_code const& ec, std::size_t read_size)
            {
                if (ec)
                {
                    return;
                }
                // 解析HTTP请求头
            });

    }
    void Connection::Stop()
    {

    }
}