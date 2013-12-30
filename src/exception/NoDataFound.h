#pragma once

/**
 * \file NoDataFound.h
 * \brief Declaration of NoDataFound
 * \author Nicolas Laille
 * \version 1.0
 * \date 24 Octobre 2013
 */
#include <stdexcept>

/**
 * \class NoDataFound
 * \brief Exception throw when No Data Found by DatabaseConnector
 */
class NoDataFound : public std::runtime_error
{
public:
   /**
    * \fn NoDataFound(const std::string& message)
	* \brief Constructor
	* \param message
	*/
   NoDataFound(const std::string& message);

   /**
    * \fn ~NoDataFound()
	* \brief Destructor
	*/
   virtual ~NoDataFound() throw ();
};