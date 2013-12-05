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
   SessionTypeException(const std::string& message);
   virtual ~SessionTypeException() throw ();
};