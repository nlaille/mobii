#pragma once

/**
 * \file SessionManager.h
 * \brief Declaration of SessionManager
 * \author Nicolas Laille
 * \version 1.0
 * \date 24 Octobre 2013
 */
#include <boost/thread.hpp>
#include <boost/bimap.hpp>
#include <map>

#include "ISession.h"

/**
 * \class SessionManager
 * \brief Stores sessions and sessions's id and sessions's connector
 */
class SessionManager
{
public:
   /**
    * \typedef boost::bimap<std::string, ISession::session>                 idsMap
	*/
   typedef boost::bimap<std::string, ISession::session>                 idsMap;

   /**
    * \typedef boost::bimap<IConnection::connection, ISession::session>     connectionsMap
	*/
   typedef boost::bimap<IConnection::connection, ISession::session>     connectionsMap;

   /**
    * \fn SessionManager(void)
	* \brief Constructor
	*/
   explicit SessionManager(void);

   /**
    * \fn ~SessionManager(void)
	* \brief Destructor
	*/
   virtual ~SessionManager(void);

   /**
    * \fn void        add(const std::string& id, ISession::session session)
	* \brief Add a session with id
	* \param id
	* \param session
	*/
   virtual void        add(const std::string& id, ISession::session session);

   /**
    * \fn ISession::session   get(const std::string& id) const
	* \brief Get session from id
	* \param id
	* \return session
	*/
   virtual ISession::session   get(const std::string& id) const;

   /**
    * \fn bool        exists(const std::string& id) const
	* \brief Check if session's id exists
	* \param id
	* \return boolean
	*/
   virtual bool        exists(const std::string& id) const;

   /**
    * \fn void        add(IConnection::connection& connection, ISession::session session)
	* \brief Add a session with connection
	* \param connection
	* \param session
	*/
   virtual void        add(IConnection::connection& connection, ISession::session session);

   /**
    * \fn ISession::session   get(IConnection::connection& connection) const
	* \brief Get session from connection
	* \param connection
	* \return session
	*/
   virtual ISession::session   get(IConnection::connection& connection) const;

   /**
    * \fn bool        exists(IConnection::connection& connection) const
	* \brief Check if session's connection exists
	* \param connection
	* \return boolean
	*/
   virtual bool        exists(IConnection::connection& connection) const;

   /**
    * \fn bool        valid(IConnection::connection& connection, const std::string& id) const
	* \brief Check connection's id, uses connectionSessions to get session associated to the connection then check session's id
	* \param connection
	* \param is
	* \return boolean
	*/
   virtual bool        valid(IConnection::connection& connection, const std::string& id) const;

   /**
    * \fn void        remove(ISession::session session)
	* \brief Remove session
	*/
   virtual void        remove(ISession::session session);

private:
   idsMap                  m_idSessions; /*!< store session with id */
   connectionsMap          m_connectionSessions; /*!< store session with connection */
   mutable boost::mutex    m_mutex; /*!< mutex */
};