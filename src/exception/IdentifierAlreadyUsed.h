#pragma once

/**
 * \file IdentifierAlreadyUsed.h
 * \brief Declaration of IdentifierAlreadyUsed
 * \author Nicolas Laille
 * \version 1.0
 * \date 24 Octobre 2013
 */

#include <stdexcept>

/**
 * \class IdentifierAlreadyUsed
 * \brief Exception throw when UID already exists
 */
class IdentifierAlreadyUsed : public std::runtime_error
{
public:
   IdentifierAlreadyUsed(const std::string& message);
   virtual ~IdentifierAlreadyUsed() throw ();
};