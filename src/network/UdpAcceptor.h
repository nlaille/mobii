#pragma once

#include <string>

#include "Acceptor.h"

#include "UdpSocket.h"

class UdpAcceptor : public Acceptor
{
public:
	UdpAcceptor(boost::asio::io_service& ioService, int port);

   virtual ~UdpAcceptor(void);

   virtual void start(void);
   virtual void stop(void);

private:
	void	waitRead();
	void	handleRead(std::size_t bytesRead, UdpSocket::boost_udp_endpoint endpoint, const ConnectionError& error);

   UdpSocket			m_udpSocket;
   std::vector<char>	m_read;
};
