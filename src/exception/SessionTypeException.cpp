#include "SessionTypeException.h"

SessionTypeException::SessionTypeException(const std::string& message)
   : std::runtime_error(message)
{
}

SessionTypeException::~SessionTypeException() throw ()
{
}