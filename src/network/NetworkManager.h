#pragma once

/**
 * \file NetworkManager.h
 * \brief Declaration of NetworkManager
 * \author Nicolas Laille
 * \version 1.0
 * \date 24 Octobre 2013
 */

#include <list>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/asio/ssl.hpp>

#include "Acceptor.h"

/**
 * \class NetworkManager
 * \brief Stack Acceptor
 */
class NetworkManager
{
public:
   NetworkManager(void);
   ~NetworkManager(void);

   void  add(Acceptor* acceptor);

   void start(void);
private:
   std::list<Acceptor*>       m_networks;
};

