#include "ScopeLockSession.h"

ScopeLockSession::ScopeLockSession(ISession::session session)
   : m_session(session)
{
   m_session->lock();
}

ScopeLockSession::~ScopeLockSession()
{
   m_session->unlock();
}