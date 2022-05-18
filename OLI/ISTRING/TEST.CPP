#include "istring"
#include "GOOGLE/gtest.h"



//***** ISTRING ********************************************************************************************************
namespace istring {

TEST(istring, compile_time)
{
  using namespace std::tr1;
  typedef ibm::istring<10> istring_t;

  // ibm::istring<0> istring; // Это не должно компилироваться

  static_assert(is_same<istring_t::value_type, char>::value, "");
  static_assert(is_same<istring_t::traits_type, std::char_traits<char>>::value, "");
  static_assert(is_same<istring_t::size_type, std::size_t>::value, "");
  static_assert(is_same<istring_t::pointer, char*>::value, "");
  static_assert(is_same<istring_t::const_pointer, const char*>::value, "");
  static_assert(is_same<istring_t::reference, char&>::value, "");
  static_assert(is_same<istring_t::const_reference, const char&>::value, "");
  static_assert(is_same<istring_t::iterator, char*>::value, "");
  static_assert(is_same<istring_t::const_iterator, const char*>::value, "");
  static_assert(is_same<istring_t::reverse_iterator, std::reverse_iterator<char*>>::value, "");
  static_assert(is_same<istring_t::const_reverse_iterator, std::reverse_iterator<const char*>>::value, "");
  static_assert(is_same<istring_t::difference_type, std::ptrdiff_t>::value, "");
  static_assert(istring_t::blank == '\x40', "");
  static_assert(sizeof(istring_t) == 10, "");
}


TEST(istring, data)
{
  typedef ibm::istring<10> istr_t;
  istr_t istr;
  const istr_t cistr;

  ASSERT_EQ(istr.data(), reinterpret_cast<istr_t::pointer>(&istr));
  ASSERT_EQ(cistr.data(), reinterpret_cast<istr_t::const_pointer>(&cistr));
}


TEST(istring, capacity)
{
  ibm::istring<10> istr;
  const ibm::istring<15> cistr;

  ASSERT_EQ(istr.capacity(), 10);
  ASSERT_EQ(cistr.capacity(), 15);
}


TEST(istring, begin)
{
  typedef ibm::istring<10> istr_t;
  istr_t istr;
  const istr_t cistr;

  ASSERT_EQ(istr.begin(), istr.data());
  ASSERT_EQ(istr.cbegin(), istr.data());
  ASSERT_EQ(cistr.begin(), cistr.data());
  ASSERT_EQ(cistr.cbegin(), cistr.data());
}


TEST(istring, end)
{
  typedef ibm::istring<10> istr_t;
  istr_t istr;
  const istr_t cistr;

  ASSERT_EQ(istr.end(), istr.data() + istr.capacity());
  ASSERT_EQ(istr.cend(), istr.data() + istr.capacity());
  ASSERT_EQ(cistr.end(), cistr.data() + cistr.capacity());
  ASSERT_EQ(cistr.cend(), cistr.data() + cistr.capacity());
}


TEST(istring, rbegin)
{
  typedef ibm::istring<10> istr_t;
  istr_t istr;
  const istr_t cistr;

  ASSERT_EQ(&(*istr.rbegin()), istr.data() + istr.capacity() - 1);
  ASSERT_EQ(&(*istr.crbegin()), istr.data() + istr.capacity() - 1);
  ASSERT_EQ(&(*cistr.rbegin()), cistr.data() + cistr.capacity() - 1);
  ASSERT_EQ(&(*cistr.crbegin()), cistr.data() + cistr.capacity() - 1);
}


TEST(istring, rend)
{
  typedef ibm::istring<10> istr_t;
  istr_t istr;
  const istr_t cistr;

  ASSERT_EQ(&(*istr.rend()), istr.data() - 1);
  ASSERT_EQ(&(*istr.crend()), istr.data() - 1);
  ASSERT_EQ(&(*cistr.rend()), cistr.data() - 1);
  ASSERT_EQ(&(*cistr.crend()), cistr.data() - 1);
}


TEST(istring, constructors)
{
  { // Конструктор по умолчанию
    ibm::istring<10> istr;
    ASSERT_EQ(std::distance(istr.begin(), istr.end()), 10);
    ASSERT_EQ(std::count(istr.begin(), istr.end(), istr.blank), 10);
  }
}
} // namespace istring {




