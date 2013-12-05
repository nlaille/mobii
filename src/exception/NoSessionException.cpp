#include "NoSessionException.h"

NoSessionException::NoSessionException(const std::string& message)
   : std::runtime_error(message)
{
}

NoSessionException::~NoSessionException() throw ()
{
}