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
   UnableToLock(const std::string& message);
   virtual ~UnableToLock() throw ();
};