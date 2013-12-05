#pragma once

/**
 * \file DatabaseProtocol.h
 * \brief Declaration of DatabaseProtocol
 * \author Nicolas Laille
 * \version 1.0
 * \date 24 Octobre 2013
 */

#include "Packet.h"
#include "IProtocolHandler.h"
#include "session/ISession.h"
#include "session/SessionManager.h"
#include "session/CommunicationManager.h"
#include "database/DatabaseConnector.h"


/**
 * \class DatabaseProtocol
 * \brief Handle database protocol
 */
class DatabaseProtocol : public IProtocolHandler
{
public:
   DatabaseProtocol(SessionManager& sessionManager, DatabaseConnector& databaseConnector);
   virtual ~DatabaseProtocol(void);

   void  registerID(ISession::session source, Packet& packet);

   virtual void  operator()(IConnection::connection connection, Packet& packet);

private:
   SessionManager&         m_sessionManager;
   DatabaseConnector&      m_databaseConnector;

   typedef void (DatabaseProtocol::*DatabaseProtocolFunc)(ISession::session, Packet&);
   std::map<char, DatabaseProtocolFunc> m_funcs;

   void  addFunc(char opcode, DatabaseProtocolFunc function);
};
