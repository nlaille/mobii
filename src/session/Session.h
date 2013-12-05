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
   explicit Session(IConnection::connection connection, const std::string& id);
   virtual ~Session(void);

   virtual bool is(SessionType type) const = 0;
   virtual bool valid(const std::string& id) const;

   virtual void write(const std::string& message);
   virtual void sendTo(Packet& packet);
   virtual void sendFromTo(ISession::session& session, Packet& packet);

   virtual IConnection::connection	getConnection() const;

   virtual void lock();
   virtual void unlock();

   virtual void close();
private:
   IConnection::connection             m_connection;
   std::string                         m_id;
   mutable boost::mutex                m_mutex;
};

