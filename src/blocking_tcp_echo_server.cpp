//
// blocking_tcp_echo_server.cpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2014 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <cstdlib>
#include <iostream>
#include <thread>
#include <utility>
#include <boost/asio.hpp>

#include "blocking_tcp_echo_server.h"

const int max_length = 1024;

void session(tcp::socket sock)
{
    try {
        for (;;) {
            char data[max_length];
            memset(data, 0, max_length);

            boost::system::error_code error;
            size_t length = sock.read_some(boost::asio::buffer(data), error);
            if (error == boost::asio::error::eof) {
                break; // Connection closed cleanly by peer.
            }
            else if (error) {
                throw boost::system::system_error(error); // Some other error.
            }

            boost::asio::write(sock, boost::asio::buffer(data, length));
            std::cout << "[" << __FILE__ << ":" << __LINE__ << "]" << " received data(" << length << "): " << data << std::endl;
        }
    }
    catch (std::exception& e) {
        std::cerr << "Exception in thread: " << e.what() << "\n";
    }
}

void server(boost::asio::io_service& io_service, unsigned short port)
{
    tcp::acceptor a(io_service, tcp::endpoint(tcp::v4(), port));
    for (;;) {
        tcp::socket sock(io_service);
        a.accept(sock);
        std::thread(session, std::move(sock)).detach();
    }
}
