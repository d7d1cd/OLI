#include "IFILE/rfile.h"
#include "GOOGLE/gtest.h"

extern std::string TEST_LIBRARY; // Имя тестовой библиотеки. Определено в test/main$.cpp

namespace {
const std::string DB_FILE = "DB_TEST";
const std::string DB_MBR  = "DB_MBR";
}



//***** ibmi::file::rfile **********************************************************************************************
namespace ibmi_file_rfile {

TEST(rfile, exception)
{
  EXPECT_THROW(ibmi::file::rfile("qtemp/not_exists", "rr"), std::runtime_error);
}


TEST(rfile, constructor)
{
  { // Конструктор от пути должен открыть файл, чтобы определить имена всех компонентов пути
    ibmi::file::rfile f(DB_FILE, "rr");
    auto p = f.path();
    ASSERT_EQ(p.library(), TEST_LIBRARY);
    ASSERT_EQ(p.object(), DB_FILE);
    ASSERT_EQ(p.member(), DB_MBR);
  }
}
} // namespace ibmi_file_rfile {

