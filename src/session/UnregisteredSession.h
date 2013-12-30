#pragma once

/**
 * \file UnregisteredSession.h
 * \brief Declaration of UnregisteredSession
 * \author Nicolas Laille
 * \version 1.0
 * \date 24 Octobre 2013
 */
#include <boost/thread.hpp>

#include "Session.h"

/**
 * \class UnregisteredSession
 * \brief Representation of an unregistered session
 */
class UnregisteredSession : public Session
{
public:
   /**
    * \fn static ISession::session   create(IConnection::connection connection)
	* \brief Create session of type Unregistered
	* \param connection
	*/
   static ISession::session   create(IConnection::connection connection);

   /**
    * \fn ~UnregisteredSession(void)
	* \brief Destructor
	*/
   virtual ~UnregisteredSession(void);

   /**
    * \fn bool is(SessionType type) const
	* \brief Check if type is Unregistered
	* \param type
	* \return boolean
	*/
   virtual bool is(SessionType type) const;

   /**
    * \fn bool valid(const std::string& id) const
	* \brief Return false, cause the session is not associated with an id
	* \param id
	* \return false
	*/
   virtual bool valid(const std::string& id) const;

   /**
    * \fn void write(const std::string& message)
	* \brief Throws an exception
	* \param message
	*/
   virtual void write(const std::string& message);
protected:
   explicit UnregisteredSession(IConnection::connection connection);
};

