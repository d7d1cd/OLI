#include "IFILE/data_base.h"
#include "GOOGLE/gtest.h"
#include <bcd.h>



//**********************************************************************************************************************
extern std::string TEST_LIBRARY; // Имя тестовой библиотеки. Определено в test/main$.cpp

namespace {

const std::string DB_FILE = "DB_TEST";
const std::string DB_MBR  = "DB_MBR";

typedef _Packed struct {
  ibmi::string<10> CHARFLD;
  _DecimalT<5, 0>  DECFLD;
} db_test_record;

typedef _Packed struct {
  ibmi::string<10> CHARFLD;
  _DecimalT<5, 0>  DECFLD;
} db_test_key;
} // namespace {




//***** ibmi::file::rfile **********************************************************************************************
namespace ibmi_file_rfile {

TEST(rfile, exception)
{
  EXPECT_THROW(ibmi::file::rfile("qtemp/not_exists", "rr"), std::runtime_error);
}


TEST(rfile, constructor)
{
  { // Конструктор от пути
    ibmi::file::rfile f(DB_FILE, "rr");
    ASSERT_FALSE(f); // Проверка, что файл не открылся

    auto p = f.path();
    ASSERT_EQ(p.library(), TEST_LIBRARY);
    ASSERT_EQ(p.object(), DB_FILE);
    ASSERT_EQ(p.member(), DB_MBR);
  }
}


TEST(rfile, open)
{
  { // Открытие по умолчанию
    ibmi::file::rfile f(DB_FILE, "rr");
    f.open();
    auto fbk = _Ropnfbk(f);
    ASSERT_NE(fbk, nullptr);
    ASSERT_EQ(ibmi::string_ref(fbk->library_name).stdstr(), TEST_LIBRARY);
    ASSERT_EQ(ibmi::string_ref(fbk->file_name).stdstr(), DB_FILE);
    ASSERT_EQ(ibmi::string_ref(fbk->member_name).stdstr(), DB_MBR);
  }
}


TEST(rfile, close)
{
  ibmi::file::rfile f(DB_FILE, "rr");
  f.open();
  ASSERT_TRUE(f);
  f.close();
  ASSERT_FALSE(f);
}


TEST(rfile, copy_constructor)
{
  ibmi::file::rfile orig(DB_FILE, "rr");
  ibmi::file::rfile copy(orig);
  ASSERT_FALSE(orig);
  ASSERT_FALSE(copy);

  orig.open();
  ibmi::file::rfile copy2(orig);
  ASSERT_TRUE(orig);
  ASSERT_FALSE(copy2);
}
} // namespace ibmi_file_rfile {




//***** ibmi::file::data_base ******************************************************************************************
namespace ibmi_file_data_base {

TEST(data_base, constructor)
{
  ibmi::file::data_base<db_test_record, db_test_key> db_test("DB_TEST");
}
} // namespace ibmi_file_data_base {

