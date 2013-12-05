#pragma once

/**
 * \file RegisterProtocol.h
 * \brief Declaration of RegisterProtocol
 * \author Nicolas Laille
 * \version 1.0
 * \date 24 Octobre 2013
 */
#include "IProtocolHandler.h"
#include "session/UnregisteredSession.h"
#include "session/SessionManager.h"
#include "database/DatabaseConnector.h"

#include "network/IConnection.h"

/**
 * \class RegisterProtocol
 * \brief Handle register protocol
 */
class RegisterProtocol : public IProtocolHandler
{
public:
   RegisterProtocol(SessionManager& sessionManager, DatabaseConnector& databaseConnector);
   virtual ~RegisterProtocol(void);

   void  registerSmartPhone(IConnection::connection connection, const std::string& id, Packet& packet);
   void  registerUser(IConnection::connection connection, const std::string& id, Packet& packet);

   virtual void  operator()(IConnection::connection connection, Packet& packet);

private:
   SessionManager&   m_sessionManager;
   DatabaseConnector&m_databaseConnector;

   typedef void (RegisterProtocol::*RegisterFunc)(IConnection::connection, const std::string&, Packet&);
   std::map<char, RegisterFunc> m_funcs;

   void  addFunc(char opcode, RegisterFunc function);
   void  unableToRegister(IConnection::connection connection);
   bool  isValidUidPassword(const std::string& uid, int password);
   bool  isSmartPhone(const std::string& uid);
};
