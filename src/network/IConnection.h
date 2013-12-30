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
	/**
	* \typedef boost::shared_ptr<IConnection> connection
	*/
   typedef boost::shared_ptr<IConnection> connection;

   /**
	* \fn ~IConnection()
	* \brief Destructor
	*/
   virtual ~IConnection() {std::cout<<"Connection deleted"<<std::endl;}

   /**
	* \fn void write(const std::string& message)
	* \brief Send message through the connection
	* \param message 
	*/
   virtual void write(const std::string& message) = 0;

   /**
	* \fn void close()
	* \brief Close the connection
	*/
   virtual void close() = 0;

   /**
	* \fn const std::string getRemoteIP() const
	* \brief Get Remote IP of the connection
	* \return Remote IP
	*/
   virtual const std::string getRemoteIP() const = 0;

   /**
	* \fn const std::string getIP() const
	* \brief Get local IP
	* \return Local IP
	*/
   virtual const std::string getIP() const = 0;

   /**
	* \fn const unsigned int getPort() const
	* \brief Get local port of the client application
	* \return Local port
	*/
   virtual const unsigned int getPort() const = 0;

   /**
	* \fn void	setPort(unsigned int port)
	* \brief Set local port of the client application
	* \param port 
	*/
   virtual void	setPort(unsigned int port) = 0;

   /**
	* \fn void	setIP(const std::string& ip)
	* \brief Set local IP
	* \param ip 
	*/
   virtual void	setIP(const std::string& ip) = 0;

   /**
	* \fn bool isSameNetwork(IConnection::connection connection) const
	* \brief Check if the 2 connections are on the same network
	* \param connection 
	* \return boolean
	*/
   virtual bool isSameNetwork(IConnection::connection connection) const = 0;	
};