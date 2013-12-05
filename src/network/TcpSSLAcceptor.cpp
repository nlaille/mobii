#include "TcpSSLAcceptor.h"

#include "TcpSSLSocket.h"
#include "ConnectionSSL.h"

#include "protocol/ProtocolQueue.h"
#include "protocol/PacketFactory.h"

TcpSSLAcceptor::TcpSSLAcceptor(boost::asio::io_service& ioService, int port)
	: m_acceptor(ioService, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)), m_context(ioService, boost::asio::ssl::context::sslv23)
{
   m_context.set_options(boost::asio::ssl::context::default_workarounds
                           | boost::asio::ssl::context::no_sslv2
                           | boost::asio::ssl::context::single_dh_use);
   m_context.use_certificate_chain_file("server.crt");
   m_context.use_private_key_file("server.key", boost::asio::ssl::context::pem);
   m_context.use_tmp_dh_file("dh512.pem");
}

TcpSSLAcceptor::~TcpSSLAcceptor(void)
{
}

void  TcpSSLAcceptor::start()
{
	waitAccept();
}

void  TcpSSLAcceptor::stop()
{
}

void  TcpSSLAcceptor::waitAccept()
{
   boost_ssl_tcp_socket* boostSSLSocket = new boost_ssl_tcp_socket(m_acceptor.get_io_service(), m_context);

   this->m_acceptor.async_accept(boostSSLSocket->lowest_layer(),
      boost::bind(&TcpSSLAcceptor::handleAccept, this,
      boostSSLSocket, boost::asio::placeholders::error));
}

void  TcpSSLAcceptor::handleAccept(boost_ssl_tcp_socket* boostSSLSocket, const boost::system::error_code& error)
{
   if (!error)
   {
      Packet   packet = PacketFactory::create(Protocol::Opcode::SERVER_HANDLER, Protocol::Opcode::Server::CONNECT);

	  ProtocolQueue::instance().push(std::make_pair(ConnectionSSL::create(new TcpSSLSocket(boostSSLSocket), 
		  boostSSLSocket->lowest_layer().remote_endpoint().address().to_string()),
		  packet));
      waitAccept();
   }
}
