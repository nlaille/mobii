#include "SessionManager.h"

#include "exception/SessionNotFound.h"

SessionManager::SessionManager(void)
   : m_idSessions(), m_connectionSessions()
{
}

SessionManager::~SessionManager(void)
{
}

void        SessionManager::add(const std::string& id, ISession::session session)
{
   boost::mutex::scoped_lock lock(m_mutex);

   m_idSessions.left.insert(std::make_pair(id, session));
}

ISession::session   SessionManager::get(const std::string& id) const
{
   boost::mutex::scoped_lock lock(m_mutex);

   if (!exists(id))
      throw SessionNotFound("get");
   return m_idSessions.left.at(id);
}

bool        SessionManager::exists(const std::string& id) const
{
   return (m_idSessions.left.find(id) != m_idSessions.left.end());
}

void        SessionManager::add(IConnection::connection& connection, ISession::session session)
{
   boost::mutex::scoped_lock lock(m_mutex);

   m_connectionSessions.left.insert(std::make_pair(connection, session));
}

ISession::session   SessionManager::get(IConnection::connection& connection) const
{
   boost::mutex::scoped_lock lock(m_mutex);

   if (!exists(connection))
      throw SessionNotFound("get");
   return m_connectionSessions.left.at(connection);
}

bool        SessionManager::exists(IConnection::connection& connection) const
{
   return (m_connectionSessions.left.find(connection) != m_connectionSessions.left.end());
}

void        SessionManager::remove(ISession::session session)
{
   boost::mutex::scoped_lock lock(m_mutex);

   m_connectionSessions.right.erase(session);
   m_idSessions.right.erase(session);
}

bool        SessionManager::valid(IConnection::connection& connection, const std::string& id) const
{
   try
   {
      ISession::session sessionConnection = get(connection);
      ISession::session sessionId = get(id);

	  return sessionConnection->valid(id);
   }
   catch (const SessionNotFound&)
   {
      return false;
   }
}