#include "TcpSocket.h"

#include "ConnectionError.h"

TcpSocket::TcpSocket(boost_tcp_socket* boostSocket)
   : m_socket(boostSocket)
{
}

TcpSocket::~TcpSocket(void)
{
	delete m_socket;
}

void TcpSocket::read(std::vector<char>& message, boost_read_callback callback)
{
   boost::asio::async_read(*m_socket, boost::asio::buffer(message),
      boost::bind(&TcpSocket::handleRead, this, 
         callback, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
}

#include <iostream>
void  TcpSocket::handleRead(boost_read_callback callback, const boost::system::error_code& error, size_t bytesRead)
{
   if (error)
   {
      callback(ConnectionFailure, bytesRead);
	  std::cout << error.value() << " - " << error.message() << std::endl;
   }
   else
      callback(ConnectionSuccess, bytesRead);

}

void TcpSocket::write(const std::string& message, boost_write_callback callback)
{
   boost::asio::async_write(*m_socket, boost::asio::buffer(message),
      boost::bind(&TcpSocket::handleWrite, this,
         callback, boost::asio::placeholders::error));
}

void  TcpSocket::handleWrite(boost_write_callback callback, const boost::system::error_code& error)
{
   if (error)
   {
      callback(ConnectionFailure);
	  std::cout << error.value() << " - " << error.message() << std::endl;
   }
   else
      callback(ConnectionSuccess);
}

void TcpSocket::close()
{
   m_socket->close();
}