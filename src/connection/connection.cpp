#include "connection.h"

#include <iostream>

#include "../parser/http_parser.h"

namespace netFrame
{
    // 临时处理客户端请求
    void func(asio::ip::tcp::socket* socket)
    {
        std::string response =
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/plain\r\n"
            "Content-Length: 13\r\n"
            "Connection: keep-alive\r\n"
            "\r\n"
            "Hello World!";

        asio::write(*socket, asio::buffer(response));

        std::cout << "Response sent to client." << std::endl;
    }

    Connection::Connection(asio::io_context& io)
        : socket_(std::unique_ptr<asio::ip::tcp::socket>(new asio::ip::tcp::socket(io)))
    {
        buffers_.resize(1024);
    }
    Connection::~Connection()
    {
        
    }
    void Connection::Start()
    {
        auto handler = this->shared_from_this();
        socket_->async_read_some(asio::buffer(buffers_),
            [this, handler](std::error_code const& ec, std::size_t read_size)
            {
                if (ec)
                {
                    std::cout << "Read error: ";
                    std::cout << ec.message() << std::endl;
                    return;
                }
                // 解析HTTP请求头
                auto res = http_parser::parser(std::string_view(&buffers_[0], read_size), meta_);

                if (res.first == parse_state::valid)
                {
                    if (res.second)
                    {
                        func(socket_.get());
                    }
                    else
                    {
                        //this->Start();
                    }
                }
                else
                {
                    // 关闭连接
                    // 日志
                    this->Disconnect();
                }
            });
    }

    void Connection::Disconnect()
    {
        std::error_code ignore_write_ec;
        socket_->write_some(asio::buffer("\0\0"), ignore_write_ec);  //solve time_wait problem
        std::error_code ignore_shutdown_ec;
        socket_->shutdown(asio::ip::tcp::socket::shutdown_both, ignore_shutdown_ec);
        std::error_code ignore_close_ec;
        socket_->close(ignore_close_ec);
    }
}