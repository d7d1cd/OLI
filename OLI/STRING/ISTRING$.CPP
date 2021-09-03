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


//template <typename Heir>
//size_t istring_base_const<Heir>::Capacity() const
//{
//  return p_End() - p_Begin();
//}


//template <typename Heir>
//size_t istring_base_const<Heir>::Length() const
//{
//  size_t len = Capacity();
//
//  for (auto it = Rbegin(), rend = Rend(); it != rend; --len)
//    if (*it++ != BLANK) break;
//
//  return len;
//}


template <typename Heir>
typename istring_base_const<Heir>::const_iterator
istring_base_const<Heir>::begin() const
{
  return static_cast<const Heir*>(this)->begin();
}


template <typename Heir>
typename istring_base_const<Heir>::const_iterator
istring_base_const<Heir>::end() const
{
  return static_cast<const Heir*>(this)->end();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





//***** БАЗА СТРОКИ ****************************************************************************************************

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


//template <typename Heir>
//Heir& istring_base<Heir>::operator =(const char* rhs)
//{
//  p_CopyFrom(rhs, std::strlen(rhs));
//  return *static_cast<Heir*>(this);
//}


template <typename Heir>
void istring_base<Heir>::Clear()
{
  fill_with(begin(), this->BLANK);
}


template <typename Heir>
typename istring_base<Heir>::iterator
istring_base<Heir>::begin()
{
  return static_cast<Heir*>(this)->begin();
}


template <typename Heir>
typename istring_base<Heir>::iterator
istring_base<Heir>::end()
{
  return static_cast<Heir*>(this)->end();
}


template <typename Heir>
void istring_base<Heir>::fill_with(iterator whence, value_type agg)
{
  std::fill(whence, end(), agg);
}


//template <typename Heir>
//void istring_base<Heir>::p_CopyFrom(ConstIterator start, const size_t size)
//{
//  size_t less = std::min(size, this->Capacity());
//  std::copy(start, start + less, p_Begin());
//  p_FillWith(p_Begin() + less, this->BLANK);
//}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





//***** РЕАЛИЗАЦИЯ СТРОКИ **********************************************************************************************

istring::istring(size_t capacity)                                                                                       // SPECIAL MEMBER FUNCTIONS
       : m_data(new value_type[capacity]),
         m_capacity(capacity)
{
  Clear();
}


istring::~istring()
{
  delete [] m_data;
}



istring::const_iterator istring::begin() const { return m_data; }
istring::const_iterator istring::end()   const { return m_data + m_capacity; }

istring::iterator istring::begin()       { return m_data; }
istring::iterator istring::end()         { return m_data + m_capacity; }

size_t istring::capacity() const        { return m_capacity; }

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





//***** ЯВНОЕ ИНСТАНЦИРОВАНИЕ ******************************************************************************************
template class istring_base_const<istring>;
template class istring_base<istring>;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





OLI_NAMESPACE_END //////////////////////////////////////////////////////////////////////////////////////////////////////

