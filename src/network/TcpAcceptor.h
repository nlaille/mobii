#pragma once

/**
 * \file TcpAcceptor.h
 * \brief Declaration of TcpAcceptor
 * \author Nicolas Laille
 * \version 1.0
 * \date 24 Octobre 2013
 */
#include <string>

#include "Acceptor.h"

#include "TcpSocket.h"

/**
 * \class TcpAcceptor
 * \brief Acceptor for tcp connection
 */
class TcpAcceptor : public Acceptor
{
public:
	TcpAcceptor(boost::asio::io_service& ioService, int port);

   virtual ~TcpAcceptor(void);

   virtual void start(void);
   virtual void stop(void);

protected:
   void  waitAccept();
   virtual void  handleAccept(boost_tcp_socket* boostSocket, const boost::system::error_code& error);

   boost::asio::ip::tcp::acceptor    m_acceptor;
};
