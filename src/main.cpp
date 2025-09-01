#include <string>
#include <iostream>


#include "asio.hpp"
#include "server/http_server.h"

int main()
{
    netFrame::HttpServer server(4);

    server.Listen("127.0.0.1", "8080");

    server.Run();

    return 0;
}