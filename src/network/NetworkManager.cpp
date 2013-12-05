#include "NetworkManager.h"

NetworkManager::NetworkManager()
   : m_networks()
{
}

NetworkManager::~NetworkManager(void)
{
   std::list<Acceptor*>::iterator  it;
   
   it = m_networks.begin();
   for (; it != m_networks.end(); ++it)
   {
      (*it)->stop();
      delete (*it);
   }
}

void  NetworkManager::add(Acceptor* acceptor)
{
   m_networks.push_back(acceptor);
}


void     NetworkManager::start(void)
{
   std::list<Acceptor*>::iterator  it;
   
   it = m_networks.begin();
   for (; it != m_networks.end(); ++it)
   {
      (*it)->start();
   }
}