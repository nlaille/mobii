#pragma once

/**
 * \file TcpSocket.h
 * \brief Declaration of TcpSocket
 * \author Nicolas Laille
 * \version 1.0
 * \date 24 Octobre 2013
 */
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <boost/asio/ssl.hpp>

#include <string>

#include "Socket.h"

typedef boost::asio::ssl::stream<boost::asio::ip::tcp::socket> boost_ssl_tcp_socket;

/**
 * \class TcpSocket
 * \brief Representation of tcp socket system using SSL
 */
class TcpSSLSocket : public Socket
{
public:
   typedef boost::function<void(const ConnectionError&)> boost_handshake_callback;

   /**
    * \fn TcpSSLSocket(boost_ssl_tcp_socket* boostSSLSocket)
	* \brief Create new boost ssl socket
	* \param boostSSLSocket
	*/
   TcpSSLSocket(boost_ssl_tcp_socket* boostSSLSocket);

   /**
    * \fn ~TcpSSLSocket(void)
	* \brief Destructor
	*/
   virtual ~TcpSSLSocket(void);

   virtual void read(std::vector<char>& message, boost_read_callback callback);
   virtual void write(const std::string& message, boost_write_callback callback);
   virtual void close();

   /**
    * \fn void  handshake(boost_handshake_callback callback)
	* \brief Handshake to finalize ssl connection
	* \param callback
	*/
   void  handshake(boost_handshake_callback callback);
private:	
   void  handleRead(boost_read_callback, const boost::system::error_code&, size_t);
   void  handleWrite(boost_write_callback, const boost::system::error_code&);
   void  handleHandshake(boost_handshake_callback, const boost::system::error_code&);

   boost_ssl_tcp_socket*          m_socket; /*!< boost tcp ssl socket */
};