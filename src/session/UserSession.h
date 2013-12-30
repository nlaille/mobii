#pragma once

/**
 * \file UserSession.h
 * \brief Declaration of UserSession
 * \author Nicolas Laille
 * \version 1.0
 * \date 24 Octobre 2013
 */
#include <boost/thread.hpp>

#include "Session.h"

/**
 * \class UserSession
 * \brief Representation of a client application
 */
class UserSession : public Session
{
public:
   /**
    * \fn ISession::session   create(IConnection::connection connection, const std::string& id)
	* \brief Create session of type User
	* \param connection
	* \param id
	* \return session
	*/
   static ISession::session   create(IConnection::connection connection, const std::string& id);

   /**
    * \fn ~UserSession(void)
	* \brief Destructor
	*/
   virtual ~UserSession(void);

   /**
    * \fn bool is(SessionType type) const
	* \brief Check if type is User
	* \param type
	* \return boolean
	*/
   virtual bool is(SessionType type) const;
protected:
   explicit UserSession(IConnection::connection connection, const std::string& id);
};

