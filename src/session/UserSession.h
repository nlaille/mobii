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
   static ISession::session   create(IConnection::connection connection, const std::string& id);
   virtual ~UserSession(void);

   virtual bool is(SessionType type) const;
protected:
   explicit UserSession(IConnection::connection connection, const std::string& id);
};

