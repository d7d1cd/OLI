#include "CORE/string.h"
#include "GOOGLE/gtest.h"



//***** string ********************************************************************************************************
namespace string {

TEST(string, compile_time)
{
  using namespace std::tr1;
  typedef ibmi::string<10> string_t;

  // ibmi::string<0> string; // Это не должно компилироваться

  static_assert(is_same<string_t::value_type, char>::value, "");
  static_assert(is_same<string_t::traits_type, std::char_traits<char>>::value, "");
  static_assert(is_same<string_t::size_type, std::size_t>::value, "");
  static_assert(is_same<string_t::pointer, char*>::value, "");
  static_assert(is_same<string_t::const_pointer, const char*>::value, "");
  static_assert(is_same<string_t::reference, char&>::value, "");
  static_assert(is_same<string_t::const_reference, const char&>::value, "");
  static_assert(is_same<string_t::iterator, char*>::value, "");
  static_assert(is_same<string_t::const_iterator, const char*>::value, "");
  static_assert(is_same<string_t::reverse_iterator, std::reverse_iterator<char*>>::value, "");
  static_assert(is_same<string_t::const_reverse_iterator, std::reverse_iterator<const char*>>::value, "");
  static_assert(is_same<string_t::difference_type, std::ptrdiff_t>::value, "");
  static_assert(string_t::blank == '\x40', "");
  static_assert(sizeof(string_t) == 10, "");
}


TEST(string, data)
{
  typedef ibmi::string<10> istr_t;
  istr_t istr;
  const istr_t cistr;

  ASSERT_EQ(istr.data(), reinterpret_cast<istr_t::pointer>(&istr));
  ASSERT_EQ(cistr.data(), reinterpret_cast<istr_t::const_pointer>(&cistr));
}


TEST(string, capacity)
{
  ibmi::string<10> istr;
  const ibmi::string<15> cistr;

  ASSERT_EQ(istr.capacity(), 10);
  ASSERT_EQ(cistr.capacity(), 15);
}


TEST(string, begin)
{
  typedef ibmi::string<10> istr_t;
  istr_t istr;
  const istr_t cistr;

  ASSERT_EQ(istr.begin(), istr.data());
  ASSERT_EQ(istr.cbegin(), istr.data());
  ASSERT_EQ(cistr.begin(), cistr.data());
  ASSERT_EQ(cistr.cbegin(), cistr.data());
}


TEST(string, end)
{
  typedef ibmi::string<10> istr_t;
  istr_t istr;
  const istr_t cistr;

  ASSERT_EQ(istr.end(), istr.data() + istr.capacity());
  ASSERT_EQ(istr.cend(), istr.data() + istr.capacity());
  ASSERT_EQ(cistr.end(), cistr.data() + cistr.capacity());
  ASSERT_EQ(cistr.cend(), cistr.data() + cistr.capacity());
}


TEST(string, rbegin)
{
  typedef ibmi::string<10> istr_t;
  istr_t istr;
  const istr_t cistr;

  ASSERT_EQ(&(*istr.rbegin()), istr.data() + istr.capacity() - 1);
  ASSERT_EQ(&(*istr.crbegin()), istr.data() + istr.capacity() - 1);
  ASSERT_EQ(&(*cistr.rbegin()), cistr.data() + cistr.capacity() - 1);
  ASSERT_EQ(&(*cistr.crbegin()), cistr.data() + cistr.capacity() - 1);
}


TEST(string, rend)
{
  typedef ibmi::string<10> istr_t;
  istr_t istr;
  const istr_t cistr;

  ASSERT_EQ(&(*istr.rend()), istr.data() - 1);
  ASSERT_EQ(&(*istr.crend()), istr.data() - 1);
  ASSERT_EQ(&(*cistr.rend()), cistr.data() - 1);
  ASSERT_EQ(&(*cistr.crend()), cistr.data() - 1);
}


TEST(string, assign)
{
  { // Копирование по указателю и размеру
    ibmi::string<5> istr;

    // Размер источника равен размеру строки
    char src5[] = { '1', '2', 'a', 'b', 'C' };
    istr.assign(src5, 5);
    ASSERT_TRUE(std::equal(istr.begin(), istr.end(), src5));

    // Размер источника большше размера строки
    char src7[] = { 'a', 'b', 'c', 'D', 'E', 'F', '0' };
    istr.assign(src7, 7);
    ASSERT_TRUE(std::equal(istr.begin(), istr.end(), src7));

    // Размер источника меньше размера строки
    char src2[] = { 'ж', 'о' };
    istr.assign(src2, 2);
    ASSERT_TRUE(std::equal(stl::begin(src2), stl::end(src2), istr.begin()));
    ASSERT_EQ(std::count(istr.begin(), istr.end(), istr.blank), 3);
  }
}


TEST(string, constructors)
{
  const char raw[5] = { '1', '2', 'a', 'b', 'C' };

  { // Конструктор по умолчанию
    ibmi::string<10> istr;
    ASSERT_EQ(std::distance(istr.begin(), istr.end()), 10);
    ASSERT_EQ(std::count(istr.begin(), istr.end(), istr.blank), 10);
  }
}


TEST(string, length)
{
  const char raw[5] = { '1', '2', 'a', 'b', 'C' };

  ASSERT_EQ(ibmi::string<5>().length(), 0);                // Пустая строка
  ASSERT_EQ(ibmi::string<9>().assign(raw, 5).length(), 5); // Частично заполненная строка
  ASSERT_EQ(ibmi::string<5>().assign(raw, 5).length(), 5); // Полностью заполненная строка
}
} // namespace string {




