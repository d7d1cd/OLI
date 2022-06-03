/*
     _____ __    _____
    |     |  |  |_   _|  C++ Object Libraries for IBM i
    |  |  |  |__ _| |_   File:   ISTRING/base.h
    |_____|_____|_____|  Author: Sergey Chebotarev

*/

#ifndef OLI_ISTRING_BASE_H
#define OLI_ISTRING_BASE_H

#include "CORE/olidef.h"
#include "STL/type_trait.h"
#include "STL/iterator.h"
#include <algorithm>

IBM_NAMESPACE_BEGIN ////////////////////////////////////////////////////////////////////////////////////////////////////
namespace istring_detail { /////////////////////////////////////////////////////////////////////////////////////////////





//***** КОНСТАНТАНАЯ БАЗА ISTRING **************************************************************************************
template <typename Heir, std::size_t N = 0>
class istring_base_const
{
  // MEMBER TYPES ------------------------------------------------------------------------------------------------------
  public:
  typedef char value_type;
  typedef std::char_traits<value_type> traits_type;
  typedef std::size_t size_type;
  typedef std::ptrdiff_t difference_type;
  typedef value_type* pointer;
  typedef const value_type* const_pointer;
  typedef value_type& reference;
  typedef const value_type& const_reference;
  typedef pointer iterator;
  typedef const_pointer const_iterator;
  typedef std::reverse_iterator<iterator> reverse_iterator;
  typedef std::reverse_iterator<const_iterator> const_reverse_iterator;



  // CONSTANTS ---------------------------------------------------------------------------------------------------------
  static const value_type blank = '\x40';



  // ITERATORS ---------------------------------------------------------------------------------------------------------
  const_iterator         begin()   const { return data(); }
  const_iterator         cbegin()  const { return begin(); }
  const_iterator         end()     const { return data() + capacity(); }
  const_iterator         cend()    const { return end(); }
  const_reverse_iterator rbegin()  const { return const_reverse_iterator(end()); }
  const_reverse_iterator crbegin() const { return rbegin(); }
  const_reverse_iterator rend()    const { return const_reverse_iterator(begin()); }
  const_reverse_iterator crend()   const { return rend(); }



  // ELEMENT ACCESS ----------------------------------------------------------------------------------------------------
  const_pointer data() const { return static_cast<const Heir*>(this)->m_data; }



  // CAPACITY ----------------------------------------------------------------------------------------------------------
  template <std::size_t N2 = N, typename stl::enable_if<N2 != 0, int>::type = 0>                                        // Емкость строки для
  size_type capacity() const { return N2; }                                                                             // istring

  template <std::size_t N2 = N, typename stl::enable_if<N2 == 0, int>::type = 0>                                        // Емкость строки для
  size_type capacity() const { return static_cast<const Heir*>(this)->m_capacity; }                                     // istring_ref, _view

  size_type length() const {                                                                                            // Длина строки
    auto crit = crbegin(), end = crend();
    while (crit != end && *crit == blank) ++crit;
    return std::distance(crit, end);
  }



  // SERVICE -----------------------------------------------------------------------------------------------------------
  private:
};



template <typename Heir, std::size_t N>
const typename istring_base_const<Heir, N>::value_type istring_base_const<Heir, N>::blank;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





//***** ИЗМЕНЯЕМАЯ БАЗА ISTRING ****************************************************************************************
template <typename Heir, std::size_t N = 0>
class istring_base : public istring_base_const<Heir, N>
{
  typedef istring_base_const<Heir, N> base;



  // SPECIAL MEMBER FUNCTIONS ------------------------------------------------------------------------------------------
  public:
  Heir& assign(typename base::const_pointer start, typename base::size_type size) {                                     // Копирование из указателя и длины
    auto less = std::min(size, base::capacity());
    std::memcpy(begin(), start, less);
    fill_tail(begin() + less);
    return *static_cast<Heir*>(this);
  }



  // ITERATORS ---------------------------------------------------------------------------------------------------------
  using    base::begin;
  using    base::end;
  using    base::rbegin;
  using    base::rend;
  typename base::iterator         begin()  { return data(); }
  typename base::iterator         end()    { return data() + base::capacity(); }
  typename base::reverse_iterator rbegin() { return base::reverse_iterator(end()); }
  typename base::reverse_iterator rend()   { return base::reverse_iterator(begin()); }



  // ELEMENT ACCESS ----------------------------------------------------------------------------------------------------
  using    base::data;
  typename base::pointer data() { return static_cast<Heir*>(this)->m_data; }



  // OPERATIONS --------------------------------------------------------------------------------------------------------
  void clear() { fill_tail(begin()); }


  // SERVICE -----------------------------------------------------------------------------------------------------------
  private:
  void fill_tail(typename base::iterator whence) {                                                                      // Заполнение хвоста
    std::memset(whence, base::blank, end() - whence);
  }
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





} // namespace istring_detail //////////////////////////////////////////////////////////////////////////////////////////
IBM_NAMESPACE_END //////////////////////////////////////////////////////////////////////////////////////////////////////

#endif // OLI_ISTRING_BASE_H

