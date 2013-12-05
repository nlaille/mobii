#include "ConnectionSSL.h"

IConnection::connection ConnectionSSL::create(TcpSSLSocket* socket, const std::string& remoteIp)
{
   return IConnection::connection(new ConnectionSSL(socket, remoteIp));
}

ConnectionSSL::ConnectionSSL(TcpSSLSocket* socket, const std::string& remoteIp)
   : Connection(socket, remoteIp)
{
   socket->handshake(boost::bind(&ConnectionSSL::handleHandshake, this, _1));
}

ConnectionSSL::~ConnectionSSL()
{
}

void  ConnectionSSL::handleHandshake(const ConnectionError& error)
{
   if (!error)
      Connection::waitForData();
   else
      close();
}