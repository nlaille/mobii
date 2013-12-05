#pragma once

/**
 * \file DatabaseConnector.h
 * \brief Declaration of DatabaseConnector
 * \author Nicolas Laille
 * \version 1.0
 * \date 24 Octobre 2013
 */

#include <soci.h>

#include "Model.h"

/**
 * \class DatabaseConnector
 * \brief Connector to the database
 */
class DatabaseConnector
{
public:
	/**
	* \fn DatabaseConnector(const std::size_t numberOfConnexion = 1)
	* \brief Constructor, initialize connection_pool with numberOfConnexion
	* \param numberOfConnexion 
	*/
   DatabaseConnector(const std::size_t numberOfConnexion = 1);

   /**
	* \fn ~DatabaseConnector()
	* \brief Destructor
	*/
   virtual ~DatabaseConnector();

   /**
	* \fn void		setConnection(const std::string& connexion)
	* \brief Initialize connection_pool with connexion as settings
	* \param connexion 
	*/
   virtual void		setConnection(const std::string& connexion);

   /**
	* \fn T		select(const std::string& selectQuery)
	* \brief Retreive an instance of T class into the database
	* \param selectQuery 
	* \return An instance
	*/
   template <typename T>
   T		select(const std::string& selectQuery);

   /**
	* \fn T		select(const std::string& selectQuery)
	* \brief Retreive an instance of T class into the database, with condition
	* \param selectQuery 
	* \param condition 
	* \return An instance
	*/
   template <typename T, typename U>
   T		select(const std::string& selectQuery, const U& condition);

   /**
	* \fn std::vector<T>		selects(const std::string& selectQuery)
	* \brief Retreive instances of T class into the database
	* \param selectQuery 
	* \return Instances
	*/
   template <typename T>
   std::vector<T>		selects(const std::string& selectQuery);

   /**
	* \fn T		selects(const std::string& selectQuery)
	* \brief Retreive an instance of T class into the database, with condition
	* \param selectQuery 
	* \param condition 
	* \return Instances
	*/
   template <typename T, typename U>
   std::vector<T>		selects(const std::string& selectQuery, const U& condition);

   /**
	* \fn void     update(const std::string& updateQuery, T& model)
	* \brief Update an instance of T class into the database
	* \param updateQuery 
	* \param model
	*/
   template <typename T>
   void     update(const std::string& updateQuery, T& model);

   /**
	* \fn void     insert(const std::string& insertQuery, T& model)
	* \brief Insert an instance of T class into the database
	* \param insertQuery 
	* \param model
	*/
   template <typename T>
   void     insert(const std::string& insertQuery, T& model);

private:
	DatabaseConnector(const DatabaseConnector& databaseConnector);
	DatabaseConnector&	operator=(const DatabaseConnector& databaseConnector);
   soci::session&	getSession();

   soci::connection_pool	m_poolConnection; /*!< poolConnection*/
   int						m_numberOfConnection; /*!< sizeOfPoolConnection*/
};

#include "DatabaseConnector.inl"