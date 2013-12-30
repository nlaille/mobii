#pragma once

/**
 * \file ProtocolDispatcher.h
 * \brief Declaration of ProtocolDispatcher
 * \author Nicolas Laille
 * \version 1.0
 * \date 24 Octobre 2013
 */
#include <map>
#include <boost/thread.hpp>

#include "IProtocolHandler.h"

#include "session/SessionManager.h"
#include "session/CommunicationManager.h"
#include "database/DatabaseConnector.h"
#include "database/UserManager.h"

/**
 * \class ProtocolDispatcher
 * \brief Dispatch packet to IProtocolHandler
 */
class ProtocolDispatcher
{
public:
   /**
    * \fn ProtocolDispatcher(SessionManager& sessionManager, CommunicationManager& communicationManager, DatabaseConnector& databaseConnector, UserManager& userManager)
	* \brief Constructor
	* \param session manager
	* \param communication manager
	* \param database connector
	* \param user manager
	*/
   ProtocolDispatcher(SessionManager& sessionManager, CommunicationManager& communicationManager, DatabaseConnector& databaseConnector, UserManager& userManager);
   
   /**
    * \fn ProtocolDispatcher(const ProtocolDispatcher& dispatcher)
	* \brief Copy constructor
	* \param dispatcher
	*/
   ProtocolDispatcher(const ProtocolDispatcher& dispatcher);

   /**
    * \fn ~ProtocolDispatcher()
	* \brief destructor
	*/
   ~ProtocolDispatcher();

   /**
    * \fn void start()
	* \breif Start a thread which will dispatch Packet to IProtocolHandler
	*/
   void  start();
private:
   void  handle_();

   bool                 m_isAlive; /*!< is alive */
   boost::thread        m_thread; /*!< thread */

   SessionManager&      m_sessionManager; /*!< session manager */

   std::map<char, IProtocolHandler*>  m_handlers; /*!< handlers */
};
