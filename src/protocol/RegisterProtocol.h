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
   /**
    * \fn RegisterProtocol(SessionManager& sessionManager, DatabaseConnector& databaseConnector)
	* \brief Constructor
	* \param sessionManager
	* \param databaseConnector
	*/
   RegisterProtocol(SessionManager& sessionManager, DatabaseConnector& databaseConnector);

   /**
    * \fn ~RegisterProtocol(void)
	* \brief Destructor
	*/
   virtual ~RegisterProtocol(void);

   /**
    * \fn void  registerSmartPhone(IConnection::connection connection, const std::string& id, Packet& packet)
	* \brief Register a SmartPhone application to the server, check if id match on database with additionnals informations
	* \param connection
	* \param id
	* \param packet
	*/
   void  registerSmartPhone(IConnection::connection connection, const std::string& id, Packet& packet);

   /**
    * \fn void  registerUser(IConnection::connection connection, const std::string& id, Packet& packet)
	* \brief Register an User application to the server, check if id match on database with additionnals informations
	* \param connection
	* \param id
	* \param packet
	*/
   void  registerUser(IConnection::connection connection, const std::string& id, Packet& packet);

   /**
    * \fn void  operator()(IConnection::connection connection, Packet& packet)
	* \brief Handle packet received from connection
	* \param connection
	* \param packet
	*/
   virtual void  operator()(IConnection::connection connection, Packet& packet);

private:
   SessionManager&   m_sessionManager; /*!< sessionManager */
   DatabaseConnector&m_databaseConnector; /*!< databaseConnector */

   typedef void (RegisterProtocol::*RegisterFunc)(IConnection::connection, const std::string&, Packet&);
   std::map<char, RegisterFunc> m_funcs; /*!< funcs */

   void  addFunc(char opcode, RegisterFunc function);
   void  unableToRegister(IConnection::connection connection);
   bool  isValidUidPassword(const std::string& uid, int password);
   bool  isSmartPhone(const std::string& uid);
};
