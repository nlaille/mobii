#pragma once

/**
 * \file SessionNotFound.h
 * \brief Declaration of SessionNotFound
 * \author Nicolas Laille
 * \version 1.0
 * \date 24 Octobre 2013
 */
#include <stdexcept>

/**
 * \class SessionNotFound
 * \brief Exception throw when Session not found in SessionManager
 */
class SessionNotFound : public std::runtime_error
{
public:
   SessionNotFound(const std::string& message);
   virtual ~SessionNotFound() throw ();
};