#ifndef __BLOCKING_TCP_ECHO_SERVER_H__
#define __BLOCKING_TCP_ECHO_SERVER_H__

#include <boost/asio.hpp>

using boost::asio::ip::tcp;

void session(tcp::socket sock);
void server(boost::asio::io_service& io_service, unsigned short port);

#endif  /* __BLOCKING_TCP_ECHO_SERVER_H__ */