//***** string_ref *****************************************************************************************************
namespace string_ref {

TEST(string_ref, compile_time)
{
  using namespace std::tr1;
  typedef ibmi::string_ref string_ref_t;

  static_assert(is_same<string_ref_t::value_type, char>::value, "");
  static_assert(is_same<string_ref_t::traits_type, std::char_traits<char>>::value, "");
  static_assert(is_same<string_ref_t::size_type, std::size_t>::value, "");
  static_assert(is_same<string_ref_t::pointer, char*>::value, "");
  static_assert(is_same<string_ref_t::const_pointer, const char*>::value, "");
  static_assert(is_same<string_ref_t::reference, char&>::value, "");
  static_assert(is_same<string_ref_t::const_reference, const char&>::value, "");
  static_assert(is_same<string_ref_t::iterator, char*>::value, "");
  static_assert(is_same<string_ref_t::const_iterator, const char*>::value, "");
  static_assert(is_same<string_ref_t::reverse_iterator, std::reverse_iterator<char*>>::value, "");
  static_assert(is_same<string_ref_t::const_reverse_iterator, std::reverse_iterator<const char*>>::value, "");
  static_assert(is_same<string_ref_t::difference_type, std::ptrdiff_t>::value, "");
  static_assert(string_ref_t::blank == '\x40', "");
}


TEST(string_ref, data)
{
  typedef ibmi::string_ref ref_t;
  ref_t::value_type source[10] = {};
  ref_t ref(source);
  const ref_t cref(source);

  ASSERT_EQ(ref.data(), reinterpret_cast<ref_t::pointer>(source));
  ASSERT_EQ(cref.data(), reinterpret_cast<ref_t::const_pointer>(source));
}


TEST(string_ref, capacity)
{
  typedef ibmi::string_ref ref_t;
  ref_t::value_type source[10] = {};
  ref_t ref(source);
  const ref_t cref(source);

  ASSERT_EQ(ref.capacity(), 10);
  ASSERT_EQ(cref.capacity(), 10);
}


TEST(string_ref, begin)
{
  typedef ibmi::string_ref ref_t;
  ref_t::value_type source[10] = {};
  ref_t ref(source);
  const ref_t cref(source);

  ASSERT_EQ(ref.begin(), source);
  ASSERT_EQ(ref.cbegin(), source);
  ASSERT_EQ(cref.begin(), source);
  ASSERT_EQ(cref.cbegin(), source);
}


TEST(string_ref, end)
{
  typedef ibmi::string_ref ref_t;
  ref_t::value_type source[10] = {};
  ref_t ref(source);
  const ref_t cref(source);

  ASSERT_EQ(ref.end(), source + sizeof(source));
  ASSERT_EQ(ref.cend(), source + sizeof(source));
  ASSERT_EQ(cref.end(), source + sizeof(source));
  ASSERT_EQ(cref.cend(), source + sizeof(source));
}


TEST(string_ref, rbegin)
{
 typedef ibmi::string_ref ref_t;
  ref_t::value_type source[10] = {};
  ref_t ref(source);
  const ref_t cref(source);

  ASSERT_EQ(&(*ref.rbegin()), source + sizeof(source) - 1);
  ASSERT_EQ(&(*ref.crbegin()), source + sizeof(source) - 1);
  ASSERT_EQ(&(*cref.rbegin()), source + sizeof(source) - 1);
  ASSERT_EQ(&(*cref.crbegin()), source + sizeof(source) - 1);
}


TEST(string_ref, rend)
{
  typedef ibmi::string_ref ref_t;
  ref_t::value_type source[10] = {};
  ref_t ref(source);
  const ref_t cref(source);

  ASSERT_EQ(&(*ref.rend()), source - 1);
  ASSERT_EQ(&(*ref.crend()), source - 1);
  ASSERT_EQ(&(*cref.rend()), source - 1);
  ASSERT_EQ(&(*cref.crend()), source - 1);
}


TEST(string_ref, assign)
{
  { // Копирование по указателю и размеру
    char dst[5] = {};
    ibmi::string_ref iref(dst);

    // Размер источника равен размеру строки
    char src5[] = { '1', '2', 'a', 'b', 'C' };
    iref.assign(src5, 5);
    ASSERT_TRUE(std::equal(stl::begin(dst), stl::end(dst), src5));

    // Размер источника большше размера строки
    char src7[] = { 'a', 'b', 'c', 'D', 'E', 'F', '0' };
    iref.assign(src7, 7);
    ASSERT_TRUE(std::equal(stl::begin(dst), stl::end(dst), src7));

    // Размер источника меньше размера строки
    char src2[] = { 'ж', 'о' };
    iref.assign(src2, 2);
    ASSERT_TRUE(std::equal(stl::begin(src2), stl::end(src2), stl::begin(dst)));
    ASSERT_EQ(std::count(stl::begin(dst), stl::end(dst), iref.blank), 3);
  }
}


TEST(string_ref, constructors)
{
  // Конструктора по умолчанию быть не может, так как класс является ссылкой и обязан ссылатся на строку

  { // Конструктор из массива
    char raw[5] = { '1', '2', 'a', 'b', 'C' };
    ibmi::string_ref ref(raw);
    const ibmi::string_ref cref(raw);

    ASSERT_EQ(std::distance(ref.begin(), ref.end()), 5);
    ASSERT_TRUE(std::equal(ref.begin(), ref.end(), stl::begin(raw)));
    ASSERT_EQ(std::distance(cref.begin(), cref.end()), 5);
    ASSERT_TRUE(std::equal(cref.begin(), cref.end(), stl::begin(raw)));

    ibmi::string_ref iref(raw, true);
    ASSERT_EQ(std::count(stl::begin(raw), stl::end(raw), iref.blank), 5);
  }
}


TEST(string_ref, length)
{
  char empty[] = { ibmi::string_ref::blank };
  char part[]  = { '1', '2', 'a', 'b', ibmi::string_ref::blank };
  char full[]  = { '1', '2', 'a', 'b' };

  ASSERT_EQ(ibmi::string_ref(empty).length(), 0); // Пустая строка
  ASSERT_EQ(ibmi::string_ref(part).length(), 4);  // Частично заполненная строка
  ASSERT_EQ(ibmi::string_ref(full).length(), 4);  // Полностью заполненная строка
}
} // namespace string_ref {




