/*
     _____ __    _____
    |     |  |  |_   _|  C++ Object Libraries for IBM i
    |  |  |  |__ _| |_   File:   IFILE/rfile.h
    |_____|_____|_____|  Author: Sergey Chebotarev

*/

#ifndef OLI_IFILE_RFILE_H
#define OLI_IFILE_RFILE_H

#include "IOBJECT/fs.h"
#include <recio.h>

IBMI_NAMESPACE_BEGIN ///////////////////////////////////////////////////////////////////////////////////////////////////
FILE_NAMESPACE_BEGIN





//***** ДЕСКРИПТОР ФАЙЛА СИСТЕМЫ IBM I *********************************************************************************
class rfile
{
  public:
  rfile(fs::path path, const char* mode);
  rfile(const rfile& src) = delete;
  rfile(rfile&& src) = delete;
  rfile& operator = (const rfile& src) = delete;
  rfile& operator = (rfile&& src) = delete;
  ~rfile();


  operator _RFILE* () {
    return handle_ ? handle_ : open();
  }

  fs::path path() const {
    return path_;
  }


  private:
  _RFILE* open();
  void close();


  private:
  fs::path    path_;
  const char* mode_;
  _RFILE*     handle_;
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





FILE_NAMESPACE_END
IBMI_NAMESPACE_END /////////////////////////////////////////////////////////////////////////////////////////////////////

#endif // OLI_IFILE_RFILE_H

