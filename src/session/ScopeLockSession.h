#pragma once

/**
 * \file ScopeLockSession.h
 * \brief Declaration of ScopeLockSession
 * \author Nicolas Laille
 * \version 1.0
 * \date 24 Octobre 2013
 */
#include <string>

#include "session/ISession.h"

/**
 * \class ScopeLockSession
 * \brief Use to lock sessions during protocol handling
 */
class ScopeLockSession
{
public:
   /**
    * \fn ScopeLockSession(ISession::session session)
	* \brief Lock the session until the class is destroy
	* \param session
	*/
   ScopeLockSession(ISession::session session);
   /**
    * \fn ~ScopeLockSession()
	* \brief Unlock the session
	*/
   ~ScopeLockSession();

private:
   ISession::session   m_session; /*!< session */
};