//#include "popl"
#include "GTEST/gtest.h"


TEST(POPL, simple_test)
{
}


#ifdef DEBUG
int main(int argc, char** argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
#endif

