#include "SmartPhoneSession.h"

SmartPhoneSession::SmartPhoneSession(IConnection::connection connection, const std::string& id)
   : Session(connection, id)
{
}

SmartPhoneSession::~SmartPhoneSession(void)
{
}

ISession::session   SmartPhoneSession::create(IConnection::connection connection, const std::string& id)
{
   return ISession::session(new SmartPhoneSession(connection, id));
}

bool SmartPhoneSession::is(SessionType type) const
{
   return (type == SMARTPHONE);
}