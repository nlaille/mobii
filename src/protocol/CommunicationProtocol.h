#pragma once

/**
 * \file CommunicationProtocol.h
 * \brief Declaration of CommunicationProtocol
 * \author Nicolas Laille
 * \version 1.0
 * \date 24 Octobre 2013
 */

#include "Packet.h"
#include "IProtocolHandler.h"
#include "session/ISession.h"
#include "session/SessionManager.h"
#include "session/CommunicationManager.h"


/**
 * \class CommunicationProtocol
 * \brief Handle communication protocol
 */
class CommunicationProtocol : public IProtocolHandler
{
public:
   CommunicationProtocol(SessionManager& sessionManager, CommunicationManager& communicationManager);
   virtual ~CommunicationProtocol(void);

   /**
	* \fn void  closeCommunication(ISession::session source, ISession::session destination, Packet& packet)
	* \brief Close communication between source and destination
	* \param source 
	* \param destination
	* \param packet
	*/
   void  closeCommunication(ISession::session source, ISession::session destination, Packet& packet);
   /**
	* \fn void  redirectPacket(ISession::session source, ISession::session destination, Packet& packet)
	* \brief Redirect packet to destination
	* \param source 
	* \param destination
	* \param packet
	*/
   void  redirectPacket(ISession::session source, ISession::session destination, Packet& packet);

   virtual void  operator()(IConnection::connection connection, Packet& packet);

private:
   SessionManager&         m_sessionManager; /*!< Stack open sessions*/
   CommunicationManager&   m_communicationManager; /*!< Stack open communication*/

   typedef void (CommunicationProtocol::*CommunicationProtocolFunc)(ISession::session, ISession::session, Packet&);
   std::map<char, CommunicationProtocolFunc> m_funcs; /*!< Stack functions to handle communication protocol*/

   void  addFunc(char opcode, CommunicationProtocolFunc function);
};
