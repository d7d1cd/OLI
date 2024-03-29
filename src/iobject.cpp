/*
     _____ __    _____
    |     |  |  |_   _|  C++ Object Libraries for IBM i
    |  |  |  |__ _| |_   File:   SRC/iobject.cpp
    |_____|_____|_____|  Author: Sergey Chebotarev

*/

#include "IOBJECT/fs.h"
#include <regex>

IBMI_NAMESPACE_BEGIN ///////////////////////////////////////////////////////////////////////////////////////////////////




//***** ПУТЬ К ОБЪЕКТУ ФАЙЛОВОЙ СИСТЕМЫ IBM I **************************************************************************
FS_NAMESPACE_BEGIN

/* Инициализация статических констант
*/ path::name_type::const_pointer const path::libl         = "*LIBL";
   path::name_type::const_pointer const path::curlib       = "*CURLIB";
   path::name_type::const_pointer const path::first_member = "*FIRST";



/*
*/ path::path(const std::string& lib, const std::string& obj, const std::string& mbr)
{
  parse_path(combine(lib, obj, mbr));
}


/* Очистка
*/ void path::clear()
{
  m_object.clear();
  m_library.clear();
  m_member.clear();
}



/* Путь к объекту в "родном" для файловой системы IBM i формате
*/ std::string path::native() const
{
  if (*m_object.begin() == name_type::blank)
    return std::string();

  return combine(library(), object(), member());
}



/* Парсинг пути к объекту
*/ void path::parse_path(std::string p)
{
  if (p.empty())
    return clear();

  std::tr1::smatch match;
  std::tr1::regex  rgx("^\\s*((([$#@A-IJ-RS-Z][$#@\\w.]{0,9})|\\*LIBL|\\*CURLIB)/)?"
                       "([$#@A-IJ-RS-Z][$#@\\w.]{0,9})"
                       "(\\((([$#@A-IJ-RS-Z][$#@\\w.]{0,9})|\\*FIRST)\\))?\\s*$");

  std::transform(p.begin(), p.end(), p.begin(), toupper);
  if (!std::tr1::regex_search(p, match, rgx))
    throw std::invalid_argument("Invalid object or file member path");

  m_object.assign(match[4]);

  if (match[2].matched)
    m_library.assign(match[2].str());
  else
    m_library.assign(libl);

  if (match[6].matched)
    m_member.assign(match[6].str());
  else
    m_member.clear();
}


/*
*/ std::string path::combine(std::string lib, const std::string& obj, const std::string& mbr) const
{
  lib.reserve(33);
  lib += '/' + obj + (mbr.empty() ? std::string() : ('(' + mbr + ')'));
  return lib;
}

FS_NAMESPACE_END
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





IBMI_NAMESPACE_END /////////////////////////////////////////////////////////////////////////////////////////////////////

