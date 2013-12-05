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
 * \brief Stack sessions
 */
class SessionManager
{
public:
   typedef boost::bimap<std::string, ISession::session>                 idsMap;
   typedef boost::bimap<IConnection::connection, ISession::session>     connectionsMap;

   explicit SessionManager(void);
   virtual ~SessionManager(void);

   virtual void        add(const std::string& id, ISession::session session);
   virtual ISession::session   get(const std::string& id) const;
   virtual bool        exists(const std::string& id) const;

   virtual void        add(IConnection::connection& connection, ISession::session session);
   virtual ISession::session   get(IConnection::connection& connection) const;
   virtual bool        exists(IConnection::connection& connection) const;

   virtual bool        valid(IConnection::connection& connection, const std::string& id) const;

   virtual void        remove(ISession::session session);

private:
   idsMap                  m_idSessions;
   connectionsMap          m_connectionSessions;
   mutable boost::mutex    m_mutex;
};