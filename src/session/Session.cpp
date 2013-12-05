#include "Session.h"

#include <algorithm>
#include <sstream>

#include "protocol/Packet.h"
#include "protocol/ProtocolQueue.h"

#include "exception/UnableToLock.h"

#include <boost/algorithm/string.hpp> 

Session::Session(IConnection::connection connection, const std::string& id)
   : m_connection(connection), m_id(id)
{
}

Session::~Session(void)
{
}

bool Session::valid(const std::string& id) const
{
	return boost::iequals(m_id, id);
}

void Session::write(const std::string& message)
{
   m_connection->write(message);
}

void Session::sendTo(Packet& packet)
{
   packet.setDestination(m_id);
   m_connection->write(packet.toString());
}

void Session::sendFromTo(ISession::session& session, Packet& packet)
{
   packet.setSource(m_id);
   session->sendTo(packet);
}

void Session::lock()
{
   if (!m_mutex.try_lock())
      throw UnableToLock("session");
}

void Session::unlock()
{
   m_mutex.unlock();
}

void Session::close()
{
   m_connection->close();
}

IConnection::connection	Session::getConnection() const
{
	return m_connection;
}