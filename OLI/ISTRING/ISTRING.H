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
  template <typename T, std::size_t N>                                                                                  // Конструктор из массива
  istring_ref(T (&src)[N], bool clear = false)
  : m_data(src), m_capacity(N) {
    if (clear)
      base::clear();
  }

  istring_ref(const istring_ref&) = delete;
  istring_ref& operator=(const istring_ref&) = delete;
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
  template <typename T, std::size_t N>                                                                                  // Конструктор из массива
  istring_view(T (&src)[N])
  : m_data(src), m_capacity(N) {}

  istring_view(const istring_view&) = delete;
  istring_view& operator=(const istring_view&) = delete;
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





IBM_NAMESPACE_END //////////////////////////////////////////////////////////////////////////////////////////////////////

#endif // OLI_ISTRING_ISTRING_H

