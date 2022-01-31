/*
     _____ __    _____
    |     |  |  |_   _|  C++ Object Library for IBM i
    |  |  |  |__ _| |_   File:   STRING/istring$.cpp
    |_____|_____|_____|  Author: Sergey Chebotarev

*/

#include "STRING/istring.h"
#include <algorithm>                                                                                                    // std::fill

OLI_NAMESPACE_BEGIN ////////////////////////////////////////////////////////////////////////////////////////////////////





//***** КОНСТАНТНАЯ БАЗА СТРОКИ ****************************************************************************************

template <typename Heir>
istring_base_const<Heir>::istring_base_const(size_type capacity)
                        : m_capacity(capacity)
{
}


template <typename Heir>
typename istring_base_const<Heir>::const_iterator
istring_base_const<Heir>::begin() const
{
  return data();
}


template <typename Heir>
typename istring_base_const<Heir>::const_iterator
istring_base_const<Heir>::end() const
{
  return data() + m_capacity;
}


template <typename Heir>
typename istring_base_const<Heir>::const_reverse_iterator
istring_base_const<Heir>::rbegin() const
{
  return const_reverse_iterator(end());
}


template <typename Heir>
typename istring_base_const<Heir>::const_reverse_iterator
istring_base_const<Heir>::rend() const
{
  return const_reverse_iterator(begin());
}


template <typename Heir>
typename istring_base_const<Heir>::size_type
istring_base_const<Heir>::capacity() const
{
  return m_capacity;
}


template <typename Heir>
typename istring_base_const<Heir>::size_type
istring_base_const<Heir>::length() const
{
  auto len = m_capacity;

  for (auto it = rbegin(), end = rend(); it != end; --len)
    if (*it++ != BLANK) break;

  return len;
}


template <typename Heir>
typename istring_base_const<Heir>::const_pointer
istring_base_const<Heir>::data() const
{
  return static_cast<const Heir*>(this)->data();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





//***** БАЗА СТРОКИ ****************************************************************************************************

template <typename Heir>
istring_base<Heir>::istring_base(size_type capacity)
                  : istring_base_const<Heir>(capacity)
{
}


template <typename Heir>
Heir& istring_base<Heir>::operator =(const char* rhs)
{
  copy_from(rhs, std::strlen(rhs));
  return *static_cast<Heir*>(this);
}


template <typename Heir>
typename istring_base<Heir>::iterator
istring_base<Heir>::begin()
{
  return data();
}


template <typename Heir>
typename istring_base<Heir>::iterator
istring_base<Heir>::end()
{
  return begin() + capacity();
}


template <typename Heir>
typename istring_base<Heir>::reverse_iterator
istring_base<Heir>::rbegin()
{
  return reverse_iterator(end());
}


template <typename Heir>
typename istring_base<Heir>::reverse_iterator
istring_base<Heir>::rend()
{
  return reverse_iterator(begin());
}


template <typename Heir>
void istring_base<Heir>::clear()
{
  fill_with(begin(), this->BLANK);
}


template <typename Heir>
typename istring_base<Heir>::pointer
istring_base<Heir>::data()
{
  return static_cast<Heir*>(this)->data();
}


template <typename Heir>
void istring_base<Heir>::fill_with(iterator whence, value_type agg)
{
  std::fill(whence, end(), agg);
}


template <typename Heir>
void istring_base<Heir>::copy_from(const_iterator start, const size_type size)
{
  size_type less = std::min(size, capacity());
  std::copy(start, start + less, begin());
  fill_with(begin() + less, this->BLANK);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





//***** РЕАЛИЗАЦИЯ СТРОКИ **********************************************************************************************

istring::istring(size_type capacity)
       : istring_base<istring>(capacity),
         m_data(new value_type[capacity])
{
  clear();
}


istring::~istring()
{
  delete [] m_data;
}


istring::pointer istring::data()
{
  return m_data;
}


istring::const_pointer istring::data() const
{
  return m_data;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





//***** ЯВНОЕ ИНСТАНЦИРОВАНИЕ ******************************************************************************************
template class istring_base_const<istring>;
template class istring_base<istring>;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





OLI_NAMESPACE_END //////////////////////////////////////////////////////////////////////////////////////////////////////

