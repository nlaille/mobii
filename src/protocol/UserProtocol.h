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
   /**
    * \fn UserProtocol(SessionManager& sessionManager, CommunicationManager& communicationManager, DatabaseConnector& databaseConnector, UserManager& userManager)
	* \brief Constructor
	* \param sessionManager
	* \param communicationManager
	* \param databaseConnector
	* \param userManager
	*/
   UserProtocol(SessionManager& sessionManager, CommunicationManager& communicationManager, DatabaseConnector& databaseConnector, UserManager& userManager);

   /**
    * \fn ~UserProtocol(void)
	* \brief Destructor
	*/
   virtual ~UserProtocol(void);

   /**
    * \fn void  askConnectionToPhone(ISession::session source, ISession::session destination, Packet& packet)
	* \brief Source (User) asks destination (SmartPhone) to open a connection
	* \param source
	* \param destination
	* \param packet
	*/
   void  askConnectionToPhone(ISession::session source, ISession::session destination, Packet& packet);

   /**
    * \fn void  login(ISession::session source, ISession::session, Packet& packet)
	* \brief Source (User) tries to login
	* \param source
	* \param destination
	* \param packet
	*/
   void  login(ISession::session source, ISession::session, Packet& packet);

   /**
    * \fn void  contact(ISession::session source, ISession::session, Packet& packet)
	* \brief Source (User) get list of smartphones associated to is User
	* \param source
	* \param destination
	* \param packet
	*/
   void  contact(ISession::session source, ISession::session, Packet& packet);

   /**
    * \fn void  operator()(IConnection::connection connection, Packet& packet)
	* \brief Handle packet received from connection
	* \param connection
	* \param packet
	*/
   virtual void  operator()(IConnection::connection connection, Packet& packet);

private:
   SessionManager&         m_sessionManager; /*!< sessionManager */
   CommunicationManager&   m_communicationManager; /*!< communicationManager */
   DatabaseConnector&	   m_databaseConnector; /*!< databaseConnector */
   UserManager&			   m_userManager; /*!< userManager */

   typedef void (UserProtocol::*UserProtocolFunc)(ISession::session, ISession::session, Packet&);
   std::map<char, UserProtocolFunc> m_funcs; /*!< funcs */

   void  addFunc(char opcode, UserProtocolFunc function);

   boost::property_tree::ptree  addContacts(User user, boost::property_tree::ptree favoris);
   boost::property_tree::ptree  renameContacts(User user, boost::property_tree::ptree favoris);
   boost::property_tree::ptree  removeContacts(User user, boost::property_tree::ptree favoris);
   boost::property_tree::ptree  listContacts(User user);
};

