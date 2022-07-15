/*
     _____ __    _____
    |     |  |  |_   _|  C++ Object Libraries for IBM i
    |  |  |  |__ _| |_   File:   OBJECT/object$.cpp
    |_____|_____|_____|  Author: Sergey Chebotarev

*/

#include "OBJECT/fs.h"
#include <regex>

IBMI_NAMESPACE_BEGIN ///////////////////////////////////////////////////////////////////////////////////////////////////




//***** ПУТЬ К ОБЪЕКТУ ФАЙЛОВОЙ СИСТЕМЫ IBM I **************************************************************************
FS_NAMESPACE_BEGIN

/* Парсинг пути к объекту
*/ void path::parse_path(std::string p)
{
  std::tr1::smatch match;
  std::tr1::regex  rgx("^\\s*((([$#@A-IJ-RS-Z][$#@\\w.]{0,9})|\\*LIBL|\\*CURLIB)/)?"
                       "([$#@A-IJ-RS-Z][$#@\\w.]{0,9})"
                       "(\\((([$#@A-IJ-RS-Z][$#@\\w.]{0,9})|\\*FIRST)\\))?\\s*$");

  std::transform(p.begin(), p.end(), p.begin(), toupper);
  if (!std::tr1::regex_search(p, match, rgx))
    throw std::invalid_argument("Invalid object path");

  m_library.assign(match[2]);
  m_object.assign(match[4]);
  m_member.assign(match[6]);
}

FS_NAMESPACE_END
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





IBMI_NAMESPACE_END /////////////////////////////////////////////////////////////////////////////////////////////////////

