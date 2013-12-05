#pragma once

/**
 * \file NoSession.h
 * \brief Declaration of NoSession
 * \author Nicolas Laille
 * \version 1.0
 * \date 24 Octobre 2013
 */
#include "Session.h"

/**
 * \class NoSession
 * \brief Representation of an un-existed session
 */
class NoSession : public ISession
{
public:
   static ISession::session   create();

   virtual ~NoSession();

   virtual bool is(SessionType type) const;
   virtual bool valid(const std::string& id) const;

   virtual void write(const std::string& message);
   virtual void sendTo(Packet& packet);
   virtual void sendFromTo(ISession::session& session, Packet& packet);

   virtual IConnection::connection	getConnection() const;

   virtual void lock();
   virtual void unlock();

   virtual void close();
private:
   explicit NoSession();
};

