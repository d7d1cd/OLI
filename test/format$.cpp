#include "FMT/format.h"
#include "GOOGLE/gtest.h"
#include <cstdarg>
#include <cctype>




namespace format { //***************************************************************************************************

using fmt::StringRef;
using fmt::CStringRef;
using fmt::MemoryWriter;
using fmt::BasicWriter;
using fmt::pad;
using fmt::WMemoryWriter;
using fmt::format;
using fmt::FormatError;


enum {BUFFER_SIZE = 256};


template <std::size_t SIZE>
void safe_sprintf(char (&buffer)[SIZE], const char *format, ...) {
  std::va_list args;
  va_start(args, format);
  vsnprintf(buffer, SIZE, format, args);
  va_end(args);
}

// Increment a number in a string.
void increment(char *s) {
  for (int i = static_cast<int>(std::strlen(s)) - 1; i >= 0; --i) {
    if (s[i] != '9') {
      ++s[i];
      break;
    }
    s[i] = '0';
  }
}


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


template <typename Char>
struct WriteChecker {
  template <typename T>
  ::testing::AssertionResult operator()(const char *, const T &value) const {
    return check_write<Char>(value, "char");
  }
};


// Checks if writing value to BasicWriter produces the same result
// as writing it to std::ostringstream both for char and wchar_t.
#define CHECK_WRITE(value)       EXPECT_PRED_FORMAT1(AnyWriteChecker(), value)
#define CHECK_WRITE_CHAR(value)  EXPECT_PRED_FORMAT1(WriteChecker<char>(), value)
#define CHECK_WRITE_WCHAR(value) EXPECT_PRED_FORMAT1(WriteChecker<wchar_t>(), value)

#define FMT_TEST_THROW_(statement, expected_exception, expected_message, fail) \
  GTEST_AMBIGUOUS_ELSE_BLOCKER_ \
  if (::testing::AssertionResult gtest_ar = ::testing::AssertionSuccess()) { \
    std::string gtest_expected_message = expected_message; \
    bool gtest_caught_expected = false; \
    try { \
      GTEST_SUPPRESS_UNREACHABLE_CODE_WARNING_BELOW_(statement); \
    } \
    catch (expected_exception const& e) { \
      if (gtest_expected_message != e.what()) { \
        gtest_ar \
          << #statement " throws an exception with a different message.\n" \
          << "Expected: " << gtest_expected_message << "\n" \
          << "  Actual: " << e.what(); \
        goto GTEST_CONCAT_TOKEN_(gtest_label_testthrow_, __LINE__); \
      } \
      gtest_caught_expected = true; \
    } \
    catch (...) { \
      gtest_ar << \
          "Expected: " #statement " throws an exception of type " \
          #expected_exception ".\n  Actual: it throws a different type."; \
      goto GTEST_CONCAT_TOKEN_(gtest_label_testthrow_, __LINE__); \
    } \
    if (!gtest_caught_expected) { \
      gtest_ar << \
          "Expected: " #statement " throws an exception of type " \
          #expected_exception ".\n  Actual: it throws nothing."; \
      goto GTEST_CONCAT_TOKEN_(gtest_label_testthrow_, __LINE__); \
    } \
  } else \
    GTEST_CONCAT_TOKEN_(gtest_label_testthrow_, __LINE__): \
      fail(gtest_ar.failure_message())

// Tests that the statement throws the expected exception and the exception's
// what() method returns expected message.
#define EXPECT_THROW_MSG(statement, expected_exception, expected_message) \
  FMT_TEST_THROW_(statement, expected_exception, \
      expected_message, GTEST_NONFATAL_FAILURE_)

std::string format_system_error(int error_code, fmt::StringRef message) {
  fmt::MemoryWriter out;
  fmt::format_system_error(out, error_code, message);
  return out.str();
}

inline FILE *safe_fopen(const char *filename, const char *mode) {
  return std::fopen(filename, mode);
}

#define EXPECT_SYSTEM_ERROR(statement, error_code, message) \
  EXPECT_THROW_MSG(statement, fmt::SystemError, \
      format_system_error(error_code, message))


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

TEST(FMT_MemoryWriter, WriteLongDouble) {
  CHECK_WRITE(4.2l);
  CHECK_WRITE_CHAR(-4.2l);
  std::wstring str;
  std_format(4.2l, str);
  if (str[0] != '-')
    CHECK_WRITE_WCHAR(-4.2l);
  else
    fmt::print("warning: long double formatting with std::swprintf is broken");
  CHECK_WRITE(std::numeric_limits<long double>::min());
  CHECK_WRITE(std::numeric_limits<long double>::max());
}

TEST(FMT_MemoryWriter, WriteDoubleAtBufferBoundary) {
  MemoryWriter writer;
  for (int i = 0; i < 100; ++i)
    writer << 1.23456789;
}

TEST(FMT_MemoryWriter, WriteDoubleWithFilledBuffer) {
  MemoryWriter writer;
  // Fill the buffer.
  for (int i = 0; i < fmt::internal::INLINE_BUFFER_SIZE; ++i)
    writer << ' ';
  writer << 1.2;
  EXPECT_STREQ("1.2", writer.c_str() + fmt::internal::INLINE_BUFFER_SIZE);
}

TEST(FMT_MemoryWriter, WriteChar) {
  CHECK_WRITE('a');
}

TEST(FMT_MemoryWriter, WriteWideChar) {
  CHECK_WRITE_WCHAR(L'a');
}

TEST(FMT_MemoryWriter, WriteString) {
  CHECK_WRITE_CHAR("abc");
  CHECK_WRITE_WCHAR("abc");
  // The following line shouldn't compile:
  //MemoryWriter() << L"abc";
}

TEST(FMT_MemoryWriter, WriteWideString) {
  CHECK_WRITE_WCHAR(L"abc");
  // The following line shouldn't compile:
  //fmt::WMemoryWriter() << "abc";
}

TEST(FMT_MemoryWriter, bin) {
  using fmt::bin;
  EXPECT_EQ("1100101011111110", (MemoryWriter() << bin(0xcafe)).str());
  EXPECT_EQ("1011101010111110", (MemoryWriter() << bin(0xbabeu)).str());
  EXPECT_EQ("1101111010101101", (MemoryWriter() << bin(0xdeadl)).str());
  EXPECT_EQ("1011111011101111", (MemoryWriter() << bin(0xbeeful)).str());
  EXPECT_EQ("11001010111111101011101010111110",
            (MemoryWriter() << bin(0xcafebabell)).str());
  EXPECT_EQ("11011110101011011011111011101111",
            (MemoryWriter() << bin(0xdeadbeefull)).str());
}

TEST(FMT_MemoryWriter, oct) {
  using fmt::oct;
  EXPECT_EQ("12", (MemoryWriter() << oct(static_cast<short>(012))).str());
  EXPECT_EQ("12", (MemoryWriter() << oct(012)).str());
  EXPECT_EQ("34", (MemoryWriter() << oct(034u)).str());
  EXPECT_EQ("56", (MemoryWriter() << oct(056l)).str());
  EXPECT_EQ("70", (MemoryWriter() << oct(070ul)).str());
  EXPECT_EQ("1234", (MemoryWriter() << oct(01234ll)).str());
  EXPECT_EQ("5670", (MemoryWriter() << oct(05670ull)).str());
}

TEST(FMT_MemoryWriter, hex) {
  using fmt::hex;
  fmt::IntFormatSpec<int, fmt::TypeSpec<'x'> > (*phex)(int value) = hex;
  phex(42);
  // This shouldn't compile:
  //fmt::IntFormatSpec<short, fmt::TypeSpec<'x'> > (*phex2)(short value) = hex;

  EXPECT_EQ("cafe", (MemoryWriter() << hex(0xcafe)).str());
  EXPECT_EQ("babe", (MemoryWriter() << hex(0xbabeu)).str());
  EXPECT_EQ("dead", (MemoryWriter() << hex(0xdeadl)).str());
  EXPECT_EQ("beef", (MemoryWriter() << hex(0xbeeful)).str());
  EXPECT_EQ("cafebabe", (MemoryWriter() << hex(0xcafebabell)).str());
  EXPECT_EQ("deadbeef", (MemoryWriter() << hex(0xdeadbeefull)).str());
}

TEST(FMT_MemoryWriter, hexu) {
  using fmt::hexu;
  EXPECT_EQ("CAFE", (MemoryWriter() << hexu(0xcafe)).str());
  EXPECT_EQ("BABE", (MemoryWriter() << hexu(0xbabeu)).str());
  EXPECT_EQ("DEAD", (MemoryWriter() << hexu(0xdeadl)).str());
  EXPECT_EQ("BEEF", (MemoryWriter() << hexu(0xbeeful)).str());
  EXPECT_EQ("CAFEBABE", (MemoryWriter() << hexu(0xcafebabell)).str());
  EXPECT_EQ("DEADBEEF", (MemoryWriter() << hexu(0xdeadbeefull)).str());
}

class Date {
  int year_, month_, day_;
 public:
  Date(int year, int month, int day) : year_(year), month_(month), day_(day) {}

