#pragma once

/**
 * \file TcpSSLAcceptor.h
 * \brief Declaration of TcpSSLAcceptor
 * \author Nicolas Laille
 * \version 1.0
 * \date 24 Octobre 2013
 */
#include "Acceptor.h"
#include "TcpSSLSocket.h"

/**
 * \class TcpSSLAcceptor
 * \brief Acceptor for tcp connection using SSL
 */
class TcpSSLAcceptor : public Acceptor
{
public:
   /**
    * \fn TcpSSLAcceptor(boost::asio::io_service& ioService, int port)
	* \brief Create new boost acceptor which will listen on port
	* \param ioService
	* \param port
	*/
   TcpSSLAcceptor(boost::asio::io_service& ioService, int port);

   /**
    * \fn ~TcpSSLAcceptor()
	* \brief Destructor
	*/
   virtual ~TcpSSLAcceptor();
				
   virtual void   start();
   virtual void   stop();

private:
   void  waitAccept();
   void  handleAccept(boost_ssl_tcp_socket* boostSSLSocket, const boost::system::error_code& error);

   boost::asio::ip::tcp::acceptor    m_acceptor; /*!< boost acceptor */
   boost::asio::ssl::context         m_context; /*!< SSL Context */
};
