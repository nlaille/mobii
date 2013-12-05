#pragma once

/**
 * \file InternalProtocol.h
 * \brief Declaration of InternalProtocol
 * \author Nicolas Laille
 * \version 1.0
 * \date 24 Octobre 2013
 */

#include "IProtocolHandler.h"

#include "network/ConnectionManager.h"
#include "session/SessionManager.h"
#include "session/CommunicationManager.h"
#include "database/UserManager.h"


/**
 * \class InternalProtocol
 * \brief Handle internal protocol
 */
class InternalProtocol : public IProtocolHandler
{
public:
   InternalProtocol(SessionManager& sessionManager, CommunicationManager& communicationManager, UserManager& userManager);
   virtual ~InternalProtocol(void);

   void  connect(IConnection::connection connection);
   void  disconnected(IConnection::connection connection);

   virtual void  operator()(IConnection::connection connection, Packet& packet);
private:
   SessionManager&         m_sessionManager;
   CommunicationManager&   m_communicationManager;
   UserManager&			   m_userManager;

   typedef void (InternalProtocol::*ServerProtocolFunc)(IConnection::connection);
   std::map<char, ServerProtocolFunc> m_funcs;

   void  addFunc(char opcode, ServerProtocolFunc func);
};
