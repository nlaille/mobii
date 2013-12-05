#pragma once

/**
 * \file UserProtocol.h
 * \brief Declaration of UserProtocol
 * \author Nicolas Laille
 * \version 1.0
 * \date 24 Octobre 2013
 */
#include "IProtocolHandler.h"

#include "session/SessionManager.h"
#include "session/UserSession.h"
#include "session/SmartPhoneSession.h"
#include "session/CommunicationManager.h"

#include "database/DatabaseConnector.h"
#include "database/UserManager.h"

#include <boost/property_tree/ptree.hpp>

/**
 * \class UserProtocol
 * \brief Handle user protocol
 */
class UserProtocol : public IProtocolHandler
{
public:
   UserProtocol(SessionManager& sessionManager, CommunicationManager& communicationManager, DatabaseConnector& databaseConnector, UserManager& userManager);
   virtual ~UserProtocol(void);

   void  askConnectionToPhone(ISession::session source, ISession::session destination, Packet& packet);
   void  login(ISession::session source, ISession::session, Packet& packet);
   void  contact(ISession::session source, ISession::session, Packet& packet);


   virtual void  operator()(IConnection::connection connection, Packet& packet);

private:
   SessionManager&         m_sessionManager;
   CommunicationManager&   m_communicationManager;
   DatabaseConnector&	   m_databaseConnector;
   UserManager&			   m_userManager;

   typedef void (UserProtocol::*UserProtocolFunc)(ISession::session, ISession::session, Packet&);
   std::map<char, UserProtocolFunc> m_funcs;

   void  addFunc(char opcode, UserProtocolFunc function);

   boost::property_tree::ptree  addContacts(User user, boost::property_tree::ptree favoris);
   boost::property_tree::ptree  renameContacts(User user, boost::property_tree::ptree favoris);
   boost::property_tree::ptree  removeContacts(User user, boost::property_tree::ptree favoris);
   boost::property_tree::ptree  listContacts(User user);
};

