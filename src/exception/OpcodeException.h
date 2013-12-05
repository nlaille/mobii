#pragma once

/**
 * \file OpcodeException.h
 * \brief Declaration of OpcodeException
 * \author Nicolas Laille
 * \version 1.0
 * \date 24 Octobre 2013
 */
#include <stdexcept>

/**
 * \class OpcodeException
 * \brief Exception throw when Opcode doesn't exist
 */
class OpcodeException : public std::runtime_error
{
public:
   OpcodeException(const std::string& message);
   virtual ~OpcodeException() throw ();
};