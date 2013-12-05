#include "UdpConnection.h"

UdpConnection::UdpConnection(UdpSocket* socket, UdpSocket::boost_udp_endpoint endpoint)
	: m_socket(socket), m_endpoint(endpoint)
{
}

UdpConnection::~UdpConnection(void)
{
}

IConnection::connection	UdpConnection::create(UdpSocket* socket, UdpSocket::boost_udp_endpoint endpoint)
{
	return IConnection::connection(new UdpConnection(socket, endpoint));
}

void UdpConnection::write(const std::string& message)
{
}

void UdpConnection::close()
{
}

const std::string  UdpConnection::getRemoteIP() const
{
   return m_endpoint.address().to_string();
}

const std::string  UdpConnection::getIP() const
{
   return m_endpoint.address().to_string();
}

const unsigned int UdpConnection::getPort() const
{
   return m_endpoint.port();
}

bool UdpConnection::isSameNetwork(IConnection::connection connection) const
{
	return getRemoteIP() == connection->getRemoteIP();
}

void	UdpConnection::setPort(unsigned int port)
{
	throw std::runtime_error("[UdpConnection::setPort]");
}

void	UdpConnection::setIP(const std::string& ip)
{
	throw std::runtime_error("[UdpConnection::setIP]");
}