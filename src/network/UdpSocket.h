#pragma once

#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/function.hpp>

#include "ConnectionError.h"

class UdpSocket
{
public:
   typedef  boost::asio::ip::udp::socket  boost_udp_socket;
   typedef  boost::asio::ip::udp::endpoint boost_udp_endpoint;

   typedef boost::function<void(std::size_t, boost_udp_endpoint, const ConnectionError&)> boost_udp_receive_callback;

	UdpSocket(boost_udp_socket* socket);
   ~UdpSocket(void);

   void	read(std::vector<char>& message, boost_udp_receive_callback callback);
   void close(void);
				
private:
   void  waitReceive();
   void  handleReceive(boost_udp_receive_callback callback, std::size_t bytesRead, const boost::system::error_code& error);

   boost_udp_socket*    m_socket;
   boost_udp_endpoint   m_endpoint;
};
