#pragma once

#include <thread>
#include <string>
#include <memory>

#include "asio.hpp"

namespace netFrame 
{
    class HttpServer
    {
    public:
        HttpServer(std::size_t thread_size);

        bool Listen(const std::string& ip, const std::string& port);

        void Server();

        void Run();

    private:

        void Accept();

    private:
        asio::io_context io_context_;
        asio::ip::tcp::acceptor acceptor_;

    };
}