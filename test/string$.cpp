#include "ICORE/string.h"
#include "GOOGLE/gtest.h"



//**********************************************************************************************************************
namespace ibmi_string {

TEST(string, compile_time)
{
  using namespace std::tr1;
  typedef ibmi::string<10> string_t;

//   ibmi::string<0> string; // Это не должно компилироваться

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
  static_assert(string_t::capacity == 10, "");
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

  ASSERT_EQ(istr.end(), istr.data() + istr.capacity);
  ASSERT_EQ(istr.cend(), istr.data() + istr.capacity);
  ASSERT_EQ(cistr.end(), cistr.data() + cistr.capacity);
  ASSERT_EQ(cistr.cend(), cistr.data() + cistr.capacity);
}


TEST(string, rbegin)
{
  typedef ibmi::string<10> istr_t;
  istr_t istr;
  const istr_t cistr;

  ASSERT_EQ(&(*istr.rbegin()), istr.data() + istr.capacity - 1);
  ASSERT_EQ(&(*istr.crbegin()), istr.data() + istr.capacity - 1);
  ASSERT_EQ(&(*cistr.rbegin()), cistr.data() + cistr.capacity - 1);
  ASSERT_EQ(&(*cistr.crbegin()), cistr.data() + cistr.capacity - 1);
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
  ibmi::string<5> str;

  { // Копирование по указателю и размеру (основная перегрузка assign)
    const char* src = "Sometext";

    str.assign(src, 5); // Размер источника равен размеру строки
    ASSERT_TRUE(std::equal(str.begin(), str.end(), src));

    str.assign(src, 7); // Размер источника больше размера строки
    ASSERT_TRUE(std::equal(str.begin(), str.end(), src));

    str.assign(src, 2); // Размер источника меньше размера строки
    ASSERT_TRUE(std::equal(src, src + 2, str.begin()));
    ASSERT_EQ(std::count(str.begin(), str.end(), str.blank), 3);
  }

  { // Копирование из любого наследника strign_base_const
    ibmi::string<3> src;
    src.assign("boo");
    str.assign(src);
    ASSERT_TRUE(std::equal(src.begin(), src.end(), str.begin()));
    ASSERT_EQ(std::count(str.begin(), str.end(), str.blank), 2);
  }

  { // Копирование из С строки
    const char* c_str = "boo";
    str.assign(c_str);
    ASSERT_TRUE(std::equal(c_str, c_str + std::strlen(c_str), str.begin()));
    ASSERT_EQ(std::count(str.begin(), str.end(), str.blank), 2);
  }

  { // Копирование из стандартной строки
    std::string stdstr = "boomerang";
    str.assign(stdstr);
    ASSERT_TRUE(std::equal(str.begin(), str.end(), stdstr.begin()));
    ASSERT_EQ(std::count(str.begin(), str.end(), str.blank), 0);
  }

  { // Копирование из ibm sso строки
    ibm::sso_string ssostr = "sso string";
    str.assign(ssostr);
    ASSERT_TRUE(std::equal(str.begin(), str.end(), "sso s"));
  }
}


TEST(string, assignment_operator)
{
  ibmi::string<5> str;

  { // Присваивание из любого наследника strign_base_const
    ibmi::string<3> src;
    src.assign("boo");
    str = src;
    ASSERT_TRUE(std::equal(src.begin(), src.end(), str.begin()));
    ASSERT_EQ(std::count(str.begin(), str.end(), str.blank), 2);
  }

  { // Присваивание из C строки
    const char* src = "c-style";
    str = src;
    ASSERT_TRUE(std::equal(str.begin(), str.end(), src));
    ASSERT_EQ(std::count(str.begin(), str.end(), str.blank), 0);
  }

  { // Присваивание из стандартной строки
    std::string src = "std";
    str = src;
    ASSERT_TRUE(std::equal(src.begin(), src.end(), str.begin()));
    ASSERT_EQ(std::count(str.begin(), str.end(), str.blank), 2);
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

  { // Конструктор от любого наследника string_base_const
    ibmi::string<5> str("zoo");
    ibmi::string<4> copy(str);
    ASSERT_EQ(copy.str(), "zoo");
    ASSERT_EQ(std::count(copy.begin(), copy.end(), copy.blank), 1);
  }

  { // Конструктор от С строки
    ibmi::string<10> str1("Hello");
    ASSERT_EQ(std::memcmp(str1.begin(), "Hello     ", str1.capacity), 0);

    ibmi::string<3> str2("Hello");
    ASSERT_EQ(std::memcmp(str2.begin(), "Hel", str2.capacity), 0);

    ibmi::string<5> str3("Hello");
    ASSERT_EQ(std::memcmp(str3.begin(), "Hello", str3.capacity), 0);

    ibmi::string<5> str4("");
    ASSERT_EQ(std::memcmp(str4.begin(), "     ", str4.capacity), 0);

    ibmi::string<5> str5(nullptr);
    ASSERT_EQ(std::memcmp(str5.begin(), "     ", str5.capacity), 0);
  }

  { // Конструктор от стандартной строки
    std::string stdstr = "hello";

    ibmi::string<5> str1(stdstr);
    ASSERT_EQ(std::memcmp(str1.begin(), &stdstr[0], 5), 0);

    ibmi::string<7> str2(stdstr);
    ASSERT_EQ(std::memcmp(str2.begin(), &stdstr[0], 5), 0);
    ASSERT_EQ(std::count(str2.begin(), str2.end(), str2.blank), 2);

    ibmi::string<3> str3(stdstr);
    ASSERT_EQ(std::memcmp(str3.begin(), &stdstr[0], 3), 0);

    ibmi::string<5> str4((std::string()));
    ASSERT_EQ(std::count(str4.begin(), str4.end(), str4.blank), 5);
  }
}


TEST(string, copy_constructor)
{
  ibmi::string<5> str("zoo");
  ibmi::string<5> copy(str);
  ASSERT_EQ(str.str(), copy.str());
}


TEST(string, length)
{
  constexpr size_t string_size = 10 * 2 + 3;
  char raw[string_size] = {};
  std::fill(stl::begin(raw), stl::end(raw), 'A');
  ibmi::string<string_size> str;

  for (int i = 0; i < string_size; ++i) {
    str.assign(raw, i);
    ASSERT_EQ(str.length(), i);
  }
}


TEST(string, empty)
{
  ASSERT_TRUE(ibmi::string<42>().empty());
  ASSERT_FALSE(ibmi::string<17>("not empty").empty());
}


TEST(string, clear)
{
  ibmi::string<17> str("not empty");
  ASSERT_FALSE(str.empty());

  str.clear();
  ASSERT_TRUE(str.empty());

}


TEST(string, str)
{
  ibmi::string<10> str;
  ASSERT_EQ(std::string(""), str.str());

  str = "short";
  ASSERT_EQ(std::string("short"), str.str());

  str = "too long for ibmi::string<10>";
  ASSERT_EQ(std::string("too long f"), str.str());

  str = "equal twos";
  ASSERT_EQ(std::string("equal twos"), str.str());
}


TEST(string, cast_to_str)
{
  ibmi::string<10> str;
  std::string stdstr = str;
  ASSERT_EQ("", stdstr);

  str.assign("short");
  stdstr = str;
  ASSERT_EQ("short", stdstr);

  str.assign("too long for ibmi::string<10>");
  stdstr = str;
  ASSERT_EQ("too long f", stdstr);

  str.assign("equal twos");
  stdstr = str;
  ASSERT_EQ("equal twos", stdstr);
}


TEST(string, compare)
{
  { // Содержимое строк одинаково
    ibmi::string<5> str1("foo");
    ibmi::string<5> str2("foo");
    ibmi::string<3> str3("foo");
    ibmi::string<9> str4("foo");
    ASSERT_EQ(str1.compare(str2), 0);
    ASSERT_EQ(str1.compare(str3), 0);
    ASSERT_EQ(str1.compare(str4), 0);
  }

  { // Исходная строка меньше
    ibmi::string<5> str1("boo");
    ibmi::string<5> str2("foo");
    ibmi::string<3> str3("foo");
    ibmi::string<9> str4("foo");
    ASSERT_EQ(str1.compare(str2), -1);
    ASSERT_EQ(str1.compare(str3), -1);
    ASSERT_EQ(str1.compare(str4), -1);
  }

  { // Исходная строка больше
    ibmi::string<5> str1("zoo");
    ibmi::string<5> str2("foo");
    ibmi::string<3> str3("foo");
    ibmi::string<9> str4("foo");
    ASSERT_EQ(str1.compare(str2), 1);
    ASSERT_EQ(str1.compare(str3), 1);
    ASSERT_EQ(str1.compare(str4), 1);
  }

  { // Разное
    ASSERT_EQ(ibmi::string<5>("boo").compare(ibmi::string<5>("boo m")), -1);
    ASSERT_EQ(ibmi::string<7>("boo").compare(ibmi::string<5>("boo m")), -1);
    ASSERT_EQ(ibmi::string<5>("boo").compare(ibmi::string<7>("boo m")), -1);
    ASSERT_EQ(ibmi::string<5>("boo m").compare(ibmi::string<7>("boo")), 1);
    ASSERT_EQ(ibmi::string<7>("boo m").compare(ibmi::string<5>("boo m")), 0);
  }
}
} // namespace ibmi_string {




//**********************************************************************************************************************
namespace ibmi_string_ref {

TEST(string_ref, compile_time)
{
  // Закомментированный код не должен компилироваться
  // char arr[5];
  // ibmi::string_ref<sizeof(arr) + 1> ref(arr);
}


TEST(string_ref, constructors)
{
  // Конструктора по умолчанию быть не может, так как класс является ссылкой и обязан ссылатся на строку

  { // Конструктор от массива символов
    char raw[5] = { '1', '2', 'a', 'b', 'C' };
    ibmi::string_ref<sizeof(raw)> ref(raw);
    ASSERT_TRUE(std::equal(ref.begin(), ref.end(), stl::begin(raw)));

    const char* c_str = "abracadabra";
    ref = c_str;
    ASSERT_TRUE(std::equal(stl::begin(raw), stl::end(raw), c_str));

    ibmi::string_ref<sizeof(raw)> ref2(raw, true);
    ASSERT_EQ(std::count(stl::begin(raw), stl::end(raw), ref2.blank), sizeof(raw));
  }

  { // Конструктор от любого наследника strign_base
    ibmi::string<42> str("some long text");
    ibmi::string_ref<42> ref(str);

    std::string stdstr = "another long text";
    ref = stdstr;
    ASSERT_EQ(str.str(), stdstr);

    ibmi::string_view<42> v(str);
    // ibmi::string_ref<42> r(v); Это не должно компилироваться
  }
}


TEST(string_ref, copy_constructor)
{
  ibmi::string<42> str("boo");
  ibmi::string_ref<42> ref(str);
  ibmi::string_ref<42> ref2(ref);

  ref2 = "some text";
  ASSERT_EQ(str.str(), "some text");
}


TEST(string_ref, assignment_operator)
{
  ibmi::string<5> str1, str2;
  ibmi::string_ref<5> ref1(str1), ref2(str2);

  ref1 = "zoo";
  ref2 = ref1;
  ASSERT_TRUE(std::equal(str1.begin(), str1.end(), str2.begin()));
}
} // namespace ibmi_string_ref {




//**********************************************************************************************************************
namespace ibmi_string_view {

TEST(string_ref, compile_time)
{
  // Закомментированный код не должен компилироваться
   char arr[5];
//   ibmi::string_view<sizeof(arr) + 1> view(arr);
}


TEST(string_view, constructors)
{
  // Конструктора по умолчанию нет по аналогии с string_ref

  { // Конструктор из константного массива
    const char arr[5] = { '1', '2', 'a', 'b', 'C' };
    ibmi::string_view<sizeof(arr)> view(arr);
    ASSERT_TRUE(std::equal(view.begin(), view.end(), stl::begin(arr)));
  }

  { // Конструктор от любого наследника string_base_const
    ibmi::string<42> str("some text");
    ibmi::string_view<42> view(str);
    ASSERT_TRUE(std::equal(view.begin(), view.end(), str.begin()));

    ibmi::string_ref<42> ref(str);
    ibmi::string_view<42> view2(ref);
    ASSERT_TRUE(std::equal(view2.begin(), view2.end(), str.begin()));
  }
}


TEST(string_view, copy_constructor)
{
  ibmi::string<42> str("boo");
  ibmi::string_view<42> view(str);
  ibmi::string_view<42> view2(view);
  ASSERT_TRUE(std::equal(view2.begin(), view2.end(), str.begin()));
}
} // namespace ibmi_string_view {





//**********************************************************************************************************************
namespace ibmi_string_free_functions {

TEST(string, operator_plus)
{
  { // string + string
    ibmi::string<5> str1("foo");
    ibmi::string<4> str2("boo");
    auto s = str1 + str2;
    ASSERT_EQ(s.capacity, 9);
    ASSERT_TRUE(std::equal(s.begin(), s.end(), "foo  boo "));
  }

  { // string + string_ref
    ibmi::string<7> str("foo");
    ibmi::string_ref<4> ref(str);
    auto s = str + ref;
    ASSERT_EQ(s.capacity, 11);
    ASSERT_TRUE(std::equal(s.begin(), s.end(), "foo    foo "));
  }
}


TEST(string, comparison_operators)
{
  ASSERT_TRUE(ibmi::string<5>("boo") == ibmi::string<5>("boo"));
  ASSERT_TRUE(ibmi::string<5>("zoo") != ibmi::string<5>("boo"));
  ASSERT_TRUE(ibmi::string<5>("boo") <  ibmi::string<5>("zoo"));
  ASSERT_TRUE(ibmi::string<5>("zoo") >  ibmi::string<5>("boo"));
  ASSERT_TRUE(ibmi::string<5>("boo") <= ibmi::string<5>("boo"));
  ASSERT_TRUE(ibmi::string<5>("boo") <= ibmi::string<5>("zoo"));
  ASSERT_TRUE(ibmi::string<5>("boo") >= ibmi::string<5>("boo"));
  ASSERT_TRUE(ibmi::string<5>("zoo") >= ibmi::string<5>("boo"));
}
} // namespace ibmi_string_free_functions {
