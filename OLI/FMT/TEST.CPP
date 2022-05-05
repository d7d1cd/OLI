#include "format"
#include "GOOGLE/gtest.h"
#include "GOOGLE/gmock.h"




namespace format { //***************************************************************************************************

using fmt::StringRef;
using fmt::CStringRef;
using fmt::MemoryWriter;


// Format value using the standard library.
template <typename Char, typename T>
void std_format(const T &value, std::basic_string<Char> &result) {
  std::basic_ostringstream<Char> os;
  os << value;
  result = os.str();
}


// Checks if writing value to BasicWriter<Char> produces the same result
// as writing it to std::basic_ostringstream<Char>.
template <typename Char, typename T>
::testing::AssertionResult check_write(const T &value, const char *type)
{
  std::basic_string<Char> actual =
      (fmt::BasicMemoryWriter<Char>() << value).str();
  std::basic_string<Char> expected;
  std_format(value, expected);
  if (expected == actual)
    return ::testing::AssertionSuccess();
  return ::testing::AssertionFailure()
      << "Value of: (Writer<" << type << ">() << value).str()\n"
      << "  Actual: " << actual << "\n"
      << "Expected: " << expected << "\n";
}


struct AnyWriteChecker
{
  template <typename T>
  ::testing::AssertionResult operator()(const char *, const T &value) const {
    ::testing::AssertionResult result = check_write<char>(value, "char");
    return result ? check_write<wchar_t>(value, "wchar_t") : result;
  }
};


// Checks if writing value to BasicWriter produces the same result
// as writing it to std::ostringstream both for char and wchar_t.
#define CHECK_WRITE(value) EXPECT_PRED_FORMAT1(AnyWriteChecker(), value)


TEST(FMT_StringRef, Ctor) {
  EXPECT_STREQ("abc", StringRef("abc").data());
  EXPECT_EQ(3u, StringRef("abc").size());

  EXPECT_STREQ("defg", StringRef(std::string("defg")).data());
  EXPECT_EQ(4u, StringRef(std::string("defg")).size());
}

TEST(FMT_StringRef, ConvertToString) {
  std::string s = StringRef("abc").to_string();
  EXPECT_EQ("abc", s);
}

TEST(CStringRefTest, Ctor) {
  EXPECT_STREQ("abc", CStringRef("abc").c_str());
  EXPECT_STREQ("defg", CStringRef(std::string("defg")).c_str());
}

TEST(FMT_MemoryWriter, Ctor) {
  MemoryWriter w;
  EXPECT_EQ(0u, w.size());
  EXPECT_STREQ("", w.c_str());
  EXPECT_EQ("", w.str());
}

TEST(FMT_MemoryWriter, Data) {
  MemoryWriter w;
  w << 42;
  EXPECT_EQ("42", std::string(w.data(), w.size()));
}

TEST(FMT_MemoryWriter, WriteWithoutArgs) {
  MemoryWriter w;
  w.write("test");
  EXPECT_EQ("test", std::string(w.data(), w.size()));
}

TEST(FMT_MemoryWriter, WriteInt) {
  CHECK_WRITE(42);
  CHECK_WRITE(-42);
  CHECK_WRITE(static_cast<short>(12));
  CHECK_WRITE(34u);
  CHECK_WRITE(std::numeric_limits<int>::min());
  CHECK_WRITE(std::numeric_limits<int>::max());
  CHECK_WRITE(std::numeric_limits<unsigned>::max());
}

TEST(FMT_MemoryWriter, WriteLong) {
  CHECK_WRITE(56l);
  CHECK_WRITE(78ul);
  CHECK_WRITE(std::numeric_limits<long>::min());
  CHECK_WRITE(std::numeric_limits<long>::max());
  CHECK_WRITE(std::numeric_limits<unsigned long>::max());
}

TEST(FMT_MemoryWriter, WriteLongLong) {
  CHECK_WRITE(56ll);
  CHECK_WRITE(78ull);
  CHECK_WRITE(std::numeric_limits<long long>::min());
  CHECK_WRITE(std::numeric_limits<long long>::max());
  CHECK_WRITE(std::numeric_limits<unsigned long long>::max());
}

TEST(FMT_MemoryWriter, WriteDouble) {
  CHECK_WRITE(4.2);
  CHECK_WRITE(-4.2);
  CHECK_WRITE(std::numeric_limits<double>::min());
  CHECK_WRITE(std::numeric_limits<double>::max());
}

} // namespace format {




#ifdef DEBUG
int main(int argc, char** argv)
{
  std::cout << (fmt::BasicMemoryWriter<char>() << 4.2).str();
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
#endif

