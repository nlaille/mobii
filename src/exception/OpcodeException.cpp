#include "OpcodeException.h"

OpcodeException::OpcodeException(const std::string& message)
   : std::runtime_error(message)
{
}

OpcodeException::~OpcodeException() throw ()
{
}