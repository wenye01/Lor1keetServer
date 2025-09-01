#include "http_server.h"

#include "asio.hpp"
#include <iostream>
#include <memory>

// 临时处理客户端请求
void func(asio::ip::tcp::socket socket)
{
    asio::streambuf request_buf;
    asio::read_until(socket, request_buf, "\r\n\r\n");

    std::istream request_stream(&request_buf);
    std::string headers;
    std::getline(request_stream, headers, '\0');

    std::cout << "Received request headers:\n" << headers << std::endl;


    std::string response =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/plain\r\n"
        "Content-Length: 13\r\n"
        "Connection: close\r\n"
        "\r\n"
        "Hello World!";

    asio::write(socket, asio::buffer(response));
    std::cout << "Response sent to client." << std::endl;
}

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
        acceptor_.async_accept(
            [this](std::error_code ec, asio::ip::tcp::socket socket)
            {
                if(!ec)
                {
                    func(std::move(socket));
                }
                Accept();
            }
        );
    }
}