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
   static ISession::session   create(IConnection::connection connection);
   virtual ~UnregisteredSession(void);

   virtual bool is(SessionType type) const;
   virtual bool valid(const std::string& id) const;

   virtual void write(const std::string& message);
protected:
   explicit UnregisteredSession(IConnection::connection connection);
};

