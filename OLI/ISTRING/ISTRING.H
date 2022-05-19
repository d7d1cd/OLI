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
  static_assert(N > 0, "String size cannot be 0");
  friend class istring_detail::istring_base_const<istring<N>, N>;
  friend class istring_detail::istring_base<istring<N>, N>;
  typedef istring_detail::istring_base<istring<N>, N> base;
  typename base::value_type m_data[N];



  // SPECIAL MEMBER FUNCTIONS ------------------------------------------------------------------------------------------
  public:
  istring() {                                                                                                           // Конструктор по умолчанию
    base::clear();
  }

  template <typename InputIt>                                                                                           // Конструктор из диапазона
  istring(InputIt first, InputIt last) {
    base::assign(first, last);
  }

  template <typename T, std::size_t S>                                                                                  // Конструктор из массива
  istring(T (&src)[S]) {
    base::assign(src, S);
  }
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





//***** ССЫЛКА НА СТРОКУ ***********************************************************************************************
class istring_ref : public istring_detail::istring_base<istring_ref>
{
  friend class istring_detail::istring_base_const<istring_ref>;
  friend class istring_detail::istring_base<istring_ref>;
  typedef istring_detail::istring_base<istring_ref> base;
  base::pointer m_data;
  base::size_type m_capacity;



  // SPECIAL MEMBER FUNCTIONS ------------------------------------------------------------------------------------------
  public:
  template <typename InputIt>                                                                                           // Конструктор из диапазона
  istring_ref(InputIt first, InputIt last, bool clear = false)
  : m_data(first), m_capacity(std::distance(first, last)) {
    if (clear)
      base::clear();
  }

  template <typename T, std::size_t N>                                                                                  // Конструктор из массива
  istring_ref(T (&src)[N], bool clear = false)
  : istring_ref(stl::begin(src), stl::end(src), clear) {}
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





//***** ПРЕДСТАВЛЕНИЕ НА СТРОКУ ****************************************************************************************
class istring_view : public istring_detail::istring_base_const<istring_view>
{
  friend class istring_detail::istring_base_const<istring_view>;
  typedef istring_detail::istring_base_const<istring_view> base;
  base::const_pointer m_data;
  base::size_type m_capacity;



  // SPECIAL MEMBER FUNCTIONS ------------------------------------------------------------------------------------------
  public:
  template <typename InputIt>                                                                                           // Конструктор из диапазона
  istring_view(InputIt first, InputIt last)
  : m_data(first), m_capacity(std::distance(first, last)) {}

  template <typename T, std::size_t N>                                                                                  // Конструктор из массива
  istring_view(T (&src)[N])
  : istring_view(stl::begin(src), stl::end(src)) {}
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





IBM_NAMESPACE_END //////////////////////////////////////////////////////////////////////////////////////////////////////

#endif // OLI_ISTRING_ISTRING_H

