#include "STL/type_trait.h"
#include "STL/utility.h"
#include "STL/memory.h"
#include "STL/map.h"
#include "STL/iterator.h"
#include "GOOGLE/gtest.h"
#include <cctype>
#include <vector>



//**********************************************************************************************************************
namespace remove_reference {

TEST(STL, remove_reference)
{
  static_assert(std::tr1::is_same<int, int>::value, "");
  static_assert(!std::tr1::is_same<int, int&>::value, "");
  static_assert(!std::tr1::is_same<int, int&&>::value, "");

  static_assert(std::tr1::is_same<int, stl::remove_reference<int>::type>::value, "");
  static_assert(std::tr1::is_same<int, stl::remove_reference<int&>::type>::value, "");
  static_assert(std::tr1::is_same<int, stl::remove_reference<int&&>::type>::value, "");
}
} // namespace remove_reference {




//**********************************************************************************************************************
namespace enable_if {

template <typename T>
typename stl::enable_if<std::tr1::is_same<T, int>::value, T>::type
foo(T t) { return t + 1; }

template <typename T>
typename stl::enable_if<std::tr1::is_same<T, char>::value, T>::type
foo(T t) { return std::tolower(t); }

TEST(STL, enable_if)
{
  static_assert(std::tr1::is_same<int, stl::enable_if<true, int>::type>::value, "");

  ASSERT_EQ(foo(7), 8);
  ASSERT_EQ(foo('A'), 'a');
}
} // namespace enable_if {




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
namespace make_shared {

struct C
{
  C() : c('A'), i(7) {}
  C(char c) : c(c), i(8) {}
  C(char c, int i) : c(c), i(i) {}
  char c;
  int i;
};

TEST(STL, make_shared)
{
  auto sp1 = stl::make_shared<C>();
  ASSERT_EQ(sp1->c, 'A');
  ASSERT_EQ(sp1->i, 7);

  auto sp2 = stl::make_shared<C>('B');
  ASSERT_EQ(sp2->c, 'B');
  ASSERT_EQ(sp2->i, 8);

  auto sp3 = stl::make_shared<C>('C', 9);
  ASSERT_EQ(sp3->c, 'C');
  ASSERT_EQ(sp3->i, 9);

  static_assert(std::tr1::is_same<decltype(sp1), std::tr1::shared_ptr<C>>::value, "");
  static_assert(std::tr1::is_same<decltype(sp2), std::tr1::shared_ptr<C>>::value, "");
  static_assert(std::tr1::is_same<decltype(sp3), std::tr1::shared_ptr<C>>::value, "");
}
} // namespace make_shared {




//**********************************************************************************************************************
// https://godbolt.org/z/YKGxe1j97
//
namespace map_operator_square_brackets {

int defctor = 0;
int intctor = 0;
int copy = 0;
int move = 0;
int destroy = 0;

void init() { defctor = intctor = copy = move = destroy = 0; }

struct type
{
  int i;
  type()                : i(0)     { ++defctor; }
  type(int i)           : i(i)     { ++intctor; }
  type(const type& src) : i(src.i) { ++copy; }
  type(type&& src)      : i(src.i) { ++move; }
  ~type()                          { ++destroy; }
};

TEST(STL, map_operator_square_brackets)
{
  { // Демонстрация ошибки в std::map
    std::map<int, type> map;

    init();
    map[0] = type();
    ASSERT_EQ(defctor, 2);
    ASSERT_EQ(intctor, 0);
    ASSERT_EQ(copy, 2);
    ASSERT_EQ(move, 0);
    ASSERT_EQ(destroy, 3);

    init();
    int i = map[0].i;
    ASSERT_EQ(defctor, 1);             // Зачем здесь вызывается конструктор по умолчанию???
    ASSERT_EQ(intctor, 0);
    ASSERT_EQ(copy, 1);
    ASSERT_EQ(move, 0);
    ASSERT_EQ(destroy, 2);
    ASSERT_EQ(i, 0);

    init();
    map[0] = type(777);
    ASSERT_EQ(defctor, 1);             // Зачем здесь вызывается конструктор по умолчанию???
    ASSERT_EQ(intctor, 1);
    ASSERT_EQ(copy, 1);
    ASSERT_EQ(move, 0);
    ASSERT_EQ(destroy, 3);
    ASSERT_EQ(map[0].i, 777);

    init();
  }
  ASSERT_EQ(defctor, 0);
  ASSERT_EQ(intctor, 0);
  ASSERT_EQ(copy, 0);
  ASSERT_EQ(move, 0);
  ASSERT_EQ(destroy, 1);


  { // Тестирование stl::map
    stl::map<int, type> map;

    init();
    map[0] = type();                   // При добавлении элемента поведение аналогично стандартной реализации
    ASSERT_EQ(defctor, 2);
    ASSERT_EQ(intctor, 0);
    ASSERT_EQ(copy, 2);
    ASSERT_EQ(move, 0);
    ASSERT_EQ(destroy, 3);

    init();
    int i = map[0].i;                  // Поиск элемента по ключу
    ASSERT_EQ(defctor, 0);             // Не вызывается конструктор по умолчанию
    ASSERT_EQ(intctor, 0);
    ASSERT_EQ(copy, 0);                // Не вызывается конструктор копии
    ASSERT_EQ(move, 0);
    ASSERT_EQ(destroy, 0);             // Не вызывается ни один деструктор
    ASSERT_EQ(i, 0);

    init();
    map[0] = type(777);                // Замена элемента по ключу
    ASSERT_EQ(defctor, 0);             // Не вызывается конструктор по умолчанию
    ASSERT_EQ(intctor, 1);
    ASSERT_EQ(copy, 0);                // Не вызывается конструктор копии
    ASSERT_EQ(move, 0);
    ASSERT_EQ(destroy, 1);             // Деструктор вызывается только для заменяемого элемента
    ASSERT_EQ(map[0].i, 777);

    init();
  }
  ASSERT_EQ(defctor, 0);
  ASSERT_EQ(intctor, 0);
  ASSERT_EQ(copy, 0);
  ASSERT_EQ(move, 0);
  ASSERT_EQ(destroy, 1);
}
} // namespace map_operator_square_brackets {




