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
        asio::io_context io_context_; // IO上下文对象，用于管理异步操作的生命周期，自身即支持多线程
        asio::ip::tcp::acceptor acceptor_;

    };
}