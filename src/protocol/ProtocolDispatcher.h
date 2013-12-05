#pragma once

/**
 * \file ProtocolDispatcher.h
 * \brief Declaration of ProtocolDispatcher
 * \author Nicolas Laille
 * \version 1.0
 * \date 24 Octobre 2013
 */
#include <map>
#include <boost/thread.hpp>

#include "IProtocolHandler.h"

#include "session/SessionManager.h"
#include "session/CommunicationManager.h"
#include "database/DatabaseConnector.h"
#include "database/UserManager.h"

/**
 * \class ProtocolDispatcher
 * \brief Dispatch packet to IProtocolHandler
 */
class ProtocolDispatcher
{
public:
   ProtocolDispatcher(SessionManager& sessionManager, CommunicationManager& communicationManager, DatabaseConnector& databaseConnector, UserManager& userManager);
   ProtocolDispatcher(const ProtocolDispatcher& dispatcher);

   ~ProtocolDispatcher();

   void  start();
private:
   void  handle_();

   bool                 m_isAlive;
   boost::thread        m_thread;

   SessionManager&      m_sessionManager;

   std::map<char, IProtocolHandler*>  m_handlers;
};
