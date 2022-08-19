#include "OBJECT/fs.h"
#include "GOOGLE/gtest.h"



//***** ibmi::fs::path *************************************************************************************************
namespace ibmi_fs_path {

std::string upper_case(std::string s)
{
  std::transform(s.begin(), s.end(), s.begin(), toupper);
  return s;
}


TEST(path, exception)
{
  EXPECT_THROW(ibmi::fs::path("0obj"), std::invalid_argument);
  EXPECT_THROW(ibmi::fs::path(".obj"), std::invalid_argument);
  EXPECT_THROW(ibmi::fs::path(",obj"), std::invalid_argument);
  EXPECT_THROW(ibmi::fs::path(";obj"), std::invalid_argument);
  EXPECT_THROW(ibmi::fs::path("_obj"), std::invalid_argument);
  EXPECT_THROW(ibmi::fs::path("!obj"), std::invalid_argument);
  EXPECT_THROW(ibmi::fs::path("?obj"), std::invalid_argument);
  EXPECT_THROW(ibmi::fs::path("%obj"), std::invalid_argument);
  EXPECT_THROW(ibmi::fs::path("^obj"), std::invalid_argument);
  EXPECT_THROW(ibmi::fs::path("&obj"), std::invalid_argument);
  EXPECT_THROW(ibmi::fs::path("*obj"), std::invalid_argument);
  EXPECT_THROW(ibmi::fs::path("=obj"), std::invalid_argument);
  EXPECT_THROW(ibmi::fs::path("+obj"), std::invalid_argument);
  EXPECT_THROW(ibmi::fs::path("-obj"), std::invalid_argument);
  EXPECT_THROW(ibmi::fs::path("'obj"), std::invalid_argument);
  EXPECT_THROW(ibmi::fs::path("`obj"), std::invalid_argument);
  EXPECT_THROW(ibmi::fs::path("~obj"), std::invalid_argument);
  EXPECT_THROW(ibmi::fs::path("(obj"), std::invalid_argument);
  EXPECT_THROW(ibmi::fs::path(")obj"), std::invalid_argument);
  EXPECT_THROW(ibmi::fs::path("{obj"), std::invalid_argument);
  EXPECT_THROW(ibmi::fs::path("}obj"), std::invalid_argument);
  EXPECT_THROW(ibmi::fs::path("<obj"), std::invalid_argument);
  EXPECT_THROW(ibmi::fs::path(">obj"), std::invalid_argument);
  EXPECT_THROW(ibmi::fs::path("/obj"), std::invalid_argument);
  EXPECT_THROW(ibmi::fs::path("\\obj"), std::invalid_argument);
  EXPECT_THROW(ibmi::fs::path("\"obj"), std::invalid_argument);

  EXPECT_THROW(ibmi::fs::path("объект"), std::invalid_argument);
  EXPECT_THROW(ibmi::fs::path("objеct"), std::invalid_argument);  // Буква 'е' - кириллицей
  EXPECT_THROW(ibmi::fs::path("long_object_name"), std::invalid_argument);

  EXPECT_THROW(ibmi::fs::path("*lib/obj"), std::invalid_argument);
  EXPECT_THROW(ibmi::fs::path("lib/obj(*mbr)"), std::invalid_argument);
  EXPECT_THROW(ibmi::fs::path("lib\\obj"), std::invalid_argument);
}


TEST(path, constants)
{
  ASSERT_STREQ(ibmi::fs::path::libl, "*LIBL");
  ASSERT_STREQ(ibmi::fs::path::curlib, "*CURLIB");
}


TEST(path, constructors)
{
  std::string lib, obj, mbr;

  { // Путь не задан
    ibmi::fs::path p;
    ASSERT_EQ(lib, p.library());
    ASSERT_EQ(obj, p.object());
    ASSERT_EQ(mbr, p.member());
  }

  { // Задано только имя объекта
    lib = ibmi::fs::path::libl;
    obj = "object";
    ibmi::fs::path p(obj);
    ASSERT_EQ(upper_case(lib), p.library());
    ASSERT_EQ(upper_case(obj), p.object());
    ASSERT_EQ(upper_case(mbr), p.member());
  }

  { // Задана библиотека и имя объекта
    lib = "library";
    ibmi::fs::path p(lib + "/" + obj);
    ASSERT_EQ(upper_case(lib), p.library());
    ASSERT_EQ(upper_case(obj), p.object());
    ASSERT_EQ(upper_case(mbr), p.member());
  }

  { // Задан полный путь к файлу
    mbr = "member";
    ibmi::fs::path p(lib + "/" + obj + "(" + mbr + ")");
    ASSERT_EQ(upper_case(lib), p.library());
    ASSERT_EQ(upper_case(obj), p.object());
    ASSERT_EQ(upper_case(mbr), p.member());
  }

  { // Задано спец. имя библиотеки *LIBL
    lib = "*LIBL";
    ibmi::fs::path p(lib + "/" + obj + "(" + mbr + ")");
    ASSERT_EQ(upper_case(lib), p.library());
    ASSERT_EQ(upper_case(obj), p.object());
    ASSERT_EQ(upper_case(mbr), p.member());
  }

  { // Задано спец. имя библиотеки *curlib
    lib = "*curlib";
    ibmi::fs::path p(lib + "/" + obj + "(" + mbr + ")");
    ASSERT_EQ(upper_case(lib), p.library());
    ASSERT_EQ(upper_case(obj), p.object());
    ASSERT_EQ(upper_case(mbr), p.member());
  }

  { // Задано спец. имя мембера *fiRST
    mbr = "*fiRST";
    ibmi::fs::path p(lib + "/" + obj + "(" + mbr + ")");
    ASSERT_EQ(upper_case(lib), p.library());
    ASSERT_EQ(upper_case(obj), p.object());
    ASSERT_EQ(upper_case(mbr), p.member());
  }

  { // Конструктор копии
    ibmi::fs::path p(lib + "/" + obj + "(" + mbr + ")");
    ibmi::fs::path p2(p);
    ASSERT_EQ(upper_case(lib), p2.library());
    ASSERT_EQ(upper_case(obj), p2.object());
    ASSERT_EQ(upper_case(mbr), p2.member());
  }
}


TEST(path, assignment_operator)
{
  std::string lib = "library", obj = "object", mbr = "member";
  ibmi::fs::path p, p2;

  { // Оператор присваивания от стандартной строки
    p = lib + "/" + obj + "(" + mbr + ")";
    ASSERT_EQ(upper_case(lib), p.library());
    ASSERT_EQ(upper_case(obj), p.object());
    ASSERT_EQ(upper_case(mbr), p.member());
  }

  { // Оператор присваивания копированием
    p2 = p;
    ASSERT_EQ(upper_case(lib), p2.library());
    ASSERT_EQ(upper_case(obj), p2.object());
    ASSERT_EQ(upper_case(mbr), p2.member());
  }
}


TEST(path, native_path)
{
  // Задано только имя объекта
  ibmi::fs::path p("object");
  ASSERT_EQ(p.native_path(), "*LIBL/OBJECT");

  // Задано имя объекта и библиотеки
  p = "library/object";
  ASSERT_EQ(p.native_path(), "LIBRARY/OBJECT");

  // Задано имя файла и мембера
  p = "file(member)";
  ASSERT_EQ(p.native_path(), "*LIBL/FILE(MEMBER)");

  // Задано имя библиотеки, файла и мембера
  p = "library/file(member)";
  ASSERT_EQ(p.native_path(), "LIBRARY/FILE(MEMBER)");
}


TEST(path, qname)
{
  ibmi::fs::path p("file(member)");
  ASSERT_EQ(memcmp(p.qname(), "FILE      *LIBL     ", 20), 0);

  p = "library/object";
  ASSERT_EQ(memcmp(p.qname(), "OBJECT    LIBRARY   ", 20), 0);
}
} // namespace ibmi_fs_path {

