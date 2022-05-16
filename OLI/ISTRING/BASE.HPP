/*
     _____ __    _____
    |     |  |  |_   _|  C++ Object Libraries for IBM i
    |  |  |  |__ _| |_   File:   ISTRING/base.hpp
    |_____|_____|_____|  Author: Sergey Chebotarev

*/

#ifndef OLI_ISTRING_BASE_HPP
#define OLI_ISTRING_BASE_HPP

#include "CORE/olidef.h"
#include "STL/type_trait.h"
#include <cstddef>                                                                                                      // std::size_t, std::ptrdiff_t
#include <iterator>                                                                                                     // std::reverse_iterator

IBM_NAMESPACE_BEGIN ////////////////////////////////////////////////////////////////////////////////////////////////////
namespace istring_detail { /////////////////////////////////////////////////////////////////////////////////////////////





//***** БАЗА ISTRING ***************************************************************************************************
template <typename Heir, std::size_t N = 0>
class istring_base
{
  // MEMBER TYPES ------------------------------------------------------------------------------------------------------
  public:
  typedef char value_type;                                                                                              // Тип символа строки
  typedef std::size_t size_type;                                                                                        // Размер
  typedef value_type* pointer;                                                                                          // Указатель
  typedef const value_type* const_pointer;                                                                              // Указатель на константу
  typedef value_type& reference;                                                                                        // Ссылка
  typedef const value_type& const_reference;                                                                            // Ссылка на константу
  typedef pointer iterator;                                                                                             // Итератор
  typedef const_pointer const_iterator;                                                                                 // Итератор на константу
  typedef std::reverse_iterator<iterator> reverse_iterator;                                                             // Обратный итератор
  typedef std::reverse_iterator<const_iterator> const_reverse_iterator;                                                 // Обратный итератор на константу
  typedef std::ptrdiff_t difference_type;                                                                               // Тип разницы указателей


  // CONSTANTS ---------------------------------------------------------------------------------------------------------
  static const value_type blank = '\x40';                                                                               // Бланк (символ пустоты в строке)


  // ITERATORS ---------------------------------------------------------------------------------------------------------



  // ELEMENT ACCESS ----------------------------------------------------------------------------------------------------
  pointer data() {
    return static_cast<Heir*>(this)->m_data;
  }

  const_pointer data() const {
    return static_cast<const Heir*>(this)->m_data;
  }


  // CAPACITY ----------------------------------------------------------------------------------------------------------
  template <std::size_t N2 = N, typename stl::enable_if<N2 != 0, bool>::type = true>
  size_type capacity() const {
    return N2;
  }

  template <std::size_t N2 = N, typename stl::enable_if<N2 == 0, bool>::type = true>
  size_type capacity() const {
    return static_cast<const Heir*>(this)->m_capacity;
  }


  // OPERATIONS --------------------------------------------------------------------------------------------------------



  // SERVICE -----------------------------------------------------------------------------------------------------------
  private:
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





} // namespace istring_detail //////////////////////////////////////////////////////////////////////////////////////////
IBM_NAMESPACE_END //////////////////////////////////////////////////////////////////////////////////////////////////////

#endif // OLI_ISTRING_BASE_HPP

