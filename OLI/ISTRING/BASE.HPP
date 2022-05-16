/*
     _____ __    _____
    |     |  |  |_   _|  C++ Object Libraries for IBM i
    |  |  |  |__ _| |_   File:   ISTRING/base.hpp
    |_____|_____|_____|  Author: Sergey Chebotarev

*/

#ifndef OLI_ISTRING_BASE_HPP
#define OLI_ISTRING_BASE_HPP

#include "CORE/olidef.h"
#include <cstddef>                                                                                                      // std::size_t, std::ptrdiff_t
#include <iterator>                                                                                                     // std::reverse_iterator

OLI_NAMESPACE_BEGIN ////////////////////////////////////////////////////////////////////////////////////////////////////
namespace istring_detail { /////////////////////////////////////////////////////////////////////////////////////////////





//***** БАЗА ISTRING ***************************************************************************************************
template <typename Heir>
class istring_base
{
  public:                                                                                                               // MEMBER TYPES
  typedef char value_type;                                                                                              // Тип символа строки
  typedef std::size_t size_type;                                                                                        // Размер
  typedef const value_type* const_pointer;                                                                              // Указатель на константу
  typedef const_pointer const_iterator;                                                                                 // Итератор на константу
  typedef std::reverse_iterator<const_iterator> const_reverse_iterator;                                                 // Обратный итератор на константу


  public:                                                                                                               // CONSTANTS
  static const value_type BLANK = '\x40';                                                                               // Бланк (символ пустоты в строке)
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





} // namespace istring_detail //////////////////////////////////////////////////////////////////////////////////////////
OLI_NAMESPACE_END //////////////////////////////////////////////////////////////////////////////////////////////////////

#endif // OLI_ISTRING_BASE_HPP

