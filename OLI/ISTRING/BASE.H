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





//***** БАЗА ISTRING ***************************************************************************************************
template <typename Heir, std::size_t N = 0>
class istring_base
{
  // MEMBER TYPES ------------------------------------------------------------------------------------------------------
  public:
  typedef char value_type;
  typedef std::char_traits<value_type> traits_type;
  typedef std::size_t size_type;
  typedef value_type* pointer;
  typedef const value_type* const_pointer;
  typedef value_type& reference;
  typedef const value_type& const_reference;
  typedef pointer iterator;
  typedef const_pointer const_iterator;
  typedef std::reverse_iterator<iterator> reverse_iterator;
  typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
  typedef std::ptrdiff_t difference_type;



  // CONSTANTS ---------------------------------------------------------------------------------------------------------
  static const value_type blank = '\x40';                                                                               // Бланк (символ пустоты в строке)



  // ITERATORS ---------------------------------------------------------------------------------------------------------
  iterator               begin()         { return data(); }
  const_iterator         begin()   const { return data(); }
  const_iterator         cbegin()  const { return begin(); }
  iterator               end()           { return data() + capacity(); }
  const_iterator         end()     const { return data() + capacity(); }
  const_iterator         cend()    const { return end(); }
  reverse_iterator       rbegin()        { return reverse_iterator(end()); }
  const_reverse_iterator rbegin()  const { return const_reverse_iterator(end()); }
  const_reverse_iterator crbegin() const { return rbegin(); }
  reverse_iterator       rend()          { return reverse_iterator(begin()); }
  const_reverse_iterator rend()    const { return const_reverse_iterator(begin()); }
  const_reverse_iterator crend()   const { return rend(); }



  // ELEMENT ACCESS ----------------------------------------------------------------------------------------------------
  pointer       data()       { return static_cast<Heir*>(this)->m_data; }
  const_pointer data() const { return static_cast<const Heir*>(this)->m_data; }



  // CAPACITY ----------------------------------------------------------------------------------------------------------
  template <std::size_t N2 = N, typename stl::enable_if<N2 != 0, int>::type = 0>
  size_type capacity() const { return N2; }

  template <std::size_t N2 = N, typename stl::enable_if<N2 == 0, int>::type = 0>
  size_type capacity() const { return static_cast<const Heir*>(this)->m_capacity; }



  // OPERATIONS --------------------------------------------------------------------------------------------------------
  void clear() { fill_with(begin(), this->blank); }


  // SERVICE -----------------------------------------------------------------------------------------------------------
  private:
  void fill_with(iterator whence, value_type agg) {
    std::fill(whence, end(), agg);
  }
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





//**********************************************************************************************************************
template <typename Heir, std::size_t N>
const typename istring_base<Heir, N>::value_type istring_base<Heir, N>::blank;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





} // namespace istring_detail //////////////////////////////////////////////////////////////////////////////////////////
IBM_NAMESPACE_END //////////////////////////////////////////////////////////////////////////////////////////////////////

#endif // OLI_ISTRING_BASE_H

