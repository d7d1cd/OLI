/*
     _____ __    _____
    |     |  |  |_   _|  C++ Object Libraries for IBM i
    |  |  |  |__ _| |_   File:   SRC/ifile.cpp
    |_____|_____|_____|  Author: Sergey Chebotarev

*/

#include "IFILE/rfile.h"
#include <qusrmbrd.h>
#include <qusec.h>

IBMI_NAMESPACE_BEGIN ///////////////////////////////////////////////////////////////////////////////////////////////////
FILE_NAMESPACE_BEGIN



//***** ДЕСКРИПТОР ФАЙЛА СИСТЕМЫ IBM I *********************************************************************************

/**
 *
 */
rfile::rfile(const fs::path& path, const char* mode)
     : path_(path), mode_(mode), handle_(nullptr)
{
  Qus_EC_t        error = { sizeof(Qus_EC_t) };
  Qdb_Mbrd0100_t  info  = {};
  char            one   = '1';
  ibmi::string<8> format("MBRD0100");

  // TODO путь должен возвращать тип name_type по значению,
  //      name_type (aka string) должен уметь конструироваться от него
  fs::path::name_type member(path.member());
  if (!member.length())  // TODO Нужен метод ibmi::string::empty()
    member = fs::path::first_member;

  QUSRMBRD(&info, sizeof(info), format.data(),
           path_.qname(),  // TODO заменить на path.object() + path.library()
           member.data(), // TODO заменить на path.member()
           &one, &error, &one);

  if (error.Bytes_Available)
    throw std::runtime_error(ibmi::string_ref(error.Exception_Id));

  // TODO path должне уметь менять любой из компонентов пути
  //      Тогда можно поменять только библиотеку и мембер:
  //      path_.library(info.Db_File_Lib);
  //      path_.library(info.Member_Name);
  path_ = fs::path(ibmi::string_ref(info.Db_File_Lib),
                   ibmi::string_ref(info.Db_File_Name),
                   ibmi::string_ref(info.Member_Name));
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



FILE_NAMESPACE_END
IBMI_NAMESPACE_END /////////////////////////////////////////////////////////////////////////////////////////////////////

