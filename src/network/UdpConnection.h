#pragma once

#include <boost/bind.hpp>
#include <boost/asio.hpp>

#include "UdpSocket.h"
#include "IConnection.h"

class UdpConnection : public boost::enable_shared_from_this<UdpConnection>, public IConnection
{
public:

   static IConnection::connection create(UdpSocket* socket, UdpSocket::boost_udp_endpoint endpoint);


   virtual ~UdpConnection(void);


   virtual void write(const std::string& message);
   virtual void close();

   virtual const std::string getRemoteIP() const;
   virtual const std::string getIP() const;
   virtual const unsigned int getPort() const;
   virtual void	setPort(unsigned int port);
   virtual void	setIP(const std::string& ip);

   virtual bool isSameNetwork(IConnection::connection connection) const;
				
private:
   explicit UdpConnection(UdpSocket* socket, UdpSocket::boost_udp_endpoint endpoint);

   UdpSocket*                    m_socket;
   UdpSocket::boost_udp_endpoint m_endpoint;
};
