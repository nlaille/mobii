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
   /**
    * \fn NetworkManager()
	* \brief Constructor
	*/
   NetworkManager(void);

   /**
    * \fn ~NetworkManager()
	* \brief Destructor
	*/
   ~NetworkManager(void);

   /**
    * \fn void  add(Acceptor* acceptor)
	* \brief Add an acceptor
	* \param acceptor
	*/
   void  add(Acceptor* acceptor);

   /**
    * \fn void start(void)
	* \brief Start all acceptors
	*/
   void start(void);
private:
   std::list<Acceptor*>       m_networks; /*!< acceptors */
};

