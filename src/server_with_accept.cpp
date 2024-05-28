#include "stdafx.h"


#include <ctime>
#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <ctime>
#include <iostream>
#include <string>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>

#include <boost/date_time/posix_time/posix_time.hpp>

#define _BUFF_SIZE 128 

using boost::asio::ip::tcp;

class CConnection : public boost::enable_shared_from_this<CConnection>
{
private:
	tcp::socket m_Socket;
	int m_nID;

	std::string m_sMsg;
	boost::array<BYTE, _BUFF_SIZE> m_RecvBuf;

	CConnection(boost::asio::io_service& io) : m_Socket(io), m_nID(-1)
	{
	}

	// 날짜시간 메세지를 만든다.
	std::string make_daytime_string()
	{
		time_t now = time(0);
		return ctime(&now); // ctime_s 권장. 예제니까 그냥 쓰고 넘어가지만
	}

	void handle_Accept(const boost::system::error_code& /*error*/, size_t /*bytes_transferred*/)
	{
		// Recv 대기
		m_Socket.async_read_some(boost::asio::buffer(m_RecvBuf),
			boost::bind(&CConnection::handle_Read, shared_from_this(), 
			boost::asio::placeholders::error, 
			boost::asio::placeholders::bytes_transferred));
	}

	void handle_Write(const boost::system::error_code& error, size_t /*bytes_transferred*/)
	{
	}

	void handle_Read(const boost::system::error_code& error, size_t /*bytes_transferred*/)
	{
		if(!error)  // 0 이 성공 나머지는 오류 플러그 
		{
			// 데이터 처리
			if(m_RecvBuf.size())
			{
				std::cout << "Recv : " << (*(char*)(m_RecvBuf.data())) << std::endl; 
				m_RecvBuf.assign(NULL); // 버퍼 초기화 
			}

			m_Socket.async_read_some(boost::asio::buffer(m_RecvBuf),
				boost::bind(&CConnection::handle_Read, shared_from_this(),
				boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred));
		}
		else
			std::cout << m_nID << " Disconnect(Write) : " << error.message() << std::endl;
	}

public:
	typedef boost::shared_ptr<CConnection> pointer; 

	static pointer create(boost::asio::io_service& io) 
	{ 
		return pointer(new CConnection(io)); 
	} 

	tcp::socket& socket() 
	{
		return m_Socket;
	}

	void start(int nID)
	{
		std::cout << "new connect id : "<< nID << " ::: Welcome !" << std::endl;
		m_nID = nID;

		// 접속 기념으로 접속 시간 한번 보내주고
		m_sMsg = make_daytime_string();
		boost::asio::async_write(m_Socket, boost::asio::buffer(m_sMsg),
			boost::bind(&CConnection::handle_Accept, shared_from_this(),
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred));
	}
};

class CTCP_Server 
{ 
private: 
	tcp::acceptor m_Acceptor; 
	int m_nAcceptCnt; 

	void WaitAccept() 
	{ 
		++m_nAcceptCnt; 
		CConnection::pointer new_connection = 
			CConnection::create(m_Acceptor.get_io_service()); 

		m_Acceptor.async_accept(new_connection->socket(), 
			boost::bind(&CTCP_Server::handle_Accept, this, new_connection, 
			boost::asio::placeholders::error)); 
	} 

	void handle_Accept(CConnection::pointer new_connection, const boost::system::error_code& error) 
	{ 
		if (!error) 
		{ 
			new_connection->start(m_nAcceptCnt); 
			WaitAccept(); 
		} 
	} 

public: 
	CTCP_Server(boost::asio::io_service& io) : m_Acceptor(io, tcp::endpoint(tcp::v4(), 13)), m_nAcceptCnt(0) 
	{ 
		WaitAccept(); 
	} 
}; 

int _tmain() 
{ 
	try
	{ 
		boost::asio::io_service io; 

		CTCP_Server server(io); 
		io.run(); 
	} 
	catch (std::exception& e) 
	{ 
		std::cerr << e.what() << std::endl; 
	} 

	return 0; 
}