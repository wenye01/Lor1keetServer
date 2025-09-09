#include "http_server.h"

#include "asio.hpp"
#include <iostream>
#include <memory>

#include "../connection/connection.h"

namespace netFrame
{
    HttpServer::HttpServer(std::size_t thread_size)
        : io_context_(), acceptor_(io_context_)
    {

    }

    // 开始监听端口
    bool HttpServer::Listen(const std::string& ip, const std::string& port)
    {
        asio::ip::tcp::resolver resolver(io_context_);
        auto endpoint = resolver.resolve(ip, port);

        for(auto& ep : endpoint)
        {
            acceptor_.open(ep.endpoint().protocol());
            acceptor_.set_option(asio::ip::tcp::acceptor::reuse_address(true));

            acceptor_.bind(ep);
            acceptor_.listen();

            Accept();
            return true;
        }

        return false;
    }

    // 启动服务器，io_context_开始处理异步事件
    void HttpServer::Run()
    {
        std::vector<std::thread> threads;
        for (size_t i = 0; i < std::thread::hardware_concurrency(); ++i) {
            threads.emplace_back([this]() {
                io_context_.run();
                });
        }

        for (auto& thread : threads) {
            thread.join();
        }
    }

    // 异步处理连接
    void HttpServer::Accept()
    {
        auto new_connection = std::make_shared<Connection>(io_context_);
        acceptor_.async_accept(new_connection->GetSocket(),
            [this, new_connection](std::error_code ec)
            {
                if(!ec)
                {
                    new_connection->Start();
                }
                else
                {
                    std::cout << "Accept error: ";
                    std::cout << ec.message()<<std::endl;
                }
                Accept();
            }
        );
    }
}