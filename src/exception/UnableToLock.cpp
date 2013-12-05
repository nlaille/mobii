#include "UnableToLock.h"

UnableToLock::UnableToLock(const std::string& message)
   : std::runtime_error(message)
{
}

UnableToLock::~UnableToLock() throw ()
{
}