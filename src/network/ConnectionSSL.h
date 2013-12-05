#pragma once

/**
 * \file ConnectionSSL.h
 * \brief Declaration of ConnectionSSL
 * \author Nicolas Laille
 * \version 1.0
 * \date 24 Octobre 2013
 */

#include "Connection.h"
#include "TcpSSLSocket.h"

/**
 * \class ConnectionSSL
 * \brief Representation of Socket using SSL
 */
class ConnectionSSL : public Connection
{
public:
   virtual ~ConnectionSSL();

   /**
	* \fn static IConnection::connection create(Socket* socket, const std::string& remoteIp)
	* \brief Create new connection using SSL, start connection to wait for data
	* \param socket 
	* \param remoteIp
	* \param privateIp
	*/
   static IConnection::connection create(TcpSSLSocket* socket, const std::string& remoteIp);
private:
   ConnectionSSL(TcpSSLSocket* socket, const std::string& remoteIp);

   void  handleHandshake(const ConnectionError& error);
};