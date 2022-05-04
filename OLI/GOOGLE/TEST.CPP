#include "gtest"

TEST(GTEST, simple_test)
{
  EXPECT_EQ(0, 0);
}

#ifdef DEBUG
int main(int argc, char** argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
#endif

