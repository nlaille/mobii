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
   /**
    * \fn SmartPhoneProtocol(SessionManager& sessionManager, CommunicationManager& communicationManager)
	* \brief Constructor
	* \param sessionManager
	* \param communicationManager
	*/
   SmartPhoneProtocol(SessionManager& sessionManager, CommunicationManager& communicationManager);

   /**
    * \fn ~SmartPhoneProtocol(void)
	* \brief Destructor
	*/
   virtual ~SmartPhoneProtocol(void);

   /**
    * \fn void  acceptConnection(ISession::session& source, ISession::session& destination, Packet& packet)
	* \brief Accept connection from source session which is a UserSession
	* \param source
	* \param destination
	* \param packet
	*/
   void  acceptConnection(ISession::session& source, ISession::session& destination, Packet& packet);

   /**
    * \fn void  declineConnection(ISession::session& source, ISession::session& destination, Packet& packet)
	* \brief Decline connection from source session which is a UserSession
	* \param source
	* \param destination
	* \param packet
	*/
   void  declineConnection(ISession::session& source, ISession::session& destination, Packet& packet);

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

   typedef void (SmartPhoneProtocol::*SmartPhoneProtocolFunc)(ISession::session&, ISession::session&, Packet&);
   std::map<char, SmartPhoneProtocolFunc> m_funcs; /*!< funcs */

   void  addFunc(char opcode, SmartPhoneProtocolFunc func);
};

