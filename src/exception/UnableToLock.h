#pragma once

/**
 * \file UnableToLock.h
 * \brief Declaration of UnableToLock
 * \author Nicolas Laille
 * \version 1.0
 * \date 24 Octobre 2013
 */
#include <stdexcept>

/**
 * \class UnableToLock
 * \brief Exception throw when IProtocolHandler is unable to lock session with ScopeLockSession
 */
class UnableToLock : public std::runtime_error
{
public:
   /**
    * \fn UnableToLock(const std::string& message)
	* \brief Constructor
	* \param message
	*/
   UnableToLock(const std::string& message);

   /**
    * \fn ~UnableToLock()
	* \brief Destructor
	*/
   virtual ~UnableToLock() throw ();
};