#include "ConnectionManager.h"

ConnectionManager::ConnectionManager(void)
   : m_connections()
{
}

ConnectionManager::~ConnectionManager(void)
{
}

void        ConnectionManager::add(IConnection::connection& connection)
{
   boost::mutex::scoped_lock lock(m_mutex);

   m_connections.push(connection);
}

void        ConnectionManager::remove(IConnection::connection& connection)
{
   boost::mutex::scoped_lock lock(m_mutex);

   m_connections.remove(connection);
}

bool        ConnectionManager::exists(IConnection::connection& connection) const
{
   boost::mutex::scoped_lock lock(m_mutex);

   return m_connections.exists(connection);
}