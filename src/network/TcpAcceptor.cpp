#include "TcpAcceptor.h"

#include "TcpSocket.h"

#include "protocol/ProtocolQueue.h"
#include "protocol/PacketFactory.h"
#include "Connection.h"

TcpAcceptor::TcpAcceptor(boost::asio::io_service& ioService, int port)
	: m_acceptor(ioService, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port))
{
}

TcpAcceptor::~TcpAcceptor(void)
{
}

void     TcpAcceptor::start(void)
{
   waitAccept();
}

void     TcpAcceptor::stop(void)
{
}

void  TcpAcceptor::waitAccept()
{
   boost_tcp_socket* boostSocket = new boost_tcp_socket(m_acceptor.get_io_service());

   this->m_acceptor.async_accept(*boostSocket,
      boost::bind(&TcpAcceptor::handleAccept, this,
         boostSocket, boost::asio::placeholders::error));
}

void  TcpAcceptor::handleAccept(boost_tcp_socket* boostSocket, const boost::system::error_code& error)
{
   if (!error)
   {
      Packet   packet = PacketFactory::create(Protocol::Opcode::SERVER_HANDLER, Protocol::Opcode::Server::CONNECT);

      ProtocolQueue::instance().push(std::make_pair(Connection::create(new TcpSocket(boostSocket), 
		  boostSocket->remote_endpoint().address().to_string()), 
		  packet));
      waitAccept();
   }
}