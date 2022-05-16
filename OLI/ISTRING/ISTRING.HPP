/*
     _____ __    _____
    |     |  |  |_   _|  C++ Object Libraries for IBM i
    |  |  |  |__ _| |_   File:   ISTRING/istring.hpp
    |_____|_____|_____|  Author: Sergey Chebotarev

*/

#ifndef OLI_ISTRING_ISTRING_HPP
#define OLI_ISTRING_ISTRING_HPP

#include "base"

IBM_NAMESPACE_BEGIN ////////////////////////////////////////////////////////////////////////////////////////////////////





//***** СТАТИЧЕСКАЯ СТРОКА *********************************************************************************************
template <std::size_t N>
class istring : public istring_detail::istring_base<istring<N>, N>
{
  static_assert(N > 0, "String size cannot be 0");                                                                      // Проверка размера строки
  typedef istring_detail::istring_base<istring<N>> base;                                                                // Короткое имя базы
  friend class istring_detail::istring_base<istring<N>>;                                                                // База нам друг


  public:                                                                                                               // SPECIAL MEMBER FUNCTIONS


  private:                                                                                                              // DATA MEMBERS
  typename base::value_type m_data[N];                                                                                  // Хранилище строки
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





//***** ДИНАМИЧЕСКАЯ СТРОКА ********************************************************************************************
class istring_dyn : public istring_detail::istring_base<istring_dyn>
{
  typedef istring_detail::istring_base<istring_dyn> base;                                                               // Короткое имя базы
  friend class istring_detail::istring_base<istring_dyn>;                                                               // База нам друг


  public:                                                                                                               // SPECIAL MEMBER FUNCTIONS


  private:                                                                                                              // DATA MEMBERS
  base::pointer m_data;                                                                                                 // Указатель на хранилище строки
  base::size_type m_capacity;                                                                                           // Емкость строки
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





//***** ССЫЛКА НА СТРОКУ ***********************************************************************************************
class istring_ref : public istring_detail::istring_base<istring_ref>
{
  typedef istring_detail::istring_base<istring_ref> base;                                                               // Короткое имя базы
  friend class istring_detail::istring_base<istring_ref>;                                                               // База нам друг


  public:                                                                                                               // SPECIAL MEMBER FUNCTIONS


  private:                                                                                                              // DATA MEMBERS
  base::pointer m_data;                                                                                                 // Указатель на хранилище строки
  base::size_type m_capacity;                                                                                           // Емкость строки
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





IBM_NAMESPACE_END //////////////////////////////////////////////////////////////////////////////////////////////////////

#endif // OLI_ISTRING_ISTRING_HPP

