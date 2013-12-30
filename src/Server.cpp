#include "Server.h"

#include "network/UdpAcceptor.h"
#include "network/TcpAcceptor.h"
#include "network/TcpSSLAcceptor.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

#include <iostream>

Server::Server()
   : m_ioService(),
   m_networkManager(),
   m_protocolManager()
{
}

Server::~Server()
{
	m_errorLogFile.close();
	m_accessLogFile.close();
}

void     Server::init()
{
   boost::property_tree::ptree	propertyTree;

   boost::property_tree::ini_parser::read_ini("config/parameters.ini", propertyTree);

   m_errorLogFile.open(propertyTree.get<std::string>("log.error").c_str(), std::ios_base::out | std::ios_base::app);
   std::cerr.rdbuf(m_errorLogFile.rdbuf());
   m_accessLogFile.open(propertyTree.get<std::string>("log.access").c_str(), std::ios_base::out | std::ios_base::app);
   std::clog.rdbuf(m_accessLogFile.rdbuf());
   m_networkManager.add(new TcpSSLAcceptor(m_ioService, propertyTree.get<long>("connection.tcpssl")));
   m_networkManager.add(new TcpAcceptor(m_ioService, propertyTree.get<long>("connection.tcp")));
   m_networkManager.add(new UdpAcceptor(m_ioService, propertyTree.get<long>("connection.udp")));
}

void     Server::start()
{
   m_protocolManager.start();
   m_networkManager.start();
   m_ioService.run();
}
