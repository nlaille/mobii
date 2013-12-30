#pragma once

/**
 * \file ISession.h
 * \brief Declaration of IProtocolHandler
 * \author Nicolas Laille
 * \version 1.0
 * \date 24 Octobre 2013
 */

#include <string>

#include "network/IConnection.h"
#include "protocol/Packet.h"

#include <iostream>

/**
 * \class ISession
 * \brief Interface for Sessions
 */
class ISession
{
public:
	/**
	 * \enum SessionType
	 * \brief Type of Session
	 */
   enum SessionType
   {
      NOSESSION=0,
      UNREGISTERED=1,
      SMARTPHONE=10,
      USER=11,
      ADMIN=100
   };

   /**
	 * \typedef boost::shared_ptr<ISession> session
	 */
   typedef boost::shared_ptr<ISession> session;

   /**
	* \fn ~ISession()
	* \brief Destructor
	*/
   virtual ~ISession() {std::cout<<"Session deleted"<<std::endl;}

   /**
	* \fn bool is(SessionType type) const
	* \brief Check type of session
	* \param type
	* \return boolean
	*/
   virtual bool is(SessionType type) const = 0;

   /**
	* \fn bool valid(const std::string& id) const
	* \brief Check session id
	* \param id
	* \return boolean
	*/
   virtual bool valid(const std::string& id) const = 0;

   /**
	* \fn void write(const std::string& message)
	* \brief Write message to this
	* \param message
	*/
   virtual void write(const std::string& message) = 0;

   /**
	* \fn void sendTo(Packet& packet)
	* \brief Send packet to this session
	* \param packet
	*/
   virtual void sendTo(Packet& packet) = 0;

   /**
	* \fn void sendFromTo(ISession::session& session, Packet& packet)
	* \brief Send packet from this to session
	* \param session
	* \param packet
	*/
   virtual void sendFromTo(ISession::session& session, Packet& packet) = 0;

   /**
	* \fn IConnection::connection	getConnection() const
	* \brief Get IConnection associated to this
	* \return connection
	*/
   virtual IConnection::connection	getConnection() const = 0;

   /**
	* \fn void lock()
	* \brief Lock this to prevent concurrential access
	*/
   virtual void lock() = 0;

   /**
	* \fn void unlock()
	* \brief Unlock this to prevent concurrential access
	*/
   virtual void unlock() = 0;

   /**
	* \fn void close()
	* \brief Close this
	*/
   virtual void close() = 0;
};
