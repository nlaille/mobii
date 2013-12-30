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
	/**
	* \fn InternalProtocol(SessionManager& sessionManager, CommunicationManager& communicationManager, UserManager& userManager)
	* \brief Constructor
	* \param sessionManager
	* \param communicationManager
	* \param userManager
	*/
   InternalProtocol(SessionManager& sessionManager, CommunicationManager& communicationManager, UserManager& userManager);

   /**
	* \fn ~InternalProtocol(void)
	* \brief Destructor
	*/
   virtual ~InternalProtocol(void);

   /**
	* \fn void  operator()(IConnection::connection connection, Packet& packet)
	* \brief Handle packet of type Internal (Connect/Disconnect)
	* \param connection
	* \param packet
	*/
   virtual void  operator()(IConnection::connection connection, Packet& packet);
private:
   SessionManager&         m_sessionManager; /*!< sessionManager*/
   CommunicationManager&   m_communicationManager; /*!< communicationManager*/
   UserManager&			   m_userManager; /*!< userManager*/

   typedef void (InternalProtocol::*ServerProtocolFunc)(IConnection::connection);
   std::map<char, ServerProtocolFunc> m_funcs;

   void  connect(IConnection::connection connection);
   void  disconnected(IConnection::connection connection);

   void  addFunc(char opcode, ServerProtocolFunc func);
};
