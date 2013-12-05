#include "UnregisteredSession.h"

#include "protocol/Protocol.h"

UnregisteredSession::UnregisteredSession(IConnection::connection connection)
   : Session(connection, Protocol::Header::EMPTY_ID)
{
}

UnregisteredSession::~UnregisteredSession(void)
{
}

ISession::session   UnregisteredSession::create(IConnection::connection connection)
{
   return ISession::session(new UnregisteredSession(connection));
}

bool UnregisteredSession::is(SessionType type) const
{
   return (type == UNREGISTERED);
}

bool UnregisteredSession::valid(const std::string& id) const
{
	return false;
}

void UnregisteredSession::write(const std::string& message)
{
   throw std::logic_error("UnregisteredSession");
}