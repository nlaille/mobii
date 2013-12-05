#include "NoDataFound.h"

NoDataFound::NoDataFound(const std::string& message)
   : std::runtime_error(message)
{
}

NoDataFound::~NoDataFound() throw ()
{
}