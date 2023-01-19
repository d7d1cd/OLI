/*
     _____ __    _____
    |     |  |  |_   _|  C++ Object Libraries for IBM i
    |  |  |  |__ _| |_   File:   IFILE/data_base.h
    |_____|_____|_____|  Author: Sergey Chebotarev

*/

#ifndef OLI_IFILE_DATA_BASE_H
#define OLI_IFILE_DATA_BASE_H

#include "IFILE/rfile.h"
#include "STL/utility.h"

IBMI_NAMESPACE_BEGIN ///////////////////////////////////////////////////////////////////////////////////////////////////
FILE_NAMESPACE_BEGIN





//**********************************************************************************************************************
template <typename R, typename K = void>
struct data_base
{
  static_assert(std::tr1::is_class<R>::value,
                "Type of Record must be a class");
  static_assert(std::tr1::is_class<K>::value || std::tr1::is_same<K, void>::value,
                "Type of Key must be a class or void");


  /**
   */
  data_base(const fs::path& p)
  : rfile_(p, "rr+") {}


  private:
  rfile rfile_;
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





FILE_NAMESPACE_END
IBMI_NAMESPACE_END /////////////////////////////////////////////////////////////////////////////////////////////////////

#endif // OLI_IFILE_DATA_BASE_H

