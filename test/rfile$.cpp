#include "IFILE/rfile.h"
#include "GOOGLE/gtest.h"



//***** ibmi::file::rfile **********************************************************************************************
namespace ibmi_file_rfile {

struct rfile : public testing::Test
{
  static void SetUpTestCase() {
    delete_test_file();
    system("RMVLIBLE LIB(QTEMP)");
    if (system("ADDLIBLE LIB(QTEMP)"))
      FAIL() << "Cannot add QTEMP to *LIBL";
    if (system("CRTSRCPF FILE(QTEMP/RFILE_TEST) MBR(MEMBER)"))
      FAIL() << "Cannot create test file QTEMP/RFILE_TEST";
  }

  static void TearDownTestCase() {
    delete_test_file();
  }

  static void delete_test_file() {
    system("DLTF FILE(QTEMP/RFILE_TEST)");
  }
};


TEST_F(rfile, exception)
{
  ibmi::file::rfile f("qtemp/not_create", "rr");
  EXPECT_THROW(static_cast<_RFILE*>(f), std::runtime_error);
}


TEST_F(rfile, constructor)
{
  { // Конструктор от параметров
    ibmi::file::rfile f("rfile_test", "rr");

    auto ptr = static_cast<_RFILE*>(f);
    ASSERT_NE(ptr, nullptr);

    auto fbk = _Ropnfbk(ptr);
    ASSERT_EQ(ibmi::string_view(fbk->library_name).stdstr(), "QTEMP");
    ASSERT_EQ(ibmi::string_view(fbk->file_name).stdstr(), "RFILE_TEST");
    ASSERT_EQ(ibmi::string_view(fbk->member_name).stdstr(), "MEMBER");
  }

  { // Конструктор копии
//    ibmi::file::rfile src("rfile_test", "rr");
//    auto src_ptr = static_cast<_RFILE*>(src);
//
//    ibmi::file::rfile dst(src);
//    auto dst_ptr = static_cast<_RFILE*>(dst);
//
//    ASSERT_NE(src_ptr, dst_ptr);
  }
}
} // namespace ibmi_file_rfile {

