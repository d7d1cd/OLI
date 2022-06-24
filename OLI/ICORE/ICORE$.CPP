/*
     _____ __    _____
    |     |  |  |_   _|  C++ Object Libraries for IBM i
    |  |  |  |__ _| |_   File:   ICORE/icore$.cpp
    |_____|_____|_____|  Author: Sergey Chebotarev

*/

#include "ICORE/string.h"

namespace ibmi { ///////////////////////////////////////////////////////////////////////////////////////////////////////





//***** СТРОКА *********************************************************************************************************
namespace string_detail {

types_holder::blanks_type get_blanks()
{
  types_holder::blanks_type blanks;
  std::fill(blanks.begin(), blanks.end(), types_holder::blank);
  return blanks;
}

const types_holder::blanks_type types_holder::blanks = get_blanks();

} // namespace string_detail ///////////////////////////////////////////////////////////////////////////////////////////





} // namespace ibmi ////////////////////////////////////////////////////////////////////////////////////////////////////

