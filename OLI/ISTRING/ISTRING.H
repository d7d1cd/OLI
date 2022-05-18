/*
     _____ __    _____
    |     |  |  |_   _|  C++ Object Libraries for IBM i
    |  |  |  |__ _| |_   File:   ISTRING/istring.h
    |_____|_____|_____|  Author: Sergey Chebotarev

*/

#ifndef OLI_ISTRING_ISTRING_H
#define OLI_ISTRING_ISTRING_H

#include "ISTRING/base.h"

IBM_NAMESPACE_BEGIN ////////////////////////////////////////////////////////////////////////////////////////////////////





//***** СТАТИЧЕСКАЯ СТРОКА *********************************************************************************************
template <std::size_t N>
class istring : public istring_detail::istring_base<istring<N>, N>
{
  static_assert(N > 0, "String size cannot be 0");                                                                      // Проверка размера строки
  typedef istring_detail::istring_base<istring<N>, N> base;                                                             // Короткое имя базы
  friend class istring_detail::istring_base<istring<N>, N>;                                                             // База нам друг
  typename base::value_type m_data[N];                                                                                  // Хранилище строки



  public:                                                                                                               // SPECIAL MEMBER FUNCTIONS
  istring() {                                                                                                           // Конструктор по умолчанию
    base::clear();
  }
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





//***** ССЫЛКА НА СТРОКУ ***********************************************************************************************
class istring_ref : public istring_detail::istring_base<istring_ref>
{
  typedef istring_detail::istring_base<istring_ref> base;                                                               // Короткое имя базы
  friend class istring_detail::istring_base<istring_ref>;                                                               // База нам друг
  base::pointer m_data;                                                                                                 // Указатель на хранилище строки
  base::size_type m_capacity;                                                                                           // Емкость строки



  public:                                                                                                               // SPECIAL MEMBER FUNCTIONS
  template <typename InputIt>
  istring_ref(InputIt first, InputIt last, bool clear = false)
  : m_data(first), m_capacity(std::distance(first, last)) {
    if (clear)
      base::clear();
  }

  template <std::size_t N>
  istring_ref(base::value_type (&src)[N], bool clear = false)
  : istring_ref(stl::begin(src), stl::end(src), clear) {}
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





IBM_NAMESPACE_END //////////////////////////////////////////////////////////////////////////////////////////////////////

#endif // OLI_ISTRING_ISTRING_H

