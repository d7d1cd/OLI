#include "type_trait"
#include "utility"
#include "GTEST/gtest.h"


//**********************************************************************************************************************
TEST(STL, remove_reference)
{
  bool v; // Почему без этой переменной не работает объяснено тут https://t.me/supapro/1077025

  ASSERT_TRUE((v = std::tr1::is_same<int, int>::value));
  ASSERT_FALSE((v = std::tr1::is_same<int, int&>::value));
  ASSERT_FALSE((v = std::tr1::is_same<int, int&&>::value));

  ASSERT_TRUE((v = std::tr1::is_same<int, stl::remove_reference<int>::type>::value));
  ASSERT_TRUE((v = std::tr1::is_same<int, stl::remove_reference<int&>::type>::value));
  ASSERT_TRUE((v = std::tr1::is_same<int, stl::remove_reference<int&&>::type>::value));
}




//**********************************************************************************************************************
// Код теста forward честно скопипизжен отсюда:
// https://github.com/llvm/llvm-project/blob/main/libcxx/test/std/utilities/utility/forward/forward.pass.cpp
struct A {};
A source() {return A();}
const A csource() {return A();}

TEST(STL, forward)
{
  A a;
  const A ca = A();

  static_assert(std::tr1::is_same<decltype(stl::forward<A&>(a)), A&>::value, "");
  static_assert(std::tr1::is_same<decltype(stl::forward<A>(a)), A&&>::value, "");
  static_assert(std::tr1::is_same<decltype(stl::forward<A>(source())), A&&>::value, "");
  stl::forward<A&>(a);
  stl::forward<A>(a);
  stl::forward<A>(source());

  static_assert(std::tr1::is_same<decltype(stl::forward<const A&>(a)), const A&>::value, "");
  static_assert(std::tr1::is_same<decltype(stl::forward<const A>(a)), const A&&>::value, "");
  static_assert(std::tr1::is_same<decltype(stl::forward<const A>(source())), const A&&>::value, "");
  stl::forward<const A&>(a);
  stl::forward<const A>(a);
  stl::forward<const A>(source());

  static_assert(std::tr1::is_same<decltype(stl::forward<const A&>(ca)), const A&>::value, "");
  static_assert(std::tr1::is_same<decltype(stl::forward<const A>(ca)), const A&&>::value, "");
  static_assert(std::tr1::is_same<decltype(stl::forward<const A>(csource())), const A&&>::value, "");
  stl::forward<const A&>(ca);
  stl::forward<const A>(ca);
  stl::forward<const A>(csource());

  constexpr int i2 = stl::forward<int>(42);
  static_assert(stl::forward<int>(42) == 42, "" );
  static_assert(stl::forward<const int&>(i2) == 42, "");
}




//**********************************************************************************************************************
#ifdef DEBUG
int main(int argc, char** argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
#endif

