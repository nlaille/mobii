#include "TcpSSLSocket.h"

TcpSSLSocket::TcpSSLSocket(boost_ssl_tcp_socket* boostSSLSocket)
   : m_socket(boostSSLSocket)
{
}

TcpSSLSocket::~TcpSSLSocket(void)
{
	delete m_socket;
}
   
void TcpSSLSocket::handshake(boost_handshake_callback callback)
{
   m_socket->async_handshake(boost::asio::ssl::stream_base::server,
      boost::bind(&TcpSSLSocket::handleHandshake, this, 
         callback, boost::asio::placeholders::error));
}

void  TcpSSLSocket::handleHandshake(boost_handshake_callback callback, const boost::system::error_code& error)
{
   if (error)
      callback(ConnectionFailure);
   else
      callback(ConnectionSuccess);
}

void TcpSSLSocket::read(std::vector<char>& message, boost_read_callback callback)
{
   boost::asio::async_read(*m_socket, boost::asio::buffer(message),
      boost::bind(&TcpSSLSocket::handleRead, this,
         callback, boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred));
}

void  TcpSSLSocket::handleRead(boost_read_callback callback, const boost::system::error_code& error, size_t bytesRead)
{
   if (error)
      callback(ConnectionFailure, bytesRead);
   else
      callback(ConnectionSuccess, bytesRead);
}

void TcpSSLSocket::write(const std::string& message, boost_write_callback callback)
{
   boost::asio::async_write(*m_socket, boost::asio::buffer(message),
      boost::bind(&TcpSSLSocket::handleWrite, this,
         callback, boost::asio::placeholders::error));
}

void  TcpSSLSocket::handleWrite(boost_write_callback callback, const boost::system::error_code& error)
{
   if (error)
      callback(ConnectionFailure);
   else
      callback(ConnectionSuccess);
}

void TcpSSLSocket::close()
{
   m_socket->lowest_layer().close();
}
