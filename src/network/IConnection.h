#pragma once

/**
 * \file IConnection.h
 * \brief Declaration of IConnection
 * \author Nicolas Laille
 * \version 1.0
 * \date 24 Octobre 2013
 */

#include <boost/asio.hpp>
#include <boost/enable_shared_from_this.hpp>


#include <iostream>

/**
 * \class IConnection
 * \brief Interface for Connection
 */
class IConnection
{
public:
   typedef boost::shared_ptr<IConnection> connection;

   virtual ~IConnection() {std::cout<<"Connection deleted"<<std::endl;}

   virtual void write(const std::string& message) = 0;
   virtual void close() = 0;

   virtual const std::string getRemoteIP() const = 0;
   virtual const std::string getIP() const = 0;
   virtual const unsigned int getPort() const = 0;
   virtual void	setPort(unsigned int port) = 0;
   virtual void	setIP(const std::string& ip) = 0;

   virtual bool isSameNetwork(IConnection::connection connection) const = 0;	
};