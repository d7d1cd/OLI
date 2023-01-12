/*
     _____ __    _____
    |     |  |  |_   _|  C++ Object Libraries for IBM i
    |  |  |  |__ _| |_   File:   SRC/ifile.cpp
    |_____|_____|_____|  Author: Sergey Chebotarev

*/

#include "IFILE/rfile.h"

IBMI_NAMESPACE_BEGIN ///////////////////////////////////////////////////////////////////////////////////////////////////




//***** ДЕСКРИПТОР ФАЙЛА СИСТЕМЫ IBM I *********************************************************************************
FILE_NAMESPACE_BEGIN

rfile::rfile(fs::path path, const char* mode)
     : path_(path), mode_(mode), handle_(nullptr)
{
  open();
}


rfile::~rfile()
{
  close();
}


_RFILE* rfile::open()
{
  handle_ = _Ropen(path_.native().c_str(), mode_);
  if (!handle_)
    throw std::runtime_error(std::strerror(errno));

  auto fbk = _Ropnfbk(handle_);
  path_ = fs::path(ibmi::string_ref(fbk->library_name),
                   ibmi::string_ref(fbk->file_name),
                   ibmi::string_ref(fbk->member_name));
  return handle_;
}


void rfile::close()
{
  if (handle_)
    _Rclose(handle_);

  handle_ = nullptr;
}

FILE_NAMESPACE_END
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




IBMI_NAMESPACE_END /////////////////////////////////////////////////////////////////////////////////////////////////////

