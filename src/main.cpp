#include <cstdlib>
#include <iostream>
#include <thread>
#include <utility>

#include "blocking_tcp_echo_server.h"

int main(int argc, char* argv[])
{
    try {
        if (argc != 2) {
            std::cerr << "Usage: blocking_tcp_echo_server <port>\n";
            return 1;
        }

        boost::asio::io_service io_service;

        server(io_service, std::atoi(argv[1]));
    }
    catch (std::exception &e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}