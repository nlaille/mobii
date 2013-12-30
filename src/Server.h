#pragma once

/**
 * \file Server.h
 * \brief Declaration of Server
 * \author Nicolas Laille
 * \version 1.0
 * \date 24 Octobre 2013
 */
#include <boost/asio.hpp>
#include <boost/bind.hpp>

#include <fstream>

#include "protocol/ProtocolManager.h"
#include "network/NetworkManager.h"

/**
 * \class Server
 * \brief Representation of the server
 */
class Server
{
public:
   /**
    * \fn Server()
	* \brief Constructor
	*/
   Server();

   /**
    * \fn Server()
	* \brief Destructor
	*/
   ~Server();

   /**
    * \fn void  init()
	* \brief Initialize property of the server (acceptors, protocol handlers, loggers, ..)
	*/
   void  init();

   /**
    * \fn void  start()
	* \brief Start acceptors, protocol handlers
	*/
   void  start();
private:
   boost::asio::io_service      m_ioService; /*!< ioService, boost */

   NetworkManager               m_networkManager; /*!< networkManager, acceptors */
   ProtocolManager              m_protocolManager; /*!< protocolManager, protocols */

   std::ofstream				m_accessLogFile; /*!< accessLogFile, logs */
   std::ofstream				m_errorLogFile; /*!< errorLogFile, logs */
};