#include "SessionNotFound.h"

SessionNotFound::SessionNotFound(const std::string& message)
   : std::runtime_error(message)
{
}

SessionNotFound::~SessionNotFound() throw ()
{
}