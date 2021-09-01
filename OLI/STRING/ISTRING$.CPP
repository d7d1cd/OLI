/*
     _____ __    _____
    |     |  |  |_   _|  C++ Object Library for IBM i
    |  |  |  |__ _| |_   File:   STRING/istring$.cpp
    |_____|_____|_____|  Author: Sergey Chebotarev

*/

#include "STRING/istring.h"
#include <algorithm>                                                            // std::fill

OLI_NAMESPACE_BEGIN ////////////////////////////////////////////////////////////



//***** КОНСТАНТНАЯ БАЗА СТРОКИ ************************************************

// ИТЕРАТОРЫ: ОБРАТНЫЙ ИТЕРАТОР НА НАЧАЛО СТРОКИ
template <typename Heir>
ConstReverseIterator IStringBaseConst<Heir>::Rbegin() const
{
  return ConstReverseIterator(p_End());
}



// ИТЕРАТОРЫ: ОБРАТНЫЙ ИТЕРАТОР НА КОНЕЦ СТРОКИ
template <typename Heir>
ConstReverseIterator IStringBaseConst<Heir>::Rend() const
{
  return ConstReverseIterator(p_Begin());
}



// СВОЙСТВА: ЕМКОСТЬ СТРОКИ
template <typename Heir>
size_t IStringBaseConst<Heir>::Capacity() const
{
  return p_End() - p_Begin();
}



// СВОЙСТВА: ДЛИНА СТРОКИ
template <typename Heir>
size_t IStringBaseConst<Heir>::Length() const
{
  size_t len = Capacity();

  for (auto it = Rbegin(), rend = Rend(); it != rend; --len)
    if (*it++ != BLANK) break;

  return len;
}



// СЕРВИС: НАЧАЛО КОНСТАНТНОЙ СТРОКИ
template <typename Heir>
ConstIterator IStringBaseConst<Heir>::p_Begin() const
{
  return static_cast<const Heir*>(this)->Begin();
}



// СЕРВИС: КОНЕЦ КОНСТАНТНОЙ СТРОКИ
template <typename Heir>
ConstIterator IStringBaseConst<Heir>::p_End() const
{
  return static_cast<const Heir*>(this)->End();
}

////////////////////////////////////////////////////////////////////////////////





//***** БАЗА СТРОКИ ************************************************************

// ИТЕРАТОРЫ: ОБРАТНЫЙ ИТЕРАТОР НА НАЧАЛО СТРОКИ
template <typename Heir>
ReverseIterator IStringBase<Heir>::Rbegin()
{
  return ReverseIterator(p_End());
}



// ИТЕРАТОРЫ: ОБРАТНЫЙ ИТЕРАТОР НА НАЧАЛО СТРОКИ
template <typename Heir>
ReverseIterator IStringBase<Heir>::Rend()
{
  return ReverseIterator(p_Begin());
}



// ОПЕРАТОРЫ: ПРИСВАИВАНИЕ ИЗ СТРОКИ В СТИЛЕ С
template <typename Heir>
Heir& IStringBase<Heir>::operator =(const char* rhs)
{
  p_CopyFrom(rhs, std::strlen(rhs));
  return *static_cast<Heir*>(this);
}



// МОДИФИКАТОРЫ: ОЧИСТКА СТРОКИ
template <typename Heir>
void IStringBase<Heir>::Clear()
{
  p_FillWith(p_Begin(), this->BLANK);
}



// СЕРВИС: НАЧАЛО СТРОКИ
template <typename Heir>
Iterator IStringBase<Heir>::p_Begin()
{
  return static_cast<Heir*>(this)->Begin();
}



// СЕРВИС: КОНЕЦ СТРОКИ
template <typename Heir>
Iterator IStringBase<Heir>::p_End()
{
  return static_cast<Heir*>(this)->End();
}



// СЕРВИС: ЗАПОЛНЕНИЕ СТРОКИ
template <typename Heir>
void IStringBase<Heir>::p_FillWith(Iterator whence, Char agg)
{
  std::fill(whence, p_End(), agg);
}



// СЕРВИС: КОПИРОВАНИЕ В СТРОКУ
template <typename Heir>
void IStringBase<Heir>::p_CopyFrom(ConstIterator start, const size_t size)
{
  size_t less = std::min(size, this->Capacity());
  std::copy(start, start + less, p_Begin());
  p_FillWith(p_Begin() + less, this->BLANK);
}
////////////////////////////////////////////////////////////////////////////////





//***** ЯВНОЕ ИНСТАНЦИРОВАНИЕ **************************************************
template class IStringBaseConst<IString>;
template class IStringBase<IString>;
////////////////////////////////////////////////////////////////////////////////



OLI_NAMESPACE_END //////////////////////////////////////////////////////////////

