#include "UdpAcceptor.h"

#include "protocol/ProtocolQueue.h"
#include "protocol/Protocol.h"

#include "UdpConnection.h"

UdpAcceptor::UdpAcceptor(boost::asio::io_service& ioService, int port)
	: m_udpSocket(new UdpSocket::boost_udp_socket(ioService, UdpSocket::boost_udp_endpoint(boost::asio::ip::udp::v4(), port))),
	m_read(200)
{
}

UdpAcceptor::~UdpAcceptor(void)
{
}

void     UdpAcceptor::start(void)
{
   waitRead();
}

void     UdpAcceptor::stop(void)
{
}

void	 UdpAcceptor::waitRead()
{
	m_read.empty();
	m_udpSocket.read(m_read, boost::bind(&UdpAcceptor::handleRead, this, _1, _2, _3));
}

void	 UdpAcceptor::handleRead(std::size_t bytesRead, UdpSocket::boost_udp_endpoint endpoint, const ConnectionError& error)
{
	std::cerr << "UDP Data from : " << endpoint.address().to_string() << ":" << endpoint.port() << std::endl;
	if (!error)
	{
		Packet		packet;
		std::string data;

		packet.setHeader(m_read);
		
		std::copy(m_read.begin() + Protocol::Header::SIZE, m_read.begin() + bytesRead, std::back_inserter(data));
        packet.setMessage(data);
		std::cerr << "UDP Packet : " << std::endl << packet;
		ProtocolQueue::instance().push(std::make_pair(UdpConnection::create(&m_udpSocket, endpoint), packet));
	}
	waitRead();
}