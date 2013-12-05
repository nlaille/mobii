#pragma once

/**
 * \file SmartPhoneProtocol.h
 * \brief Declaration of SmartPhoneProtocol
 * \author Nicolas Laille
 * \version 1.0
 * \date 24 Octobre 2013
 */
#include "IProtocolHandler.h"

#include "session/ISession.h"
#include "session/SessionManager.h"
#include "session/CommunicationManager.h"

/**
 * \class SmartPhoneProtocol
 * \brief Handle smartphone protocol
 */
class SmartPhoneProtocol : public IProtocolHandler
{
public:
   SmartPhoneProtocol(SessionManager& sessionManager, CommunicationManager& communicationManager);
   virtual ~SmartPhoneProtocol(void);

   void  acceptConnection(ISession::session& source, ISession::session& destination, Packet& packet);
   void  declineConnection(ISession::session& source, ISession::session& destination, Packet& packet);

   virtual void  operator()(IConnection::connection connection, Packet& packet);
private:
   SessionManager&         m_sessionManager;
   CommunicationManager&   m_communicationManager;

   typedef void (SmartPhoneProtocol::*SmartPhoneProtocolFunc)(ISession::session&, ISession::session&, Packet&);
   std::map<char, SmartPhoneProtocolFunc> m_funcs;

   void  addFunc(char opcode, SmartPhoneProtocolFunc func);
};

