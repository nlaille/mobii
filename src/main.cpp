#include <iostream>
#include <string>
#include <vector>

#include "Server.h"
#include "database/DatabaseConnector.h"
#include "database/Model.h"

#ifndef		MOBII_TEST_MACRO 
int main()
{
   try
   {
      Server   s;

      s.init();
      s.start();
   }
   catch (const std::exception& e)
   {
      std::cerr << e.what() << std::endl;
   }
   return 0;
}
#else

#include <gtest/gtest.h>
#include <gmock/gmock.h>

int main(int argc, char** argv) {
  // The following line causes Google Mock to throw an exception on failure,
  // which will be interpreted by your testing framework as a test failure.
  ::testing::GTEST_FLAG(throw_on_failure) = true;
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}
#endif