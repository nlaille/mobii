#include "NoSession.h"

#include "exception/NoSessionException.h"

NoSession::NoSession()
{
}


NoSession::~NoSession(void)
{
}

ISession::session   NoSession::create()
{
   return ISession::session(new NoSession());
}

bool NoSession::is(SessionType type) const
{
   throw NoSessionException("is");
}

bool NoSession::valid(const std::string& id) const
{
	throw NoSessionException("valid");
}

void NoSession::write(const std::string& message)
{
   throw NoSessionException("write");
}

void NoSession::sendTo(Packet& packet)
{
   throw NoSessionException("sendTo");
}

void NoSession::sendFromTo(ISession::session& session, Packet& packet)
{
   throw NoSessionException("sendFromTo");
}

IConnection::connection	NoSession::getConnection() const
{
	throw NoSessionException("getConnection");
}

void NoSession::lock()
{
}

void NoSession::unlock()
{
}

void NoSession::close()
{
}
