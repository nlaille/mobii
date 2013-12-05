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
   Server();
   ~Server();

   void  init();
   void  start();
private:
   boost::asio::io_service      m_ioService;

   NetworkManager               m_networkManager;
   ProtocolManager              m_protocoleManager;

   std::ofstream				m_accessLogFile;
   std::ofstream				m_errorLogFile;
};