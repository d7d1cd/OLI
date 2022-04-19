#include "cxxopts"
#include "GTEST/gtest.h"


TEST(CXXOPTS, simple_test)
{
//  cxxopts::Options options("test", "A brief description");
}


#ifdef DEBUG
int main(int argc, char** argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
#endif

