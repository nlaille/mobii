#include "UserSession.h"

UserSession::UserSession(IConnection::connection connection, const std::string& id)
   : Session(connection, id)
{
}

UserSession::~UserSession(void)
{
}

ISession::session   UserSession::create(IConnection::connection connection, const std::string& id)
{
   return ISession::session(new UserSession(connection, id));
}

bool UserSession::is(SessionType type) const
{
   return (type == USER);
}