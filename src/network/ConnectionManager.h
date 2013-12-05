#pragma once

/**
 * \file ConnectionManager.h
 * \brief Declaration of ConnectionManager
 * \author Nicolas Laille
 * \version 1.0
 * \date 24 Octobre 2013
 */

#include <boost/thread.hpp>

#include "IConnection.h"
#include "safe/List.h"

/**
 * \class ConnectionManager
 * \brief Stack open Connection
 */
class ConnectionManager
{
public:
   explicit ConnectionManager(void);
   ~ConnectionManager(void);

   /**
	* \fn void        add(IConnection::connection& connection)
	* \brief Add new open connection
	* \param connection 
	*/
   void        add(IConnection::connection& connection);

   /**
	* \fn void        remove(IConnection::connection& connection)
	* \brief Remove open connection
	* \param connection 
	*/
   void        remove(IConnection::connection& connection);

   /**
	* \fn void        exists(IConnection::connection& connection)
	* \brief Check if connection is stacked
	* \param connection 
	* \return Connection exists
	*/
   bool        exists(IConnection::connection& connection) const;
private:
   List<IConnection::connection>    m_connections; /*!< Stack of connections*/
   mutable boost::mutex             m_mutex; /*!< Mutex for access*/
};