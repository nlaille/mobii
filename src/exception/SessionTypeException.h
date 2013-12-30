#pragma once

/**
 * \file SessionTypeException.h
 * \brief Declaration of SessionTypeException
 * \author Nicolas Laille
 * \version 1.0
 * \date 24 Octobre 2013
 */
#include <stdexcept>

/**
 * \class SessionTypeException
 * \brief Exception throw when session is bad type in IProtocolHandler
 */
class SessionTypeException : public std::runtime_error
{
public:
   /**
    * \fn SessionTypeException(const std::string& message)
	* \brief Constructor
	* \param message
	*/
   SessionTypeException(const std::string& message);

   /**
    * \fn ~SessionTypeException()
	* \brief Destructor
	*/
   virtual ~SessionTypeException() throw ();
};