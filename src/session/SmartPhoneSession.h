#pragma once

/**
 * \file SmartPhoneSession.h
 * \brief Declaration of SmartPhoneSession
 * \author Nicolas Laille
 * \version 1.0
 * \date 24 Octobre 2013
 */
#include <boost/thread.hpp>

#include "Session.h"

/**
 * \class SmartPhoneSession
 * \brief Representation of a mobile application
 */
class SmartPhoneSession : public Session
{
public:
   /**
    * \fn ISession::session   create(IConnection::connection connection, const std::string& id)
	* \brief Create session of type SmartPhone
	* \param connection
	* \param id
	* \return new session
	*/
   static ISession::session   create(IConnection::connection connection, const std::string& id);

   /**
    * \fn ~SmartPhoneSession(void)
	* \brief Destructor
	*/
   virtual ~SmartPhoneSession(void);

   /**
    * \fn bool is(SessionType type) const
	* \brief Check if type is SmartPhone
	* \param type
	* \return new boolean
	*/
   virtual bool is(SessionType type) const;
protected:
   explicit SmartPhoneSession(IConnection::connection connection, const std::string& id);
};

