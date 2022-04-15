#include "type_trait"
#include "utility"
#include "GTEST/gtest.h"


//**********************************************************************************************************************
namespace remove_reference {

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
} // namespace remove_reference {




//**********************************************************************************************************************
// Код теста forward честно скопипизжен отсюда:
// https://github.com/llvm/llvm-project/blob/main/libcxx/test/std/utilities/utility/forward/forward.pass.cpp

namespace forward {

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
} // namespace forward {




//**********************************************************************************************************************
// Код теста move честно скопипизжен отсюда:
// https://github.com/llvm/llvm-project/blob/main/libcxx/test/std/utilities/utility/forward/move.pass.cpp

namespace move {

class move_only
{
  move_only(const move_only&);
  move_only& operator=(const move_only&);
  public:
  move_only(move_only&&) {}
  move_only& operator=(move_only&&) {return *this;}
  move_only() {}
};

move_only source() {return move_only();}

void test(move_only) {}

int x = 42;
const int& cx = x;

template <class QualInt>
QualInt get() { return static_cast<QualInt>(x); }

int copy_ctor = 0;
int move_ctor = 0;

struct B
{
  B() {}
  B(const B&) {++copy_ctor;}
  B(B&&) {++move_ctor;}
  B& operator=(const B&) = delete;
};

TEST(STL, move)
{
  { // Test return type and noexcept.
    static_assert(std::tr1::is_same<decltype(stl::move(x)), int&&>::value, "");
    stl::move(x);
    static_assert(std::tr1::is_same<decltype(stl::move(cx)), const int&&>::value, "");
    stl::move(cx);
    static_assert(std::tr1::is_same<decltype(stl::move(42)), int&&>::value, "");
    stl::move(42);
    static_assert(std::tr1::is_same<decltype(stl::move(get<const int&&>())), const int&&>::value, "");
    stl::move(get<int const&&>());
  }

  { // test copy and move semantics
    B b;
    const B cb;

    ASSERT_EQ(copy_ctor, 0);
    ASSERT_EQ(move_ctor, 0);

    B b2 = b;
    ASSERT_EQ(copy_ctor, 1);
    ASSERT_EQ(move_ctor, 0);

    B b3 = stl::move(b);
    ASSERT_EQ(copy_ctor, 1);
    ASSERT_EQ(move_ctor, 1);

    B b4 = cb;
    ASSERT_EQ(copy_ctor, 2);
    ASSERT_EQ(move_ctor, 1);

    B b5 = stl::move(cb);
    ASSERT_EQ(copy_ctor, 3);
    ASSERT_EQ(move_ctor, 1);
  }

  { // test on a move only type
    move_only mo;
    test(stl::move(mo));
    test(source());
  }

  { // Test that std::move is constexpr in C++11
    constexpr int y = 42;
    static_assert(stl::move(y) == 42, "");
  }
}
} // namespace move




//**********************************************************************************************************************
namespace conditional {

TEST(STL, conditional)
{
  static_assert(std::tr1::is_same<stl::conditional<true, int, double>::type, int>::value, "");
  static_assert(std::tr1::is_same<stl::conditional<false, int, double>::type, double>::value, "");
}
} // namespace conditional {




//**********************************************************************************************************************
#ifdef DEBUG
int main(int argc, char** argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
#endif