  int year() const { return year_; }
  int month() const { return month_; }
  int day() const { return day_; }
};

template <typename Char>
BasicWriter<Char> &operator<<(BasicWriter<Char> &f, const Date &d) {
  return f << d.year() << '-' << d.month() << '-' << d.day();
}

class ISO8601DateFormatter {
 const Date *date_;

public:
  ISO8601DateFormatter(const Date &d) : date_(&d) {}

  template <typename Char>
  friend BasicWriter<Char> &operator<<(
      BasicWriter<Char> &w, const ISO8601DateFormatter &d) {
    return w << pad(d.date_->year(), 4, '0') << '-'
        << pad(d.date_->month(), 2, '0') << '-' << pad(d.date_->day(), 2, '0');
  }
};

ISO8601DateFormatter iso8601(const Date &d) { return ISO8601DateFormatter(d); }

TEST(FMT_MemoryWriter, pad) {
  using fmt::hex;
  EXPECT_EQ("    cafe", (MemoryWriter() << pad(hex(0xcafe), 8)).str());
  EXPECT_EQ("    babe", (MemoryWriter() << pad(hex(0xbabeu), 8)).str());
  EXPECT_EQ("    dead", (MemoryWriter() << pad(hex(0xdeadl), 8)).str());
  EXPECT_EQ("    beef", (MemoryWriter() << pad(hex(0xbeeful), 8)).str());
  EXPECT_EQ("    dead", (MemoryWriter() << pad(hex(0xdeadll), 8)).str());
  EXPECT_EQ("    beef", (MemoryWriter() << pad(hex(0xbeefull), 8)).str());

  EXPECT_EQ("     11", (MemoryWriter() << pad(11, 7)).str());
  EXPECT_EQ("     22", (MemoryWriter() << pad(22u, 7)).str());
  EXPECT_EQ("     33", (MemoryWriter() << pad(33l, 7)).str());
  EXPECT_EQ("     44", (MemoryWriter() << pad(44ul, 7)).str());
  EXPECT_EQ("     33", (MemoryWriter() << pad(33ll, 7)).str());
  EXPECT_EQ("     44", (MemoryWriter() << pad(44ull, 7)).str());

  MemoryWriter w;
  w.clear();
  w << pad(42, 5, '0');
  EXPECT_EQ("00042", w.str());
  w.clear();
  w << Date(2012, 12, 9);
  EXPECT_EQ("2012-12-9", w.str());
  w.clear();
  w << iso8601(Date(2012, 1, 9));
  EXPECT_EQ("2012-01-09", w.str());
}

TEST(FMT_MemoryWriter, PadString) {
  EXPECT_EQ("test    ", (MemoryWriter() << pad("test", 8)).str());
  EXPECT_EQ("test******", (MemoryWriter() << pad("test", 10, '*')).str());
}

TEST(FMT_MemoryWriter, PadWString) {
  EXPECT_EQ(L"test    ", (WMemoryWriter() << pad(L"test", 8)).str());
  EXPECT_EQ(L"test******", (WMemoryWriter() << pad(L"test", 10, '*')).str());
  EXPECT_EQ(L"test******", (WMemoryWriter() << pad(L"test", 10, L'*')).str());
}

TEST(FMT_MemoryWriter, NoConflictWithIOManip) {
  using namespace std;
  using namespace fmt;
  EXPECT_EQ("cafe", (MemoryWriter() << hex(0xcafe)).str());
  EXPECT_EQ("12", (MemoryWriter() << oct(012)).str());
}

TEST(FMT_MemoryWriter, Format) {
  MemoryWriter w;
  w.write("part{0}", 1);
  EXPECT_EQ(strlen("part1"), w.size());
  EXPECT_STREQ("part1", w.c_str());
  EXPECT_STREQ("part1", w.data());
  EXPECT_EQ("part1", w.str());
  w.write("part{0}", 2);
  EXPECT_EQ(strlen("part1part2"), w.size());
  EXPECT_STREQ("part1part2", w.c_str());
  EXPECT_STREQ("part1part2", w.data());
  EXPECT_EQ("part1part2", w.str());
}

TEST(FMT_MemoryWriter, WWriter) {
  EXPECT_EQ(L"cafe", (fmt::WMemoryWriter() << fmt::hex(0xcafe)).str());
}

TEST(FMT_ArrayWriter, Ctor) {
  char array[10] = "garbage";
  fmt::ArrayWriter w(array, sizeof(array));
  EXPECT_EQ(0u, w.size());
  EXPECT_STREQ("", w.c_str());
}

TEST(FMT_ArrayWriter, CompileTimeSizeCtor) {
  char array[10] = "garbage";
  fmt::ArrayWriter w(array);
  EXPECT_EQ(0u, w.size());
  EXPECT_STREQ("", w.c_str());
  w.write("{:10}", 1);
}

TEST(FMT_ArrayWriter, Write) {
  char array[10];
  fmt::ArrayWriter w(array, sizeof(array));
  w.write("{}", 42);
  EXPECT_EQ("42", w.str());
}

TEST(FMT_ArrayWriter, BufferOverflow) {
  char array[10];
  fmt::ArrayWriter w(array, sizeof(array));
  w.write("{:10}", 1);
  EXPECT_THROW_MSG(w.write("{}", 1), std::runtime_error, "buffer overflow");
}

TEST(FMT_ArrayWriter, WChar) {
  wchar_t array[10];
  fmt::WArrayWriter w(array);
  w.write(L"{}", 42);
  EXPECT_EQ(L"42", w.str());
}

TEST(FMT_Formatter, Escape) {
  EXPECT_EQ("{", format("{{"));
  EXPECT_EQ("before {", format("before {{"));
  EXPECT_EQ("{ after", format("{{ after"));
  EXPECT_EQ("before { after", format("before {{ after"));

  EXPECT_EQ("}", format("}}"));
  EXPECT_EQ("before }", format("before }}"));
  EXPECT_EQ("} after", format("}} after"));
  EXPECT_EQ("before } after", format("before }} after"));

  EXPECT_EQ("{}", format("{{}}"));
  EXPECT_EQ("{42}", format("{{{0}}}", 42));
}

TEST(FMT_Formatter, UnmatchedBraces) {
  EXPECT_THROW_MSG(format("{"), FormatError, "invalid format string");
  EXPECT_THROW_MSG(format("}"), FormatError, "unmatched '}' in format string");
  EXPECT_THROW_MSG(format("{0{}"), FormatError, "invalid format string");
}

TEST(FMT_Formatter, NoArgs) {
  EXPECT_EQ("test", format("test"));
}

TEST(FMT_Formatter, ArgsInDifferentPositions) {
  EXPECT_EQ("42", format("{0}", 42));
  EXPECT_EQ("before 42", format("before {0}", 42));
  EXPECT_EQ("42 after", format("{0} after", 42));
  EXPECT_EQ("before 42 after", format("before {0} after", 42));
  EXPECT_EQ("answer = 42", format("{0} = {1}", "answer", 42));
  EXPECT_EQ("42 is the answer", format("{1} is the {0}", "answer", 42));
  EXPECT_EQ("abracadabra", format("{0}{1}{0}", "abra", "cad"));
}

TEST(FMT_Formatter, ArgErrors) {
  EXPECT_THROW_MSG(format("{"), FormatError, "invalid format string");
  EXPECT_THROW_MSG(format("{?}"), FormatError, "invalid format string");
  EXPECT_THROW_MSG(format("{0"), FormatError, "invalid format string");
  EXPECT_THROW_MSG(format("{0}"), FormatError, "argument index out of range");

  char format_str[BUFFER_SIZE];
  safe_sprintf(format_str, "{%u", INT_MAX);
  EXPECT_THROW_MSG(format(format_str), FormatError, "invalid format string");
  safe_sprintf(format_str, "{%u}", INT_MAX);
  EXPECT_THROW_MSG(format(format_str), FormatError,
      "argument index out of range");

  safe_sprintf(format_str, "{%u", INT_MAX + 1u);
  EXPECT_THROW_MSG(format(format_str), FormatError, "number is too big");
  safe_sprintf(format_str, "{%u}", INT_MAX + 1u);
  EXPECT_THROW_MSG(format(format_str), FormatError, "number is too big");
}

TEST(FMT_Formatter, NamedArg) {
  EXPECT_EQ("1/a/A", format("{_1}/{a_}/{A_}", fmt::arg("a_", 'a'),
                            fmt::arg("A_", "A"), fmt::arg("_1", 1)));
  char a = 'A', b = 'B', c = 'C';
  EXPECT_EQ("BB/AA/CC", format("{1}{b}/{0}{a}/{2}{c}", FMT_CAPTURE(a, b, c)));
  EXPECT_EQ(" A", format("{a:>2}", FMT_CAPTURE(a)));
  EXPECT_THROW_MSG(format("{a+}", FMT_CAPTURE(a)), FormatError,
                   "missing '}' in format string");
  EXPECT_THROW_MSG(format("{a}"), FormatError, "argument not found");
  EXPECT_THROW_MSG(format("{d}", FMT_CAPTURE(a, b, c)), FormatError,
                   "argument not found");
  EXPECT_THROW_MSG(format("{a}{}", FMT_CAPTURE(a)),
    FormatError, "cannot switch from manual to automatic argument indexing");
  EXPECT_THROW_MSG(format("{}{a}", FMT_CAPTURE(a)),
    FormatError, "cannot switch from automatic to manual argument indexing");
  EXPECT_EQ(" -42", format("{0:{width}}", -42, fmt::arg("width", 4)));
  EXPECT_EQ("st", format("{0:.{precision}}", "str", fmt::arg("precision", 2)));
  int n = 100;
  EXPECT_EQ(L"n=100", format(L"n={n}", FMT_CAPTURE_W(n)));
}

TEST(FMT_Formatter, AutoArgIndex) {
  EXPECT_EQ("abc", format("{}{}{}", 'a', 'b', 'c'));
  EXPECT_THROW_MSG(format("{0}{}", 'a', 'b'),
      FormatError, "cannot switch from manual to automatic argument indexing");
  EXPECT_THROW_MSG(format("{}{0}", 'a', 'b'),
      FormatError, "cannot switch from automatic to manual argument indexing");
  EXPECT_EQ("1.2", format("{:.{}}", 1.2345, 2));
  EXPECT_THROW_MSG(format("{0}:.{}", 1.2345, 2),
      FormatError, "cannot switch from manual to automatic argument indexing");
  EXPECT_THROW_MSG(format("{:.{0}}", 1.2345, 2),
      FormatError, "cannot switch from automatic to manual argument indexing");
  EXPECT_THROW_MSG(format("{}"), FormatError, "argument index out of range");
}

TEST(FMT_Formatter, EmptySpecs) {
  EXPECT_EQ("42", format("{0:}", 42));
}

TEST(FMT_Formatter, LeftAlign) {
  EXPECT_EQ("42  ", format("{0:<4}", 42));
  EXPECT_EQ("42  ", format("{0:<4o}", 042));
  EXPECT_EQ("42  ", format("{0:<4x}", 0x42));
  EXPECT_EQ("-42  ", format("{0:<5}", -42));
  EXPECT_EQ("42   ", format("{0:<5}", 42u));
  EXPECT_EQ("-42  ", format("{0:<5}", -42l));
  EXPECT_EQ("42   ", format("{0:<5}", 42ul));
  EXPECT_EQ("-42  ", format("{0:<5}", -42ll));
  EXPECT_EQ("42   ", format("{0:<5}", 42ull));
  EXPECT_EQ("-42  ", format("{0:<5}", -42.0));
  EXPECT_EQ("-42  ", format("{0:<5}", -42.0l));
  EXPECT_EQ("c    ", format("{0:<5}", 'c'));
  EXPECT_EQ("abc  ", format("{0:<5}", "abc"));
  EXPECT_EQ("0xface  ", format("{0:<8}", reinterpret_cast<void*>(0xface)));
}

TEST(FMT_Formatter, RightAlign) {
  EXPECT_EQ("  42", format("{0:>4}", 42));
  EXPECT_EQ("  42", format("{0:>4o}", 042));
  EXPECT_EQ("  42", format("{0:>4x}", 0x42));
  EXPECT_EQ("  -42", format("{0:>5}", -42));
  EXPECT_EQ("   42", format("{0:>5}", 42u));
  EXPECT_EQ("  -42", format("{0:>5}", -42l));
  EXPECT_EQ("   42", format("{0:>5}", 42ul));
  EXPECT_EQ("  -42", format("{0:>5}", -42ll));
  EXPECT_EQ("   42", format("{0:>5}", 42ull));
  EXPECT_EQ("  -42", format("{0:>5}", -42.0));
  EXPECT_EQ("  -42", format("{0:>5}", -42.0l));
  EXPECT_EQ("    c", format("{0:>5}", 'c'));
  EXPECT_EQ("  abc", format("{0:>5}", "abc"));
  EXPECT_EQ("  0xface", format("{0:>8}", reinterpret_cast<void*>(0xface)));
}

TEST(FMT_Formatter, NumericAlign) {
  EXPECT_EQ("  42", format("{0:=4}", 42));
  EXPECT_EQ("+ 42", format("{0:=+4}", 42));
  EXPECT_EQ("  42", format("{0:=4o}", 042));
  EXPECT_EQ("+ 42", format("{0:=+4o}", 042));
  EXPECT_EQ("  42", format("{0:=4x}", 0x42));
  EXPECT_EQ("+ 42", format("{0:=+4x}", 0x42));
  EXPECT_EQ("-  42", format("{0:=5}", -42));
  EXPECT_EQ("   42", format("{0:=5}", 42u));
  EXPECT_EQ("-  42", format("{0:=5}", -42l));
  EXPECT_EQ("   42", format("{0:=5}", 42ul));
  EXPECT_EQ("-  42", format("{0:=5}", -42ll));
  EXPECT_EQ("   42", format("{0:=5}", 42ull));
  EXPECT_EQ("-  42", format("{0:=5}", -42.0));
  EXPECT_EQ("-  42", format("{0:=5}", -42.0l));
  EXPECT_THROW_MSG(format("{0:=5", 'c'),
      FormatError, "missing '}' in format string");
  EXPECT_THROW_MSG(format("{0:=5}", 'c'),
      FormatError, "invalid format specifier for char");
  EXPECT_THROW_MSG(format("{0:=5}", "abc"),
      FormatError, "format specifier '=' requires numeric argument");
  EXPECT_THROW_MSG(format("{0:=8}", reinterpret_cast<void*>(0xface)),
      FormatError, "format specifier '=' requires numeric argument");
}

TEST(FMT_Formatter, CenterAlign) {
  EXPECT_EQ(" 42  ", format("{0:^5}", 42));
  EXPECT_EQ(" 42  ", format("{0:^5o}", 042));
  EXPECT_EQ(" 42  ", format("{0:^5x}", 0x42));
  EXPECT_EQ(" -42 ", format("{0:^5}", -42));
  EXPECT_EQ(" 42  ", format("{0:^5}", 42u));
  EXPECT_EQ(" -42 ", format("{0:^5}", -42l));
  EXPECT_EQ(" 42  ", format("{0:^5}", 42ul));
  EXPECT_EQ(" -42 ", format("{0:^5}", -42ll));
  EXPECT_EQ(" 42  ", format("{0:^5}", 42ull));
  EXPECT_EQ(" -42  ", format("{0:^6}", -42.0));
  EXPECT_EQ(" -42 ", format("{0:^5}", -42.0l));
  EXPECT_EQ("  c  ", format("{0:^5}", 'c'));
  EXPECT_EQ(" abc  ", format("{0:^6}", "abc"));
  EXPECT_EQ(" 0xface ", format("{0:^8}", reinterpret_cast<void*>(0xface)));
}


TEST(FMT_Formatter, Fill) {
  EXPECT_THROW_MSG(format("{0:{<5}", 'c'),
      FormatError, "invalid fill character '{'");
  EXPECT_THROW_MSG(format("{0:{<5}}", 'c'),
      FormatError, "invalid fill character '{'");
  EXPECT_EQ("**42", format("{0:*>4}", 42));
  EXPECT_EQ("**-42", format("{0:*>5}", -42));
  EXPECT_EQ("***42", format("{0:*>5}", 42u));
  EXPECT_EQ("**-42", format("{0:*>5}", -42l));
  EXPECT_EQ("***42", format("{0:*>5}", 42ul));
  EXPECT_EQ("**-42", format("{0:*>5}", -42ll));
  EXPECT_EQ("***42", format("{0:*>5}", 42ull));
  EXPECT_EQ("**-42", format("{0:*>5}", -42.0));
  EXPECT_EQ("**-42", format("{0:*>5}", -42.0l));
  EXPECT_EQ("c****", format("{0:*<5}", 'c'));
  EXPECT_EQ("abc**", format("{0:*<5}", "abc"));
  EXPECT_EQ("**0xface", format("{0:*>8}", reinterpret_cast<void*>(0xface)));
}

TEST(FMT_Formatter, PlusSign) {
  EXPECT_EQ("+42", format("{0:+}", 42));
  EXPECT_EQ("-42", format("{0:+}", -42));
  EXPECT_EQ("+42", format("{0:+}", 42));
  EXPECT_THROW_MSG(format("{0:+}", 42u),
      FormatError, "format specifier '+' requires signed argument");
  EXPECT_EQ("+42", format("{0:+}", 42l));
  EXPECT_THROW_MSG(format("{0:+}", 42ul),
      FormatError, "format specifier '+' requires signed argument");
  EXPECT_EQ("+42", format("{0:+}", 42ll));
  EXPECT_THROW_MSG(format("{0:+}", 42ull),
      FormatError, "format specifier '+' requires signed argument");
  EXPECT_EQ("+42", format("{0:+}", 42.0));
  EXPECT_EQ("+42", format("{0:+}", 42.0l));
  EXPECT_THROW_MSG(format("{0:+", 'c'),
      FormatError, "missing '}' in format string");
  EXPECT_THROW_MSG(format("{0:+}", 'c'),
      FormatError, "invalid format specifier for char");
  EXPECT_THROW_MSG(format("{0:+}", "abc"),
      FormatError, "format specifier '+' requires numeric argument");
  EXPECT_THROW_MSG(format("{0:+}", reinterpret_cast<void*>(0x42)),
      FormatError, "format specifier '+' requires numeric argument");
}

TEST(FMT_Formatter, MinusSign) {
  EXPECT_EQ("42", format("{0:-}", 42));
  EXPECT_EQ("-42", format("{0:-}", -42));
  EXPECT_EQ("42", format("{0:-}", 42));
  EXPECT_THROW_MSG(format("{0:-}", 42u),
      FormatError, "format specifier '-' requires signed argument");
  EXPECT_EQ("42", format("{0:-}", 42l));
  EXPECT_THROW_MSG(format("{0:-}", 42ul),
      FormatError, "format specifier '-' requires signed argument");
  EXPECT_EQ("42", format("{0:-}", 42ll));
  EXPECT_THROW_MSG(format("{0:-}", 42ull),
      FormatError, "format specifier '-' requires signed argument");
  EXPECT_EQ("42", format("{0:-}", 42.0));
  EXPECT_EQ("42", format("{0:-}", 42.0l));
  EXPECT_THROW_MSG(format("{0:-", 'c'),
      FormatError, "missing '}' in format string");
  EXPECT_THROW_MSG(format("{0:-}", 'c'),
      FormatError, "invalid format specifier for char");
  EXPECT_THROW_MSG(format("{0:-}", "abc"),
      FormatError, "format specifier '-' requires numeric argument");
  EXPECT_THROW_MSG(format("{0:-}", reinterpret_cast<void*>(0x42)),
      FormatError, "format specifier '-' requires numeric argument");
}

TEST(FMT_Formatter, SpaceSign) {
  EXPECT_EQ(" 42", format("{0: }", 42));
  EXPECT_EQ("-42", format("{0: }", -42));
  EXPECT_EQ(" 42", format("{0: }", 42));
  EXPECT_THROW_MSG(format("{0: }", 42u),
      FormatError, "format specifier ' ' requires signed argument");
  EXPECT_EQ(" 42", format("{0: }", 42l));
  EXPECT_THROW_MSG(format("{0: }", 42ul),
      FormatError, "format specifier ' ' requires signed argument");
  EXPECT_EQ(" 42", format("{0: }", 42ll));
  EXPECT_THROW_MSG(format("{0: }", 42ull),
      FormatError, "format specifier ' ' requires signed argument");
  EXPECT_EQ(" 42", format("{0: }", 42.0));
  EXPECT_EQ(" 42", format("{0: }", 42.0l));
  EXPECT_THROW_MSG(format("{0: ", 'c'),
      FormatError, "missing '}' in format string");
  EXPECT_THROW_MSG(format("{0: }", 'c'),
      FormatError, "invalid format specifier for char");
  EXPECT_THROW_MSG(format("{0: }", "abc"),
      FormatError, "format specifier ' ' requires numeric argument");
  EXPECT_THROW_MSG(format("{0: }", reinterpret_cast<void*>(0x42)),
      FormatError, "format specifier ' ' requires numeric argument");
}

TEST(FMT_Formatter, HashFlag) {
  EXPECT_EQ("42", format("{0:#}", 42));
  EXPECT_EQ("-42", format("{0:#}", -42));
  EXPECT_EQ("0b101010", format("{0:#b}", 42));
  EXPECT_EQ("0B101010", format("{0:#B}", 42));
  EXPECT_EQ("-0b101010", format("{0:#b}", -42));
  EXPECT_EQ("0x42", format("{0:#x}", 0x42));
  EXPECT_EQ("0X42", format("{0:#X}", 0x42));
  EXPECT_EQ("-0x42", format("{0:#x}", -0x42));
  EXPECT_EQ("042", format("{0:#o}", 042));
  EXPECT_EQ("-042", format("{0:#o}", -042));
  EXPECT_EQ("42", format("{0:#}", 42u));
  EXPECT_EQ("0x42", format("{0:#x}", 0x42u));
  EXPECT_EQ("042", format("{0:#o}", 042u));

  EXPECT_EQ("-42", format("{0:#}", -42l));
  EXPECT_EQ("0x42", format("{0:#x}", 0x42l));
  EXPECT_EQ("-0x42", format("{0:#x}", -0x42l));
  EXPECT_EQ("042", format("{0:#o}", 042l));
  EXPECT_EQ("-042", format("{0:#o}", -042l));
  EXPECT_EQ("42", format("{0:#}", 42ul));
  EXPECT_EQ("0x42", format("{0:#x}", 0x42ul));
  EXPECT_EQ("042", format("{0:#o}", 042ul));

  EXPECT_EQ("-42", format("{0:#}", -42ll));
  EXPECT_EQ("0x42", format("{0:#x}", 0x42ll));
  EXPECT_EQ("-0x42", format("{0:#x}", -0x42ll));
  EXPECT_EQ("042", format("{0:#o}", 042ll));
  EXPECT_EQ("-042", format("{0:#o}", -042ll));
  EXPECT_EQ("42", format("{0:#}", 42ull));
  EXPECT_EQ("0x42", format("{0:#x}", 0x42ull));
  EXPECT_EQ("042", format("{0:#o}", 042ull));

  EXPECT_EQ("-42.0000", format("{0:#}", -42.0));
  EXPECT_EQ("-42.0000", format("{0:#}", -42.0l));
  EXPECT_THROW_MSG(format("{0:#", 'c'),
      FormatError, "missing '}' in format string");
  EXPECT_THROW_MSG(format("{0:#}", 'c'),
      FormatError, "invalid format specifier for char");
  EXPECT_THROW_MSG(format("{0:#}", "abc"),
      FormatError, "format specifier '#' requires numeric argument");
  EXPECT_THROW_MSG(format("{0:#}", reinterpret_cast<void*>(0x42)),
      FormatError, "format specifier '#' requires numeric argument");
}

TEST(FMT_Formatter, ZeroFlag) {
  EXPECT_EQ("42", format("{0:0}", 42));
  EXPECT_EQ("-0042", format("{0:05}", -42));
  EXPECT_EQ("00042", format("{0:05}", 42u));
  EXPECT_EQ("-0042", format("{0:05}", -42l));
  EXPECT_EQ("00042", format("{0:05}", 42ul));
  EXPECT_EQ("-0042", format("{0:05}", -42ll));
  EXPECT_EQ("00042", format("{0:05}", 42ull));
  EXPECT_EQ("-0042", format("{0:05}", -42.0));
  EXPECT_EQ("-0042", format("{0:05}", -42.0l));
  EXPECT_THROW_MSG(format("{0:0", 'c'),
      FormatError, "missing '}' in format string");
  EXPECT_THROW_MSG(format("{0:05}", 'c'),
      FormatError, "invalid format specifier for char");
  EXPECT_THROW_MSG(format("{0:05}", "abc"),
      FormatError, "format specifier '0' requires numeric argument");
  EXPECT_THROW_MSG(format("{0:05}", reinterpret_cast<void*>(0x42)),
      FormatError, "format specifier '0' requires numeric argument");
}

TEST(FMT_Formatter, Width) {
  char format_str[BUFFER_SIZE];
  safe_sprintf(format_str, "{0:%u", UINT_MAX);
  increment(format_str + 3);
  EXPECT_THROW_MSG(format(format_str, 0), FormatError, "number is too big");
  std::size_t size = std::strlen(format_str);
  format_str[size] = '}';
  format_str[size + 1] = 0;
  EXPECT_THROW_MSG(format(format_str, 0), FormatError, "number is too big");

  safe_sprintf(format_str, "{0:%u", INT_MAX + 1u);
  EXPECT_THROW_MSG(format(format_str, 0), FormatError, "number is too big");
  safe_sprintf(format_str, "{0:%u}", INT_MAX + 1u);
  EXPECT_THROW_MSG(format(format_str, 0), FormatError, "number is too big");
  EXPECT_EQ(" -42", format("{0:4}", -42));
  EXPECT_EQ("   42", format("{0:5}", 42u));
  EXPECT_EQ("   -42", format("{0:6}", -42l));
  EXPECT_EQ("     42", format("{0:7}", 42ul));
  EXPECT_EQ("   -42", format("{0:6}", -42ll));
  EXPECT_EQ("     42", format("{0:7}", 42ull));
  EXPECT_EQ("   -1.23", format("{0:8}", -1.23));
  EXPECT_EQ("    -1.23", format("{0:9}", -1.23l));
  EXPECT_EQ("    0xcafe", format("{0:10}", reinterpret_cast<void*>(0xcafe)));
  EXPECT_EQ("x          ", format("{0:11}", 'x'));
  EXPECT_EQ("str         ", format("{0:12}", "str"));
}

TEST(FMT_Formatter, RuntimeWidth) {
  char format_str[BUFFER_SIZE];
  safe_sprintf(format_str, "{0:{%u", UINT_MAX);
  increment(format_str + 4);
  EXPECT_THROW_MSG(format(format_str, 0), FormatError, "number is too big");
  std::size_t size = std::strlen(format_str);
  format_str[size] = '}';
  format_str[size + 1] = 0;
  EXPECT_THROW_MSG(format(format_str, 0), FormatError, "number is too big");
  format_str[size + 1] = '}';
  format_str[size + 2] = 0;
  EXPECT_THROW_MSG(format(format_str, 0), FormatError, "number is too big");

  EXPECT_THROW_MSG(format("{0:{", 0),
      FormatError, "invalid format string");
  EXPECT_THROW_MSG(format("{0:{}", 0),
      FormatError, "cannot switch from manual to automatic argument indexing");
  EXPECT_THROW_MSG(format("{0:{?}}", 0),
      FormatError, "invalid format string");
  EXPECT_THROW_MSG(format("{0:{1}}", 0),
      FormatError, "argument index out of range");

  EXPECT_THROW_MSG(format("{0:{0:}}", 0),
      FormatError, "invalid format string");

  EXPECT_THROW_MSG(format("{0:{1}}", 0, -1),
      FormatError, "negative width");
  EXPECT_THROW_MSG(format("{0:{1}}", 0, (INT_MAX + 1u)),
      FormatError, "number is too big");
  EXPECT_THROW_MSG(format("{0:{1}}", 0, -1l),
      FormatError, "negative width");
  if (fmt::internal::const_check(sizeof(long) > sizeof(int))) {
    long value = INT_MAX;
    EXPECT_THROW_MSG(format("{0:{1}}", 0, (value + 1)),
        FormatError, "number is too big");
  }
  EXPECT_THROW_MSG(format("{0:{1}}", 0, (INT_MAX + 1ul)),
      FormatError, "number is too big");

  EXPECT_THROW_MSG(format("{0:{1}}", 0, '0'),
      FormatError, "width is not integer");
  EXPECT_THROW_MSG(format("{0:{1}}", 0, 0.0),
      FormatError, "width is not integer");

  EXPECT_EQ(" -42", format("{0:{1}}", -42, 4));
  EXPECT_EQ("   42", format("{0:{1}}", 42u, 5));
  EXPECT_EQ("   -42", format("{0:{1}}", -42l, 6));
  EXPECT_EQ("     42", format("{0:{1}}", 42ul, 7));
  EXPECT_EQ("   -42", format("{0:{1}}", -42ll, 6));
  EXPECT_EQ("     42", format("{0:{1}}", 42ull, 7));
  EXPECT_EQ("   -1.23", format("{0:{1}}", -1.23, 8));
  EXPECT_EQ("    -1.23", format("{0:{1}}", -1.23l, 9));
  EXPECT_EQ("    0xcafe",
            format("{0:{1}}", reinterpret_cast<void*>(0xcafe), 10));
  EXPECT_EQ("x          ", format("{0:{1}}", 'x', 11));
  EXPECT_EQ("str         ", format("{0:{1}}", "str", 12));
}

TEST(FMT_Formatter, Precision) {
  char format_str[BUFFER_SIZE];
  safe_sprintf(format_str, "{0:.%u", UINT_MAX);
  increment(format_str + 4);
  EXPECT_THROW_MSG(format(format_str, 0), FormatError, "number is too big");
  std::size_t size = std::strlen(format_str);
  format_str[size] = '}';
  format_str[size + 1] = 0;
  EXPECT_THROW_MSG(format(format_str, 0), FormatError, "number is too big");

  safe_sprintf(format_str, "{0:.%u", INT_MAX + 1u);
  EXPECT_THROW_MSG(format(format_str, 0), FormatError, "number is too big");
  safe_sprintf(format_str, "{0:.%u}", INT_MAX + 1u);
  EXPECT_THROW_MSG(format(format_str, 0), FormatError, "number is too big");

  EXPECT_THROW_MSG(format("{0:.", 0),
      FormatError, "missing precision specifier");
  EXPECT_THROW_MSG(format("{0:.}", 0),
      FormatError, "missing precision specifier");

  EXPECT_THROW_MSG(format("{0:.2", 0),
      FormatError, "precision not allowed in integer format specifier");
  EXPECT_THROW_MSG(format("{0:.2}", 42),
      FormatError, "precision not allowed in integer format specifier");
  EXPECT_THROW_MSG(format("{0:.2f}", 42),
      FormatError, "precision not allowed in integer format specifier");
  EXPECT_THROW_MSG(format("{0:.2}", 42u),
      FormatError, "precision not allowed in integer format specifier");
  EXPECT_THROW_MSG(format("{0:.2f}", 42u),
      FormatError, "precision not allowed in integer format specifier");
  EXPECT_THROW_MSG(format("{0:.2}", 42l),
      FormatError, "precision not allowed in integer format specifier");
  EXPECT_THROW_MSG(format("{0:.2f}", 42l),
      FormatError, "precision not allowed in integer format specifier");
  EXPECT_THROW_MSG(format("{0:.2}", 42ul),
      FormatError, "precision not allowed in integer format specifier");
  EXPECT_THROW_MSG(format("{0:.2f}", 42ul),
      FormatError, "precision not allowed in integer format specifier");
  EXPECT_THROW_MSG(format("{0:.2}", 42ll),
      FormatError, "precision not allowed in integer format specifier");
  EXPECT_THROW_MSG(format("{0:.2f}", 42ll),
      FormatError, "precision not allowed in integer format specifier");
  EXPECT_THROW_MSG(format("{0:.2}", 42ull),
      FormatError, "precision not allowed in integer format specifier");
  EXPECT_THROW_MSG(format("{0:.2f}", 42ull),
      FormatError, "precision not allowed in integer format specifier");
  EXPECT_THROW_MSG(format("{0:3.0}", 'x'),
      FormatError, "precision not allowed in integer format specifier");
  EXPECT_EQ("1.2", format("{0:.2}", 1.2345));
  EXPECT_EQ("1.2", format("{0:.2}", 1.2345l));

  EXPECT_THROW_MSG(format("{0:.2}", reinterpret_cast<void*>(0xcafe)),
      FormatError, "precision not allowed in pointer format specifier");
  EXPECT_THROW_MSG(format("{0:.2f}", reinterpret_cast<void*>(0xcafe)),
      FormatError, "precision not allowed in pointer format specifier");

  EXPECT_EQ("st", format("{0:.2}", "str"));
}

TEST(FMT_Formatter, RuntimePrecision) {
  char format_str[BUFFER_SIZE];
  safe_sprintf(format_str, "{0:.{%u", UINT_MAX);
  increment(format_str + 5);
  EXPECT_THROW_MSG(format(format_str, 0), FormatError, "number is too big");
  std::size_t size = std::strlen(format_str);
  format_str[size] = '}';
  format_str[size + 1] = 0;
  EXPECT_THROW_MSG(format(format_str, 0), FormatError, "number is too big");
  format_str[size + 1] = '}';
  format_str[size + 2] = 0;
  EXPECT_THROW_MSG(format(format_str, 0), FormatError, "number is too big");

  EXPECT_THROW_MSG(format("{0:.{", 0),
      FormatError, "invalid format string");
  EXPECT_THROW_MSG(format("{0:.{}", 0),
      FormatError, "cannot switch from manual to automatic argument indexing");
  EXPECT_THROW_MSG(format("{0:.{?}}", 0),
      FormatError, "invalid format string");
  EXPECT_THROW_MSG(format("{0:.{1}", 0, 0),
      FormatError, "precision not allowed in integer format specifier");
  EXPECT_THROW_MSG(format("{0:.{1}}", 0),
      FormatError, "argument index out of range");

  EXPECT_THROW_MSG(format("{0:.{0:}}", 0),
      FormatError, "invalid format string");

  EXPECT_THROW_MSG(format("{0:.{1}}", 0, -1),
      FormatError, "negative precision");
  EXPECT_THROW_MSG(format("{0:.{1}}", 0, (INT_MAX + 1u)),
      FormatError, "number is too big");
  EXPECT_THROW_MSG(format("{0:.{1}}", 0, -1l),
      FormatError, "negative precision");
  if (fmt::internal::const_check(sizeof(long) > sizeof(int))) {
    long value = INT_MAX;
    EXPECT_THROW_MSG(format("{0:.{1}}", 0, (value + 1)),
        FormatError, "number is too big");
  }
  EXPECT_THROW_MSG(format("{0:.{1}}", 0, (INT_MAX + 1ul)),
      FormatError, "number is too big");

  EXPECT_THROW_MSG(format("{0:.{1}}", 0, '0'),
      FormatError, "precision is not integer");
  EXPECT_THROW_MSG(format("{0:.{1}}", 0, 0.0),
      FormatError, "precision is not integer");

  EXPECT_THROW_MSG(format("{0:.{1}}", 42, 2),
      FormatError, "precision not allowed in integer format specifier");
  EXPECT_THROW_MSG(format("{0:.{1}f}", 42, 2),
      FormatError, "precision not allowed in integer format specifier");
  EXPECT_THROW_MSG(format("{0:.{1}}", 42u, 2),
      FormatError, "precision not allowed in integer format specifier");
  EXPECT_THROW_MSG(format("{0:.{1}f}", 42u, 2),
      FormatError, "precision not allowed in integer format specifier");
  EXPECT_THROW_MSG(format("{0:.{1}}", 42l, 2),
      FormatError, "precision not allowed in integer format specifier");
  EXPECT_THROW_MSG(format("{0:.{1}f}", 42l, 2),
      FormatError, "precision not allowed in integer format specifier");
  EXPECT_THROW_MSG(format("{0:.{1}}", 42ul, 2),
      FormatError, "precision not allowed in integer format specifier");
  EXPECT_THROW_MSG(format("{0:.{1}f}", 42ul, 2),
      FormatError, "precision not allowed in integer format specifier");
  EXPECT_THROW_MSG(format("{0:.{1}}", 42ll, 2),
      FormatError, "precision not allowed in integer format specifier");
  EXPECT_THROW_MSG(format("{0:.{1}f}", 42ll, 2),
      FormatError, "precision not allowed in integer format specifier");
  EXPECT_THROW_MSG(format("{0:.{1}}", 42ull, 2),
      FormatError, "precision not allowed in integer format specifier");
  EXPECT_THROW_MSG(format("{0:.{1}f}", 42ull, 2),
      FormatError, "precision not allowed in integer format specifier");
  EXPECT_THROW_MSG(format("{0:3.{1}}", 'x', 0),
      FormatError, "precision not allowed in integer format specifier");
  EXPECT_EQ("1.2", format("{0:.{1}}", 1.2345, 2));
  EXPECT_EQ("1.2", format("{1:.{0}}", 2, 1.2345l));

  EXPECT_THROW_MSG(format("{0:.{1}}", reinterpret_cast<void*>(0xcafe), 2),
      FormatError, "precision not allowed in pointer format specifier");
  EXPECT_THROW_MSG(format("{0:.{1}f}", reinterpret_cast<void*>(0xcafe), 2),
      FormatError, "precision not allowed in pointer format specifier");

  EXPECT_EQ("st", format("{0:.{1}}", "str", 2));
}

template <typename T>
void check_unknown_types(
    const T &value, const char *types, const char *type_name) {
  char format_str[BUFFER_SIZE], message[BUFFER_SIZE];
  const char *special = ".0123456789}";
  for (int i = CHAR_MIN; i <= CHAR_MAX; ++i) {
    char c = static_cast<char>(i);
    if (std::strchr(types, c) || std::strchr(special, c) || !c) continue;
    safe_sprintf(format_str, "{0:10%c}", c);
    if (std::isprint(static_cast<unsigned char>(c))) {
      safe_sprintf(message, "unknown format code '%c' for %s", c, type_name);
    } else {
      safe_sprintf(message, "unknown format code '\\x%02x' for %s", c,
                   type_name);
    }
    EXPECT_THROW_MSG(format(format_str, value), FormatError, message)
      << format_str << " " << message;
  }
}

TEST(FMT_Bool, FormatBool) {
  EXPECT_EQ("true", format("{}", true));
  EXPECT_EQ("false", format("{}", false));
  EXPECT_EQ("1", format("{:d}", true));
  EXPECT_EQ("true ", format("{:5}", true));
  EXPECT_EQ(L"true", format(L"{}", true));
}

TEST(FMT_Formatter, FormatShort) {
  short s = 42;
  EXPECT_EQ("42", format("{0:d}", s));
  unsigned short us = 42;
  EXPECT_EQ("42", format("{0:d}", us));
}

TEST(FMT_Formatter, FormatInt) {
  EXPECT_THROW_MSG(format("{0:v", 42),
      FormatError, "missing '}' in format string");
  check_unknown_types(42, "bBdoxXn", "integer");
}

TEST(FMT_Formatter, FormatBin) {
  EXPECT_EQ("0", format("{0:b}", 0));
  EXPECT_EQ("101010", format("{0:b}", 42));
  EXPECT_EQ("101010", format("{0:b}", 42u));
  EXPECT_EQ("-101010", format("{0:b}", -42));
  EXPECT_EQ("11000000111001", format("{0:b}", 12345));
  EXPECT_EQ("10010001101000101011001111000", format("{0:b}", 0x12345678));
  EXPECT_EQ("10010000101010111100110111101111", format("{0:b}", 0x90ABCDEF));
  EXPECT_EQ("11111111111111111111111111111111",
            format("{0:b}", std::numeric_limits<uint32_t>::max()));
}

TEST(FMT_Formatter, FormatDec) {
  EXPECT_EQ("0", format("{0}", 0));
  EXPECT_EQ("42", format("{0}", 42));
  EXPECT_EQ("42", format("{0:d}", 42));
  EXPECT_EQ("42", format("{0}", 42u));
  EXPECT_EQ("-42", format("{0}", -42));
  EXPECT_EQ("12345", format("{0}", 12345));
  EXPECT_EQ("67890", format("{0}", 67890));
  char buffer[BUFFER_SIZE];
  safe_sprintf(buffer, "%d", INT_MIN);
  EXPECT_EQ(buffer, format("{0}", INT_MIN));
  safe_sprintf(buffer, "%d", INT_MAX);
  EXPECT_EQ(buffer, format("{0}", INT_MAX));
  safe_sprintf(buffer, "%u", UINT_MAX);
  EXPECT_EQ(buffer, format("{0}", UINT_MAX));
  safe_sprintf(buffer, "%ld", 0 - static_cast<unsigned long>(LONG_MIN));
  EXPECT_EQ(buffer, format("{0}", LONG_MIN));
  safe_sprintf(buffer, "%ld", LONG_MAX);
  EXPECT_EQ(buffer, format("{0}", LONG_MAX));
  safe_sprintf(buffer, "%lu", ULONG_MAX);
  EXPECT_EQ(buffer, format("{0}", ULONG_MAX));
}

TEST(FMT_Formatter, FormatHex) {
  EXPECT_EQ("0", format("{0:x}", 0));
  EXPECT_EQ("42", format("{0:x}", 0x42));
  EXPECT_EQ("42", format("{0:x}", 0x42u));
  EXPECT_EQ("-42", format("{0:x}", -0x42));
  EXPECT_EQ("12345678", format("{0:x}", 0x12345678));
  EXPECT_EQ("90abcdef", format("{0:x}", 0x90abcdef));
  EXPECT_EQ("12345678", format("{0:X}", 0x12345678));
  EXPECT_EQ("90ABCDEF", format("{0:X}", 0x90ABCDEF));

  char buffer[BUFFER_SIZE];
  safe_sprintf(buffer, "-%x", 0 - static_cast<unsigned>(INT_MIN));
  EXPECT_EQ(buffer, format("{0:x}", INT_MIN));
  safe_sprintf(buffer, "%x", INT_MAX);
  EXPECT_EQ(buffer, format("{0:x}", INT_MAX));
  safe_sprintf(buffer, "%x", UINT_MAX);
  EXPECT_EQ(buffer, format("{0:x}", UINT_MAX));
  safe_sprintf(buffer, "-%lx", 0 - static_cast<unsigned long>(LONG_MIN));
  EXPECT_EQ(buffer, format("{0:x}", LONG_MIN));
  safe_sprintf(buffer, "%lx", LONG_MAX);
  EXPECT_EQ(buffer, format("{0:x}", LONG_MAX));
  safe_sprintf(buffer, "%lx", ULONG_MAX);
  EXPECT_EQ(buffer, format("{0:x}", ULONG_MAX));
}

TEST(FMT_Formatter, FormatOct) {
  EXPECT_EQ("0", format("{0:o}", 0));
  EXPECT_EQ("42", format("{0:o}", 042));
  EXPECT_EQ("42", format("{0:o}", 042u));
  EXPECT_EQ("-42", format("{0:o}", -042));
  EXPECT_EQ("12345670", format("{0:o}", 012345670));
  char buffer[BUFFER_SIZE];
  safe_sprintf(buffer, "-%o", 0 - static_cast<unsigned>(INT_MIN));
  EXPECT_EQ(buffer, format("{0:o}", INT_MIN));
  safe_sprintf(buffer, "%o", INT_MAX);
  EXPECT_EQ(buffer, format("{0:o}", INT_MAX));
  safe_sprintf(buffer, "%o", UINT_MAX);
  EXPECT_EQ(buffer, format("{0:o}", UINT_MAX));
  safe_sprintf(buffer, "-%lo", 0 - static_cast<unsigned long>(LONG_MIN));
  EXPECT_EQ(buffer, format("{0:o}", LONG_MIN));
  safe_sprintf(buffer, "%lo", LONG_MAX);
  EXPECT_EQ(buffer, format("{0:o}", LONG_MAX));
  safe_sprintf(buffer, "%lo", ULONG_MAX);
  EXPECT_EQ(buffer, format("{0:o}", ULONG_MAX));
}

struct ConvertibleToLongLong {
  operator fmt::LongLong() const {
    return fmt::LongLong(1) << 32;
  }
};

TEST(FMT_Formatter, FormatConvertibleToLongLong) {
  EXPECT_EQ("100000000", format("{:x}", ConvertibleToLongLong()));
}

TEST(FMT_Formatter, FormatFloat) {
  EXPECT_EQ("392.500000", format("{0:f}", 392.5f));
}

TEST(FMT_Formatter, FormatDouble) {
  check_unknown_types(1.2, "eEfFgGaA", "double");
  EXPECT_EQ("0", format("{0:}", 0.0));
  EXPECT_EQ("0.000000", format("{0:f}", 0.0));
  EXPECT_EQ("392.65", format("{0:}", 392.65));
  EXPECT_EQ("392.65", format("{0:g}", 392.65));
  EXPECT_EQ("392.65", format("{0:G}", 392.65));
  EXPECT_EQ("392.650000", format("{0:f}", 392.65));
  EXPECT_EQ("392.650000", format("{0:F}", 392.65));
  char buffer[BUFFER_SIZE];
  safe_sprintf(buffer, "%e", 392.65);
  EXPECT_EQ(buffer, format("{0:e}", 392.65));
  safe_sprintf(buffer, "%E", 392.65);
  EXPECT_EQ(buffer, format("{0:E}", 392.65));
  EXPECT_EQ("+0000392.6", format("{0:+010.4g}", 392.65));
  safe_sprintf(buffer, "%a", -42.0);
  EXPECT_EQ(buffer, format("{:a}", -42.0));
  safe_sprintf(buffer, "%A", -42.0);
  EXPECT_EQ(buffer, format("{:A}", -42.0));
}

TEST(FMT_Formatter, FormatNaN) {
  double nan = std::numeric_limits<double>::quiet_NaN();
  EXPECT_EQ("nan", format("{}", nan));
  EXPECT_EQ("+nan", format("{:+}", nan));
  EXPECT_EQ(" nan", format("{: }", nan));
  EXPECT_EQ("NAN", format("{:F}", nan));
  EXPECT_EQ("nan    ", format("{:<7}", nan));
  EXPECT_EQ("  nan  ", format("{:^7}", nan));
  EXPECT_EQ("    nan", format("{:>7}", nan));
}

TEST(FMT_Formatter, FormatInfinity) {
  double inf = std::numeric_limits<double>::infinity();
  EXPECT_EQ("inf", format("{}", inf));
  EXPECT_EQ("+inf", format("{:+}", inf));
  EXPECT_EQ("-inf", format("{}", -inf));
  EXPECT_EQ(" inf", format("{: }", inf));
  EXPECT_EQ("INF", format("{:F}", inf));
  EXPECT_EQ("inf    ", format("{:<7}", inf));
  EXPECT_EQ("  inf  ", format("{:^7}", inf));
  EXPECT_EQ("    inf", format("{:>7}", inf));
}

TEST(FMT_Formatter, FormatLongDouble) {
  EXPECT_EQ("0", format("{0:}", 0.0l));
  EXPECT_EQ("0.000000", format("{0:f}", 0.0l));
  EXPECT_EQ("392.65", format("{0:}", 392.65l));
  EXPECT_EQ("392.65", format("{0:g}", 392.65l));
  EXPECT_EQ("392.65", format("{0:G}", 392.65l));
  EXPECT_EQ("392.650000", format("{0:f}", 392.65l));
  EXPECT_EQ("392.650000", format("{0:F}", 392.65l));
  char buffer[BUFFER_SIZE];
  safe_sprintf(buffer, "%Le", 392.65l);
  EXPECT_EQ(buffer, format("{0:e}", 392.65l));
  EXPECT_EQ("+0000392.6", format("{0:+010.4g}", 392.64l));
}

TEST(FMT_Formatter, FormatChar) {
  const char types[] = "cbBdoxXn";
  check_unknown_types('a', types, "char");
  EXPECT_EQ("a", format("{0}", 'a'));
  EXPECT_EQ("z", format("{0:c}", 'z'));
  EXPECT_EQ(L"a", format(L"{0}", 'a'));
  int n = 'x';
  for (const char *type = types + 1; *type; ++type) {
    std::string format_str = fmt::format("{{:{}}}", *type);
    EXPECT_EQ(fmt::format(format_str, n), fmt::format(format_str, 'x'));
  }
  EXPECT_EQ(fmt::format("{:02X}", n), fmt::format("{:02X}", 'x'));
}

TEST(FMT_Formatter, FormatUnsignedChar) {
  EXPECT_EQ("42", format("{}", static_cast<unsigned char>(42)));
  EXPECT_EQ("42", format("{}", static_cast<uint8_t>(42)));
}

TEST(FMT_Formatter, FormatWChar) {
  EXPECT_EQ(L"a", format(L"{0}", L'a'));
  // This shouldn't compile:
  //format("{}", L'a');
}

TEST(FMT_Formatter, FormatCString) {
  check_unknown_types("test", "sp", "string");
  EXPECT_EQ("test", format("{0}", "test"));
  EXPECT_EQ("test", format("{0:s}", "test"));
  char nonconst[] = "nonconst";
  EXPECT_EQ("nonconst", format("{0}", nonconst));
  EXPECT_THROW_MSG(format("{0}", reinterpret_cast<const char*>(0)),
      FormatError, "string pointer is null");
}

TEST(FMT_Formatter, FormatSCharString) {
  signed char str[] = "test";
  EXPECT_EQ("test", format("{0:s}", str));
  const signed char *const_str = str;
  EXPECT_EQ("test", format("{0:s}", const_str));
}

TEST(FMT_Formatter, FormatUCharString) {
  unsigned char str[] = "test";
  EXPECT_EQ("test", format("{0:s}", str));
  const unsigned char *const_str = str;
  EXPECT_EQ("test", format("{0:s}", const_str));
  unsigned char *ptr = str;
  EXPECT_EQ("test", format("{0:s}", ptr));
}

TEST(FMT_Formatter, FormatPointer) {
  check_unknown_types(reinterpret_cast<void*>(0x1234), "p", "pointer");
  EXPECT_EQ("0x0", format("{0}", reinterpret_cast<void*>(0)));
  EXPECT_EQ("0x1234", format("{0}", reinterpret_cast<void*>(0x1234)));
  EXPECT_EQ("0x1234", format("{0:p}", reinterpret_cast<void*>(0x1234)));
//  EXPECT_EQ("0x" + std::string(sizeof(void*) * CHAR_BIT / 4, 'f'),
//      format("{0}", reinterpret_cast<void*>(~uint64_t())));
}

TEST(FMT_Formatter, FormatString) {
  EXPECT_EQ("test", format("{0}", std::string("test")));
}

TEST(FMT_Formatter, FormatStringRef) {
  EXPECT_EQ("test", format("{0}", StringRef("test")));
}

TEST(FMT_Formatter, FormatCStringRef) {
  EXPECT_EQ("test", format("{0}", CStringRef("test")));
}

void format_arg(fmt::BasicFormatter<char> &f, const char *, const Date &d) {
  f.writer() << d.year() << '-' << d.month() << '-' << d.day();
}

TEST(FMT_Formatter, FormatCustom) {
  Date date(2012, 12, 9);
  EXPECT_THROW_MSG(fmt::format("{:s}", date), FormatError,
                   "unmatched '}' in format string");
}

class Answer {};

template <typename Char>
void format_arg(fmt::BasicFormatter<Char> &f, const Char *, Answer) {
  f.writer() << "42";
}

TEST(FMT_Formatter, CustomFormat) {
  EXPECT_EQ("42", format("{0}", Answer()));
}

TEST(FMT_Formatter, WideFormatString) {
  EXPECT_EQ(L"42", format(L"{}", 42));
  EXPECT_EQ(L"4.2", format(L"{}", 4.2));
  EXPECT_EQ(L"abc", format(L"{}", L"abc"));
  EXPECT_EQ(L"z", format(L"{}", L'z'));
}

TEST(FMT_Formatter, FormatStringFromSpeedTest) {
  EXPECT_EQ("1.2340000000:0042:+3.13:str:0x3e8:X:%",
      format("{0:0.10f}:{1:04}:{2:+g}:{3}:{4}:{5}:%",
          1.234, 42, 3.13, "str", reinterpret_cast<void*>(1000), 'X'));
}

TEST(FMT_Formatter, FormatExamples) {
  using fmt::hex;
  EXPECT_EQ("0000cafe", (MemoryWriter() << pad(hex(0xcafe), 8, '0')).str());

  std::string message = format("The answer is {}", 42);
  EXPECT_EQ("The answer is 42", message);

  EXPECT_EQ("42", format("{}", 42));
  EXPECT_EQ("42", format(std::string("{}"), 42));

  MemoryWriter out;
  out << "The answer is " << 42 << "\n";
  out.write("({:+f}, {:+f})", -3.14, 3.14);
  EXPECT_EQ("The answer is 42\n(-3.140000, +3.140000)", out.str());

  {
    MemoryWriter writer;
    for (int i = 0; i < 10; i++)
      writer.write("{}", i);
    std::string s = writer.str(); // s == 0123456789
    EXPECT_EQ("0123456789", s);
  }

  const char *filename = "nonexistent";
  FILE *ftest = safe_fopen(filename, "r");
  if (ftest) fclose(ftest);
  int error_code = errno;
  EXPECT_TRUE(ftest == 0);
  EXPECT_SYSTEM_ERROR({
    FILE *f = safe_fopen(filename, "r");
    if (!f)
      throw fmt::SystemError(errno, "Cannot open file '{}'", filename);
    fclose(f);
  }, error_code, "Cannot open file 'nonexistent'");
}

TEST(FMT_Formatter, Examples) {
  EXPECT_EQ("First, thou shalt count to three",
      format("First, thou shalt count to {0}", "three"));
  EXPECT_EQ("Bring me a shrubbery",
      format("Bring me a {}", "shrubbery"));
  EXPECT_EQ("From 1 to 3", format("From {} to {}", 1, 3));

  char buffer[BUFFER_SIZE];
  safe_sprintf(buffer, "%03.2f", -1.2);
  EXPECT_EQ(buffer, format("{:03.2f}", -1.2));

  EXPECT_EQ("a, b, c", format("{0}, {1}, {2}", 'a', 'b', 'c'));
  EXPECT_EQ("a, b, c", format("{}, {}, {}", 'a', 'b', 'c'));
  EXPECT_EQ("c, b, a", format("{2}, {1}, {0}", 'a', 'b', 'c'));
  EXPECT_EQ("abracadabra", format("{0}{1}{0}", "abra", "cad"));

  EXPECT_EQ("left aligned                  ",
      format("{:<30}", "left aligned"));
  EXPECT_EQ("                 right aligned",
      format("{:>30}", "right aligned"));
  EXPECT_EQ("           centered           ",
      format("{:^30}", "centered"));
  EXPECT_EQ("***********centered***********",
      format("{:*^30}", "centered"));

  EXPECT_EQ("+3.140000; -3.140000",
      format("{:+f}; {:+f}", 3.14, -3.14));
  EXPECT_EQ(" 3.140000; -3.140000",
      format("{: f}; {: f}", 3.14, -3.14));
  EXPECT_EQ("3.140000; -3.140000",
      format("{:-f}; {:-f}", 3.14, -3.14));

  EXPECT_EQ("int: 42;  hex: 2a;  oct: 52",
      format("int: {0:d};  hex: {0:x};  oct: {0:o}", 42));
  EXPECT_EQ("int: 42;  hex: 0x2a;  oct: 052",
      format("int: {0:d};  hex: {0:#x};  oct: {0:#o}", 42));

  EXPECT_EQ("The answer is 42", format("The answer is {}", 42));
  EXPECT_THROW_MSG(
    format("The answer is {:d}", "forty-two"), FormatError,
    "unknown format code 'd' for string");

  EXPECT_EQ(L"Cyrillic letter \x42e",
    format(L"Cyrillic letter {}", L'\x42e'));

//  EXPECT_WRITE(stdout,
//      fmt::print("{}", std::numeric_limits<double>::infinity()), "inf");
}

TEST(FMT_FormatInt, Data) {
  fmt::FormatInt format_int(42);
  EXPECT_EQ("42", std::string(format_int.data(), format_int.size()));
}

TEST(FMT_FormatInt, FormatInt) {
  EXPECT_EQ("42", fmt::FormatInt(42).str());
  EXPECT_EQ(2u, fmt::FormatInt(42).size());
  EXPECT_EQ("-42", fmt::FormatInt(-42).str());
  EXPECT_EQ(3u, fmt::FormatInt(-42).size());
  EXPECT_EQ("42", fmt::FormatInt(42ul).str());
  EXPECT_EQ("-42", fmt::FormatInt(-42l).str());
  EXPECT_EQ("42", fmt::FormatInt(42ull).str());
  EXPECT_EQ("-42", fmt::FormatInt(-42ll).str());
  std::ostringstream os;
  os << std::numeric_limits<int64_t>::max();
  EXPECT_EQ(os.str(),
            fmt::FormatInt(std::numeric_limits<int64_t>::max()).str());
}

template <typename T>
std::string format_decimal(T value) {
  char buffer[10];
  char *ptr = buffer;
  fmt::format_decimal(ptr, value);
  return std::string(buffer, ptr);
}

TEST(FMT_FormatInt, FormatDec) {
  EXPECT_EQ("-42", format_decimal(static_cast<signed char>(-42)));
  EXPECT_EQ("-42", format_decimal(static_cast<short>(-42)));
  std::ostringstream os;
  os << std::numeric_limits<unsigned short>::max();
  EXPECT_EQ(os.str(),
            format_decimal(std::numeric_limits<unsigned short>::max()));
  EXPECT_EQ("1", format_decimal(1));
  EXPECT_EQ("-1", format_decimal(-1));
  EXPECT_EQ("42", format_decimal(42));
  EXPECT_EQ("-42", format_decimal(-42));
  EXPECT_EQ("42", format_decimal(42l));
  EXPECT_EQ("42", format_decimal(42ul));
  EXPECT_EQ("42", format_decimal(42ll));
  EXPECT_EQ("42", format_decimal(42ull));
}

TEST(FMT_FormatInt, Variadic) {
  EXPECT_EQ("abc1", format("{}c{}", "ab", 1));
  EXPECT_EQ(L"abc1", format(L"{}c{}", L"ab", 1));
}

TEST(FMT_Format, JoinArg) {
  using fmt::join;
  int v1[3] = { 1, 2, 3 };
  std::vector<float> v2;
  v2.push_back(1.2f);
  v2.push_back(3.4f);

  EXPECT_EQ("(1, 2, 3)", format("({})", join(v1, v1 + 3, ", ")));
  EXPECT_EQ("(1)", format("({})", join(v1, v1 + 1, ", ")));
  EXPECT_EQ("()", format("({})", join(v1, v1, ", ")));
  EXPECT_EQ("(001, 002, 003)", format("({:03})", join(v1, v1 + 3, ", ")));
  EXPECT_EQ("(+01.20, +03.40)",
            format("({:+06.2f})", join(v2.begin(), v2.end(), ", ")));

  EXPECT_EQ(L"(1, 2, 3)", format(L"({})", join(v1, v1 + 3, L", ")));
  EXPECT_EQ("1, 2, 3", format("{0:{1}}", join(v1, v1 + 3, ", "), 1));
}

template <typename T>
std::string str(const T &value) {
  return fmt::format("{}", value);
}

TEST(FMT_Str, Convert) {
  EXPECT_EQ("42", str(42));
  std::string s = str(Date(2012, 12, 9));
  EXPECT_EQ("2012-12-9", s);
}

std::string format_message(int id, const char *format, const fmt::ArgList &args) {
  MemoryWriter w;
  w.write("[{}] ", id);
  w.write(format, args);
  return w.str();
}

FMT_VARIADIC(std::string, format_message, int, const char *)

TEST(FMT_Format, FormatMessageExample) {
  EXPECT_EQ("[42] something happened",
      format_message(42, "{} happened", "something"));
}

class test_class
{
  public:
  std::string format_message(int id, const char *format,const fmt::ArgList &args) const {
    MemoryWriter w;
    w.write("[{}] ", id);
    w.write(format, args);
    return w.str();
  }
  FMT_VARIADIC_CONST(std::string, format_message, int, const char *)
};

TEST(FMT_Format, ConstFormatMessage) {
  test_class c;
  EXPECT_EQ("[42] something happened",
    c.format_message(42, "{} happened", "something"));
}

TEST(FMT_Format, MaxArgs) {
  EXPECT_EQ("0123456789abcde",
            fmt::format("{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}",
                        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 'a', 'b', 'c', 'd', 'e'));
}

enum TestEnum { A };

TEST(FMT_Format, Enum) {
  EXPECT_EQ("0", fmt::format("{}", A));
}

enum TestFixedEnum : short { B };

TEST(FMT_Format, FixedEnum) {
  EXPECT_EQ("0", fmt::format("{}", B));
}
} // namespace format {


