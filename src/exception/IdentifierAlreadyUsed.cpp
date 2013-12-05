#include "IdentifierAlreadyUsed.h"

IdentifierAlreadyUsed::IdentifierAlreadyUsed(const std::string& message)
   : std::runtime_error(message)
{
}

IdentifierAlreadyUsed::~IdentifierAlreadyUsed() throw ()
{
}