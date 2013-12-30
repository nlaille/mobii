#pragma once

/**
 * \file Socket.h
 * \brief Declaration of Socket
 * \author Nicolas Laille
 * \version 1.0
 * \date 24 Octobre 2013
 */
#include <boost/bind.hpp>
#include <boost/function.hpp>

#include "ConnectionError.h"

#include <vector>

/**
 * \class Socket
 * \brief Encapsulsation of socket system
 */
class Socket
{
public:
   /**
    * \typedef boost::function<void(const ConnectionError&, size_t)> boost_read_callback
	*/
   typedef boost::function<void(const ConnectionError&, size_t)> boost_read_callback;

   /**
    * \typedef boost::function<void(const ConnectionError&)> boost_write_callback
	*/
   typedef boost::function<void(const ConnectionError&)> boost_write_callback;

   /**
    * \fn ~Socket()
	* \brief Destructor
	*/
   virtual ~Socket() {}

   /**
    * \fn void read(std::vector<char>& message, boost_read_callback callback)
	* \brief Read data from the socket, data are put into message, then callback is called
	* \param message
	* \param callback
	*/
   virtual void read(std::vector<char>& message, boost_read_callback callback) = 0;

   /**
    * \fn void write(const std::string& message, boost_write_callback callback)
	* \brief Write message to the socket then callback is called
	* \param message
	* \param callback
	*/
   virtual void write(const std::string& message, boost_write_callback callback) = 0;

   /**
    * \fn void close()
	* \brief Close the socket
	*/
   virtual void close() = 0;
};