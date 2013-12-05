#include "UdpSocket.h"

#include "protocol/ProtocolQueue.h"

UdpSocket::UdpSocket(boost_udp_socket* socket)
	: m_socket(socket)
{
}

UdpSocket::~UdpSocket(void)
{
	delete m_socket;
}

void     UdpSocket::close(void)
{
	m_socket->close();
}

void     UdpSocket::read(std::vector<char>& message, boost_udp_receive_callback callback)
{
	m_socket->async_receive_from(
		boost::asio::buffer(message), m_endpoint,
		boost::bind(&UdpSocket::handleReceive, this,
		callback, boost::asio::placeholders::bytes_transferred,
		boost::asio::placeholders::error));
}

void     UdpSocket::handleReceive(boost_udp_receive_callback callback, std::size_t bytesRead, const boost::system::error_code& error)
{
   if (!error)
   {
      std::cout << "Adresse : " << m_endpoint.address() << std::endl;
      std::cout << "Port : " << m_endpoint.port() << std::endl;
	  callback(bytesRead, m_endpoint, ConnectionSuccess);
   }
   else
   {
      callback(bytesRead, boost_udp_endpoint(), ConnectionFailure);
   }
}
