/*
     _____ _____ __
    |   __|_   _|  |     Standard library implementation for IBM i
    |__   | | | |  |__   File:   STL/iterator.h
    |_____| |_| |_____|  Author: Sergey Chebotarev

*/

#ifndef OLI_STL_ITERATOR_H
#define OLI_STL_ITERATOR_H

#include "CORE/olidef.h"
#include <iterator>

STL_NAMESPACE_BEGIN ////////////////////////////////////////////////////////////////////////////////////////////////////





//***** std::begin *****************************************************************************************************
//
// Реализовано по причине отсутствия в std IBM i
// Из-за бага decltype возвращаемый тип не decltype(cont.begin())
//

template <typename Container>
inline auto begin(Container& cont) -> typename Container::iterator
{
  return cont.begin();
}

template <typename Container>
inline auto begin(const Container& cont) -> typename Container::const_iterator
{
  return cont.begin();
}
	
template <typename T, size_t Size>
inline constexpr T* begin(T (&Array)[Size])
{
  return Array;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





//***** std::end *******************************************************************************************************
//
// Реализовано по причине отсутствия в std IBM i
// Из-за бага decltype возвращаемый тип не decltype(cont.end())
//

template <typename Container>
inline auto end(Container& cont) -> typename Container::iterator
{
  return cont.end();
}

template <typename Container>
inline auto end(const Container& cont) -> typename Container::const_iterator
{
  return cont.end();
}
	
template <typename T, size_t Size>
inline constexpr T* end(T (&Array)[Size])
{
  return Array + Size;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





STL_NAMESPACE_END //////////////////////////////////////////////////////////////////////////////////////////////////////

#endif // OLI_STL_ITERATOR_H

