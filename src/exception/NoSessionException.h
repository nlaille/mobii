#pragma once

/**
 * \file NoSessionException.h
 * \brief Declaration of NoSessionException
 * \author Nicolas Laille
 * \version 1.0
 * \date 24 Octobre 2013
 */
#include <stdexcept>

/**
 * \class NoSessionException
 * \brief Exception throw when NoSession method called
 */
class NoSessionException : public std::runtime_error
{
public:
   NoSessionException(const std::string& message);
   virtual ~NoSessionException() throw ();
};