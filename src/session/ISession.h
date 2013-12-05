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
   enum SessionType
   {
      NOSESSION=0,
      UNREGISTERED=1,
      SMARTPHONE=10,
      USER=11,
      ADMIN=100
   };

   typedef boost::shared_ptr<ISession> session;

   virtual ~ISession() {std::cout<<"Session deleted"<<std::endl;}

   virtual bool is(SessionType type) const = 0;
   virtual bool valid(const std::string& id) const = 0;

   virtual void write(const std::string& message) = 0;
   virtual void sendTo(Packet& packet) = 0;
   virtual void sendFromTo(ISession::session& session, Packet& packet) = 0;

   virtual IConnection::connection	getConnection() const = 0;

   virtual void lock() = 0;
   virtual void unlock() = 0;

   virtual void close() = 0;
};
