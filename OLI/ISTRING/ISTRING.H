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

