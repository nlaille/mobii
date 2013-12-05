#pragma once

#include "IProtocolHandler.h"

#include "session/SessionManager.h"
#include "session/UserSession.h"
#include "session/SmartPhoneSession.h"
#include "session/CommunicationManager.h"
#include "database/DatabaseConnector.h"

class UdpProtocol : public IProtocolHandler
{
public:
   UdpProtocol(SessionManager& sessionManager, CommunicationManager& communicationManager, DatabaseConnector& databaseConnector);
   virtual ~UdpProtocol(void);

   void  udpHolePunching(ISession::session source, ISession::session destination, IConnection::connection connection, Packet& packet);

   virtual void  operator()(IConnection::connection connection, Packet& packet);

private:
   SessionManager&         m_sessionManager;
   CommunicationManager&   m_communicationManager;
   DatabaseConnector&      m_databaseConnector;

   typedef void (UdpProtocol::*UdpProtocolFunc)(ISession::session, ISession::session, IConnection::connection connection, Packet&);
   std::map<char, UdpProtocolFunc> m_funcs;

   void  addFunc(char opcode, UdpProtocolFunc function);

   bool  isValidUidPassword(const std::string& uid, int password);
};