//***** string_view **************************************************************************************************1*
namespace string_view {

TEST(string_view, compile_time)
{
  using namespace std::tr1;
  typedef ibmi::string_view string_view_t;

  static_assert(is_same<string_view_t::value_type, char>::value, "");
  static_assert(is_same<string_view_t::traits_type, std::char_traits<char>>::value, "");
  static_assert(is_same<string_view_t::size_type, std::size_t>::value, "");
  static_assert(is_same<string_view_t::pointer, char*>::value, "");
  static_assert(is_same<string_view_t::const_pointer, const char*>::value, "");
  static_assert(is_same<string_view_t::reference, char&>::value, "");
  static_assert(is_same<string_view_t::const_reference, const char&>::value, "");
  static_assert(is_same<string_view_t::iterator, char*>::value, "");
  static_assert(is_same<string_view_t::const_iterator, const char*>::value, "");
  static_assert(is_same<string_view_t::reverse_iterator, std::reverse_iterator<char*>>::value, "");
  static_assert(is_same<string_view_t::const_reverse_iterator, std::reverse_iterator<const char*>>::value, "");
  static_assert(is_same<string_view_t::difference_type, std::ptrdiff_t>::value, "");
  static_assert(string_view_t::blank == '\x40', "");
}


TEST(string_view, data)
{
  typedef ibmi::string_view view_t;

  {
    view_t::value_type src[10] = {};
    view_t view(src);
    const view_t cview(src);

    ASSERT_EQ(view.data(), reinterpret_cast<view_t::const_pointer>(src));
    ASSERT_EQ(cview.data(), reinterpret_cast<view_t::const_pointer>(src));
  }

  {
    const view_t::value_type src[15] = {};
    view_t view(src);
    const view_t cview(src);

    ASSERT_EQ(view.data(), reinterpret_cast<view_t::const_pointer>(src));
    ASSERT_EQ(cview.data(), reinterpret_cast<view_t::const_pointer>(src));
  }
}


TEST(string_view, capacity)
{
  typedef ibmi::string_view view_t;

  {
    view_t::value_type src[10] = {};
    view_t view(src);
    const view_t cview(src);

    ASSERT_EQ(view.capacity(), 10);
    ASSERT_EQ(cview.capacity(), 10);
  }

  {
    const view_t::value_type src[15] = {};
    view_t view(src);
    const view_t cview(src);

    ASSERT_EQ(view.capacity(), 15);
    ASSERT_EQ(cview.capacity(), 15);
  }
}


TEST(string_view, begin)
{
  typedef ibmi::string_view view_t;

  {
    view_t::value_type src[10] = {};
    view_t view(src);
    const view_t cview(src);

    ASSERT_EQ(view.begin(), src);
    ASSERT_EQ(view.cbegin(), src);
    ASSERT_EQ(cview.begin(), src);
    ASSERT_EQ(cview.cbegin(), src);
  }

  {
    const view_t::value_type src[15] = {};
    view_t view(src);
    const view_t cview(src);

    ASSERT_EQ(view.begin(), src);
    ASSERT_EQ(view.cbegin(), src);
    ASSERT_EQ(cview.begin(), src);
    ASSERT_EQ(cview.cbegin(), src);
  }
}


TEST(string_view, end)
{
  typedef ibmi::string_view view_t;

  {
    view_t::value_type src[10] = {};
    view_t view(src);
    const view_t cview(src);

    ASSERT_EQ(view.end(), src + sizeof(src));
    ASSERT_EQ(view.cend(), src + sizeof(src));
    ASSERT_EQ(cview.end(), src + sizeof(src));
    ASSERT_EQ(cview.cend(), src + sizeof(src));
  }

  {
    const view_t::value_type src[15] = {};
    view_t view(src);
    const view_t cview(src);

    ASSERT_EQ(view.end(), src + sizeof(src));
    ASSERT_EQ(view.cend(), src + sizeof(src));
    ASSERT_EQ(cview.end(), src + sizeof(src));
    ASSERT_EQ(cview.cend(), src + sizeof(src));
  }
}


TEST(string_view, rbegin)
{
  typedef ibmi::string_view view_t;

  {
    view_t::value_type src[10] = {};
    view_t view(src);
    const view_t cview(src);

    ASSERT_EQ(&(*view.rbegin()), src + sizeof(src) - 1);
    ASSERT_EQ(&(*view.crbegin()), src + sizeof(src) - 1);
    ASSERT_EQ(&(*cview.rbegin()), src + sizeof(src) - 1);
    ASSERT_EQ(&(*cview.crbegin()), src + sizeof(src) - 1);
  }

  {
    const view_t::value_type src[15] = {};
    view_t view(src);
    const view_t cview(src);

    ASSERT_EQ(&(*view.rbegin()), src + sizeof(src) - 1);
    ASSERT_EQ(&(*view.crbegin()), src + sizeof(src) - 1);
    ASSERT_EQ(&(*cview.rbegin()), src + sizeof(src) - 1);
    ASSERT_EQ(&(*cview.crbegin()), src + sizeof(src) - 1);
  }
}


TEST(string_view, rend)
{
  typedef ibmi::string_view view_t;

  {
    view_t::value_type src[10] = {};
    view_t view(src);
    const view_t cview(src);

    ASSERT_EQ(&(*view.rend()), src - 1);
    ASSERT_EQ(&(*view.crend()), src - 1);
    ASSERT_EQ(&(*cview.rend()), src - 1);
    ASSERT_EQ(&(*cview.crend()), src - 1);
  }

  {
    const view_t::value_type src[15] = {};
    view_t view(src);
    const view_t cview(src);

    ASSERT_EQ(&(*view.rend()), src - 1);
    ASSERT_EQ(&(*view.crend()), src - 1);
    ASSERT_EQ(&(*cview.rend()), src - 1);
    ASSERT_EQ(&(*cview.crend()), src - 1);
  }
}


TEST(string_view, constructors)
{
  // Конструктора по умолчанию нет по аналогии с string_ref

  { // Конструктор из константного массива
    const char raw[5] = { '1', '2', 'a', 'b', 'C' };
    ibmi::string_view view(raw);
    const ibmi::string_view cview(raw);

    ASSERT_EQ(std::distance(view.begin(), view.end()), 5);
    ASSERT_TRUE(std::equal(view.begin(), view.end(), stl::begin(raw)));
    ASSERT_EQ(std::distance(cview.begin(), cview.end()), 5);
    ASSERT_TRUE(std::equal(cview.begin(), cview.end(), stl::begin(raw)));
  }
}


TEST(string_view, length)
{
  char empty[] = { ibmi::string_view::blank };
  char part[]  = { '1', '2', 'a', 'b', ibmi::string_view::blank };
  char full[]  = { '1', '2', 'a', 'b' };

  ASSERT_EQ(ibmi::string_view(empty).length(), 0); // Пустая строка
  ASSERT_EQ(ibmi::string_view(part).length(), 4);  // Частично заполненная строка
  ASSERT_EQ(ibmi::string_view(full).length(), 4);  // Полностью заполненная строка
}
} // namespace string_view {