//***** ISTRING_REF *****************************************************************************************************
namespace istring_ref {

TEST(istring_ref, compile_time)
{
  using namespace std::tr1;
  typedef ibm::istring_ref istring_ref_t;

  static_assert(is_same<istring_ref_t::value_type, char>::value, "");
  static_assert(is_same<istring_ref_t::traits_type, std::char_traits<char>>::value, "");
  static_assert(is_same<istring_ref_t::size_type, std::size_t>::value, "");
  static_assert(is_same<istring_ref_t::pointer, char*>::value, "");
  static_assert(is_same<istring_ref_t::const_pointer, const char*>::value, "");
  static_assert(is_same<istring_ref_t::reference, char&>::value, "");
  static_assert(is_same<istring_ref_t::const_reference, const char&>::value, "");
  static_assert(is_same<istring_ref_t::iterator, char*>::value, "");
  static_assert(is_same<istring_ref_t::const_iterator, const char*>::value, "");
  static_assert(is_same<istring_ref_t::reverse_iterator, std::reverse_iterator<char*>>::value, "");
  static_assert(is_same<istring_ref_t::const_reverse_iterator, std::reverse_iterator<const char*>>::value, "");
  static_assert(is_same<istring_ref_t::difference_type, std::ptrdiff_t>::value, "");
  static_assert(istring_ref_t::blank == '\x40', "");
}


TEST(istring_ref, data)
{
  typedef ibm::istring_ref ref_t;
  ref_t::value_type source[10] = {};
  ref_t ref(source);
  const ref_t cref(source);

  ASSERT_EQ(ref.data(), reinterpret_cast<ref_t::pointer>(source));
  ASSERT_EQ(cref.data(), reinterpret_cast<ref_t::const_pointer>(source));
}


TEST(istring_ref, capacity)
{
  ibm::istring_ref::value_type src1[10] = {};
  ibm::istring_ref::value_type src2[15] = {};

  ASSERT_EQ(ibm::istring_ref(src1).capacity(), 10);
  ASSERT_EQ(ibm::istring_ref(src2).capacity(), 15);
}


TEST(istring_ref, begin)
{
  typedef ibm::istring_ref ref_t;
  ref_t::value_type source[10] = {};
  ref_t ref(source);
  const ref_t cref(source);

  ASSERT_EQ(ref.begin(), source);
  ASSERT_EQ(ref.cbegin(), source);
  ASSERT_EQ(cref.begin(), source);
  ASSERT_EQ(cref.cbegin(), source);
}


TEST(istring_ref, end)
{
 typedef ibm::istring_ref ref_t;
  ref_t::value_type source[10] = {};
  ref_t ref(source);
  const ref_t cref(source);

  ASSERT_EQ(ref.end(), source + sizeof(source));
  ASSERT_EQ(ref.cend(), source + sizeof(source));
  ASSERT_EQ(cref.end(), source + sizeof(source));
  ASSERT_EQ(cref.cend(), source + sizeof(source));
}


TEST(istring_ref, rbegin)
{
 typedef ibm::istring_ref ref_t;
  ref_t::value_type source[10] = {};
  ref_t ref(source);
  const ref_t cref(source);

  ASSERT_EQ(&(*ref.rbegin()), source + sizeof(source) - 1);
  ASSERT_EQ(&(*ref.crbegin()), source + sizeof(source) - 1);
  ASSERT_EQ(&(*cref.rbegin()), source + sizeof(source) - 1);
  ASSERT_EQ(&(*cref.crbegin()), source + sizeof(source) - 1);
}


TEST(istring_ref, rend)
{
  typedef ibm::istring_ref ref_t;
  ref_t::value_type source[10] = {};
  ref_t ref(source);
  const ref_t cref(source);

  ASSERT_EQ(&(*ref.rend()), source - 1);
  ASSERT_EQ(&(*ref.crend()), source - 1);
  ASSERT_EQ(&(*cref.rend()), source - 1);
  ASSERT_EQ(&(*cref.crend()), source - 1);
}


TEST(istring_ref, constructors)
{
  // Конструктора по умолчанию быть не может, так как класс является ссылкой и обязан ссылатся на строку

  { // Конструктор из пары итераторов
    char raw[5] = { '1', '2', 'a', 'b', 'C' };
    ibm::istring_ref iref(stl::begin(raw), stl::end(raw));
    ASSERT_EQ(std::distance(iref.begin(), iref.end()), 5);
    ASSERT_TRUE(std::equal(iref.begin(), iref.end(), stl::begin(raw)));
  }

  { // Конструктор из массива с очисткой
    char raw[5] = { '1', '2', 'a', 'b', 'C' };
    ibm::istring_ref iref(raw, true);
    ASSERT_EQ(std::count(iref.begin(), iref.end(), iref.blank), 5);
  }
}
} // namespace istring_ref {




//**********************************************************************************************************************
#ifdef DEBUG
int main(int argc, char** argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
#endif

