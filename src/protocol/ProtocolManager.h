#pragma once

/**
 * \file ProtocolManager.h
 * \brief Declaration of ProtocolManager
 * \author Nicolas Laille
 * \version 1.0
 * \date 24 Octobre 2013
 */
#include <list>

#include "ProtocolDispatcher.h"
#include "session/SessionManager.h"
#include "session/CommunicationManager.h"
#include "database/DatabaseConnector.h"
#include "database/UserManager.h"

/**
 * \class ProtocolManager
 * \brief Handle thread whose handle protocol
 */
class ProtocolManager
{
public:
   ProtocolManager();

   ~ProtocolManager();

   void  start();
   void  addDispatcher(ProtocolDispatcher* protocolDispatcher);
private:
   std::list<ProtocolDispatcher*>    m_dispatchers;
   SessionManager                    m_sessionManager;
   CommunicationManager              m_communicationManager;
   DatabaseConnector				 m_databaseConnector;
   UserManager						 m_userManager;
};
