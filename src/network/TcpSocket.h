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
#include <boost/asio/error.hpp>

#include <string>

#include "Socket.h"

typedef boost::asio::ip::tcp::socket   boost_tcp_socket;

/**
 * \class TcpSocket
 * \brief Representation of tcp socket system
 */
class TcpSocket : public Socket
{
public:
   /**
    * \fn TcpSocket(boost_tcp_socket* boostSocket)
	* \brief Constructor new socket with boost socket
	* \param boostSocket
	*/
   TcpSocket(boost_tcp_socket* boostSocket);

   /**
    * \fn TcpSocket(boost_tcp_socket* boostSocket)
	* \brief Constructor
	* \param boostSocket
	*/
   virtual ~TcpSocket(void);


   virtual void read(std::vector<char>& message, boost_read_callback callback);
   virtual void write(const std::string& message, boost_write_callback callback);
   virtual void close();
private:	
   void  handleRead(boost_read_callback, const boost::system::error_code&, size_t);
   void  handleWrite(boost_write_callback, const boost::system::error_code&);

   boost_tcp_socket*              m_socket; /*!< boost socket */
};