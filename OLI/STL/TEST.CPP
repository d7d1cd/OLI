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
namespace make_signed {

TEST(STL, make_signed)
{
  typedef stl::make_signed<signed int>::type                  int1;
  typedef stl::make_signed<unsigned int>::type                int2;
  typedef stl::make_signed<volatile signed int>::type         volatile_int1;
  typedef stl::make_signed<volatile unsigned int>::type       volatile_int2;
  typedef stl::make_signed<const signed int>::type            const_int1;
  typedef stl::make_signed<const unsigned int>::type          const_int2;
  typedef stl::make_signed<const volatile signed int>::type   const_volatile_int1;
  typedef stl::make_signed<const volatile unsigned int>::type const_volatile_int2;

  static_assert(std::tr1::is_same<int1, signed int>::value, "");
  static_assert(std::tr1::is_same<int2, signed int>::value, "");
  static_assert(std::tr1::is_same<volatile_int1, volatile signed int>::value, "");
  static_assert(std::tr1::is_same<volatile_int2, volatile signed int>::value, "");
  static_assert(std::tr1::is_same<const_int1, const signed int>::value, "");
  static_assert(std::tr1::is_same<const_int2, const signed int>::value, "");
  static_assert(std::tr1::is_same<const_volatile_int1, const volatile signed int>::value, "");
  static_assert(std::tr1::is_same<const_volatile_int2, const volatile signed int>::value, "");

  static_assert(std::tr1::is_same<stl::make_signed<signed char>::type, signed char>::value, "");
  static_assert(std::tr1::is_same<stl::make_signed<unsigned char>::type, signed char>::value, "");
  static_assert(std::tr1::is_same<stl::make_signed<signed short>::type, signed short>::value, "");
  static_assert(std::tr1::is_same<stl::make_signed<unsigned short>::type, signed short>::value, "");
  static_assert(std::tr1::is_same<stl::make_signed<signed long>::type, signed long>::value, "");
  static_assert(std::tr1::is_same<stl::make_signed<unsigned long>::type, signed long>::value, "");
  static_assert(std::tr1::is_same<stl::make_signed<signed long long>::type, signed long long>::value, "");
  static_assert(std::tr1::is_same<stl::make_signed<unsigned long long>::type, signed long long>::value, "");
}
} // namespace make_signed {




//**********************************************************************************************************************
namespace make_unsigned {

TEST(STL, make_unsigned)
{
  typedef stl::make_unsigned<signed int>::type                  int1;
  typedef stl::make_unsigned<unsigned int>::type                int2;
  typedef stl::make_unsigned<volatile signed int>::type         volatile_int1;
  typedef stl::make_unsigned<volatile unsigned int>::type       volatile_int2;
  typedef stl::make_unsigned<const signed int>::type            const_int1;
  typedef stl::make_unsigned<const unsigned int>::type          const_int2;
  typedef stl::make_unsigned<const volatile signed int>::type   const_volatile_int1;
  typedef stl::make_unsigned<const volatile unsigned int>::type const_volatile_int2;

  static_assert(std::tr1::is_same<int1, unsigned int>::value, "");
  static_assert(std::tr1::is_same<int2, unsigned int>::value, "");
  static_assert(std::tr1::is_same<volatile_int1, volatile unsigned int>::value, "");
  static_assert(std::tr1::is_same<volatile_int2, volatile unsigned int>::value, "");
  static_assert(std::tr1::is_same<const_int1, const unsigned int>::value, "");
  static_assert(std::tr1::is_same<const_int2, const unsigned int>::value, "");
  static_assert(std::tr1::is_same<const_volatile_int1, const volatile unsigned int>::value, "");
  static_assert(std::tr1::is_same<const_volatile_int2, const volatile unsigned int>::value, "");

  static_assert(std::tr1::is_same<stl::make_unsigned<signed char>::type, unsigned char>::value, "");
  static_assert(std::tr1::is_same<stl::make_unsigned<unsigned char>::type, unsigned char>::value, "");
  static_assert(std::tr1::is_same<stl::make_unsigned<signed short>::type, unsigned short>::value, "");
  static_assert(std::tr1::is_same<stl::make_unsigned<unsigned short>::type, unsigned short>::value, "");
  static_assert(std::tr1::is_same<stl::make_unsigned<signed long>::type, unsigned long>::value, "");
  static_assert(std::tr1::is_same<stl::make_unsigned<unsigned long>::type, unsigned long>::value, "");
  static_assert(std::tr1::is_same<stl::make_unsigned<signed long long>::type, unsigned long long>::value, "");
  static_assert(std::tr1::is_same<stl::make_unsigned<unsigned long long>::type, unsigned long long>::value, "");
}
} // namespace make_unsigned {




//**********************************************************************************************************************
#ifdef DEBUG
int main(int argc, char** argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
#endif

