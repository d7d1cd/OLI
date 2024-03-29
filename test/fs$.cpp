#include "IOBJECT/fs.h"
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

  EXPECT_THROW(ibmi::fs::path("?lib", "obj"), std::invalid_argument);
  EXPECT_THROW(ibmi::fs::path("lib", "longnameobject"), std::invalid_argument);
  EXPECT_THROW(ibmi::fs::path("lib", "file", "1mbr"), std::invalid_argument);
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

  { // Задан пустой путь
    ibmi::fs::path p("");
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

  { // Конструктор от const char* + проверка copy initialization
    ibmi::fs::path p = "library/file(member)";
    ASSERT_EQ(p.library(), "LIBRARY");
    ASSERT_EQ(p.object(), "FILE");
    ASSERT_EQ(p.member(), "MEMBER");
  }

  { // Конструктор от библиотеки и объекта
    ibmi::fs::path p("lib", "obj");
    ASSERT_EQ(p.library(), "LIB");
    ASSERT_EQ(p.object(), "OBJ");
    ASSERT_EQ(p.member(), "");
  }

  { // Конструктор от библиотеки, файла и мембера
    ibmi::fs::path p("lib", "file", "mbr");
    ASSERT_EQ(p.library(), "LIB");
    ASSERT_EQ(p.object(), "FILE");
    ASSERT_EQ(p.member(), "MBR");
  }
}


TEST(path, clear)
{
  ibmi::fs::path p("library/file(member)");
  ASSERT_EQ(p.library(), "LIBRARY");
  ASSERT_EQ(p.object(), "FILE");
  ASSERT_EQ(p.member(), "MEMBER");

  p.clear();
  ASSERT_EQ(p.library(), "");
  ASSERT_EQ(p.object(), "");
  ASSERT_EQ(p.member(), "");
}


TEST(path, native)
{
  // Путь пуст
  ibmi::fs::path p;
  ASSERT_EQ(p.native(), "");

  // Задано только имя объекта
  p = "object";
  ASSERT_EQ(p.native(), "*LIBL/OBJECT");

  // Задано имя объекта и библиотеки
  p = "library/object";
  ASSERT_EQ(p.native(), "LIBRARY/OBJECT");

  // Задано имя файла и мембера
  p = "file(member)";
  ASSERT_EQ(p.native(), "*LIBL/FILE(MEMBER)");

  // Задано имя библиотеки, файла и мембера
  p = "library/file(member)";
  ASSERT_EQ(p.native(), "LIBRARY/FILE(MEMBER)");
}


TEST(path, qname)
{
  ibmi::fs::path p("file(member)");
  ASSERT_EQ(memcmp(p.qname(), "FILE      *LIBL     ", 20), 0);

  p = "library/object";
  ASSERT_EQ(memcmp(p.qname(), "OBJECT    LIBRARY   ", 20), 0);

  p.clear();
  ASSERT_EQ(memcmp(p.qname(), "                    ", 20), 0);
}
} // namespace ibmi_fs_path {

