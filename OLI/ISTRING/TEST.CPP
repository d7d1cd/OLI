#include "istring"
#include "GOOGLE/gtest.h"



//**********************************************************************************************************************
TEST(ISTRING, compile_time)
{
  using namespace std::tr1;
  using namespace ibm;

  // istring<0> istring; // Это не должно компилироваться

  static_assert(is_same<istring<10>::value_type, char>::value, "");
  static_assert(is_same<istring_dyn::value_type, char>::value, "");
  static_assert(is_same<istring_ref::value_type, char>::value, "");

  static_assert(is_same<istring<10>::size_type, std::size_t>::value, "");
  static_assert(is_same<istring_dyn::size_type, std::size_t>::value, "");
  static_assert(is_same<istring_ref::size_type, std::size_t>::value, "");

  static_assert(is_same<istring<10>::pointer, char*>::value, "");
  static_assert(is_same<istring_dyn::pointer, char*>::value, "");
  static_assert(is_same<istring_ref::pointer, char*>::value, "");

  static_assert(is_same<istring<10>::const_pointer, const char*>::value, "");
  static_assert(is_same<istring_dyn::const_pointer, const char*>::value, "");
  static_assert(is_same<istring_ref::const_pointer, const char*>::value, "");

  static_assert(is_same<istring<10>::reference, char&>::value, "");
  static_assert(is_same<istring_dyn::reference, char&>::value, "");
  static_assert(is_same<istring_ref::reference, char&>::value, "");

  static_assert(is_same<istring<10>::const_reference, const char&>::value, "");
  static_assert(is_same<istring_dyn::const_reference, const char&>::value, "");
  static_assert(is_same<istring_ref::const_reference, const char&>::value, "");

  static_assert(is_same<istring<10>::iterator, char*>::value, "");
  static_assert(is_same<istring_dyn::iterator, char*>::value, "");
  static_assert(is_same<istring_ref::iterator, char*>::value, "");

  static_assert(is_same<istring<10>::const_iterator, const char*>::value, "");
  static_assert(is_same<istring_dyn::const_iterator, const char*>::value, "");
  static_assert(is_same<istring_ref::const_iterator, const char*>::value, "");

  static_assert(is_same<istring<10>::reverse_iterator, std::reverse_iterator<char*>>::value, "");
  static_assert(is_same<istring_dyn::reverse_iterator, std::reverse_iterator<char*>>::value, "");
  static_assert(is_same<istring_ref::reverse_iterator, std::reverse_iterator<char*>>::value, "");

  static_assert(is_same<istring<10>::const_reverse_iterator, std::reverse_iterator<const char*>>::value, "");
  static_assert(is_same<istring_dyn::const_reverse_iterator, std::reverse_iterator<const char*>>::value, "");
  static_assert(is_same<istring_ref::const_reverse_iterator, std::reverse_iterator<const char*>>::value, "");

  static_assert(is_same<istring<10>::difference_type, std::ptrdiff_t>::value, "");
  static_assert(is_same<istring_dyn::difference_type, std::ptrdiff_t>::value, "");
  static_assert(is_same<istring_ref::difference_type, std::ptrdiff_t>::value, "");

  static_assert(istring<10>::blank == '\x40', "");
  static_assert(istring_dyn::blank == '\x40', "");
  static_assert(istring_ref::blank == '\x40', "");

  static_assert(sizeof(istring<42>) == 42, "");
}

TEST(ISTRING, capacity)
{
}




//**********************************************************************************************************************
#ifdef DEBUG
int main(int argc, char** argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
#endif

