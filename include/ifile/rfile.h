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
struct rfile
{
  /**
   *    Конструктор от пути к файлу (мемберу) и его режима открытия
   */
  rfile(const fs::path& path, const char* mode);

  /**
   */
  rfile(const rfile& src, const char* mode)
  : path_(src.path_), mode_(mode), handle_(nullptr) {}

  /**
   */
  rfile(const rfile& src)
  : rfile(src, src.mode_) {}

  /**
   */
  rfile(rfile&& src) = delete;

  /**
   */
  rfile& operator = (const rfile& src) = delete;

  /**
   */
  rfile& operator = (rfile&& src) = delete;

  /**
   */
  ~rfile() {
    close();
  }

  /**
   */
  void open() {
    handle_ = _Ropen(path_.native().c_str(), mode_);
    if (!handle_)
      throw std::runtime_error(std::strerror(errno));
  }

  /**
   */
  void close() {
    if (handle_)
      _Rclose(handle_);
    handle_ = nullptr;
  }

  /**
   */
  operator _RFILE* () const {
    return handle_;
  }

  /**
   */
  fs::path path() const {
    return path_;
  }

  private:
  fs::path    path_;    ///< Полный путь к мемберу файла
  const char* mode_;    ///< Режим открытия файла
  _RFILE*     handle_;  ///< "Сырой" дескриптор файла
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





FILE_NAMESPACE_END
IBMI_NAMESPACE_END /////////////////////////////////////////////////////////////////////////////////////////////////////

#endif // OLI_IFILE_RFILE_H

