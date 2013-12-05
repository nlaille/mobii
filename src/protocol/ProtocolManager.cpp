#include "ProtocolManager.h"

#include "session/NoSession.h"
#include "session/Session.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

ProtocolManager::ProtocolManager()
   : m_dispatchers(0), m_sessionManager(), m_communicationManager(), m_userManager()
{
   m_sessionManager.add(Protocol::Header::EMPTY_ID, NoSession::create());
   boost::property_tree::ptree	propertyTree;
   std::stringstream			databaseConnection;

   boost::property_tree::ini_parser::read_ini("config/parameters.ini", propertyTree);
   databaseConnection << propertyTree.get<std::string>("database.type") << "://";
   databaseConnection << "dbname=" << propertyTree.get<std::string>("database.dbname");
   databaseConnection << " user=" << propertyTree.get<std::string>("database.user");
   databaseConnection << " password=" << propertyTree.get<std::string>("database.password");

   m_databaseConnector.setConnection(databaseConnection.str());
   for (int i = 0; i < 4; ++i)
   {
      this->addDispatcher(new ProtocolDispatcher(m_sessionManager, m_communicationManager, m_databaseConnector, m_userManager));
   }
}

ProtocolManager::~ProtocolManager(void)
{
}

void  ProtocolManager::start(void)
{
}

void  ProtocolManager::addDispatcher(ProtocolDispatcher* ProtocolDispatcher)
{
   this->m_dispatchers.push_back(ProtocolDispatcher);
   ProtocolDispatcher->start();
}