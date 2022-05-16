#include "istring"
#include "GOOGLE/gtest.h"



//**********************************************************************************************************************
namespace xxx {

TEST(ISTRING, simple_test)
{
  oli::istring<10> istring;
}
} // namespace xxx {




//**********************************************************************************************************************
#ifdef DEBUG
int main(int argc, char** argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
#endif