//**********************************************************************************************************************
namespace map_at {

TEST(STL, map_at)
{
  stl::map<int, int> m;
  m.insert(std::make_pair(1, 1));
  m.insert(std::make_pair(2, 2));
  ASSERT_EQ(m.at(1), 1);
  ASSERT_EQ(m.at(2), 2);
  m.at(2) = 3;
  ASSERT_EQ(m.at(2), 3);


  try {
    m.at(3);
    FAIL() << "Expected std::out_of_range";
  }
  catch (std::out_of_range const & err) {
    EXPECT_EQ(err.what(), std::string("invalid map<K, T> key"));
  }
  catch (...) {
    FAIL() << "Expected std::out_of_range";
  }


  const stl::map<int, int> cm(m);
  ASSERT_EQ(cm.at(1), 1);
  ASSERT_EQ(cm.at(2), 3);

  try {
    cm.at(3);
    FAIL() << "Expected std::out_of_range";
  }
  catch (std::out_of_range const & err) {
    EXPECT_EQ(err.what(), std::string("invalid map<K, T> key"));
  }
  catch (...) {
    FAIL() << "Expected std::out_of_range";
  }
}
} // namespace map_at {





//**********************************************************************************************************************
namespace begin {

TEST(STL, begin)
{
  constexpr S = 5;
  int arr[S] = {};
  const char carr[S] = {};
  std::vector<int> vec(S, 0);
  const std::vector<char> cvec(S, 0);

  ASSERT_EQ(stl::begin(arr), &arr[0]);
  ASSERT_EQ(stl::begin(carr), &carr[0]);
  ASSERT_EQ(stl::begin(vec), vec.begin());
  ASSERT_EQ(stl::begin(cvec), cvec.begin());
}
} // namespace begin {




//**********************************************************************************************************************
namespace end {

TEST(STL, end)
{
  constexpr S = 5;
  int arr[S] = {};
  const char carr[S] = {};
  std::vector<int> vec(S, 0);
  const std::vector<char> cvec(S, 0);

  ASSERT_EQ(stl::end(arr), &arr[0] + S);
  ASSERT_EQ(stl::end(carr), &carr[0] + S);
  ASSERT_EQ(stl::end(vec), vec.end());
  ASSERT_EQ(stl::end(cvec), cvec.end());
}
} // namespace end {




//**********************************************************************************************************************
namespace next {

TEST(STL, next)
{
  int array[5] = { 1, 2, 3, 4, 5 };

  ASSERT_EQ(*stl::next(array), 2);
  ASSERT_EQ(*stl::next(array, 0), 1);
  ASSERT_EQ(*stl::next(array, 4), 5);

  auto it = stl::next(array, 4);
  ASSERT_EQ(*stl::next(it, -1), 4);
}
} // namespace next {




//**********************************************************************************************************************
namespace prev {

TEST(STL, prev)
{
  int array[5] = { 1, 2, 3, 4, 5 };
  auto last = array + 4;

  ASSERT_EQ(*stl::prev(last), 4);
  ASSERT_EQ(*stl::prev(last, 0), 5);
  ASSERT_EQ(*stl::prev(last, 4), 1);

  auto it = stl::prev(last, 4);
  ASSERT_EQ(*stl::prev(it, -1), 2);
}
} // namespace prev {

