#pragma once

/**
 * \file Session.h
 * \brief Declaration of Session
 * \author Nicolas Laille
 * \version 1.0
 * \date 24 Octobre 2013
 */
#include <boost/thread.hpp>

#include "network/IConnection.h"
#include "ISession.h"

/**
 * \class Session
 * \brief Representation of a client/mobile application
 */
class Session : public ISession
{
public:
   /**
    * \fn Session(IConnection::connection connection, const std::string& id)
	* \brief Constructor, a session is identified by a connection and an id
	* \param connection
	* \param id
	*/
   explicit Session(IConnection::connection connection, const std::string& id);

   /**
    * \fn ~Session(void)
	* \brief Destructor
	*/
   virtual ~Session(void);

   /**
    * \fn bool is(SessionType type) const
	* \brief Check session type
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
   virtual bool valid(const std::string& id) const;

   /**
    * \fn void write(const std::string& message)
	* \brief Write message to the session
	* \param message
	*/
   virtual void write(const std::string& message);

   /**
    * \fn void sendTo(Packet& packet)
	* \brief Send packet to the session
	* \param packet
	*/
   virtual void sendTo(Packet& packet);

   /**
    * \fn void sendFromTo(ISession::session& session, Packet& packet)
	* \brief Send packet from this session to the session
	* \param session
	* \param packet
	*/
   virtual void sendFromTo(ISession::session& session, Packet& packet);

   /**
    * \fn IConnection::connection	getConnection() const
	* \brief Get connection
	* \return connection
	*/
   virtual IConnection::connection	getConnection() const;

   /**
    * \fn void lock()
	* \brief Lock session mutex
	*/
   virtual void lock();

   /**
    * \fn void unlock()
	* \brief Unlock session mutex
	*/
   virtual void unlock();

   /**
    * \fn void close()
	* \brief Close the session
	*/
   virtual void close();
private:
   IConnection::connection             m_connection; /*!< connection */
   std::string                         m_id; /*!< id */
   mutable boost::mutex                m_mutex; /*!< mutex */
};

