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
   static ISession::session   create(IConnection::connection connection, const std::string& id);
   virtual ~SmartPhoneSession(void);

   virtual bool is(SessionType type) const;
protected:
   explicit SmartPhoneSession(IConnection::connection connection, const std::string& id);
};

