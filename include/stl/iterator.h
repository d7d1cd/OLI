/*
     _____ _____ __
    |   __|_   _|  |     Standard library implementation for IBM i
    |__   | | | |  |__   File:   STL/iterator.h
    |_____| |_| |_____|  Author: Sergey Chebotarev

*/

#ifndef OLI_STL_ITERATOR_H
#define OLI_STL_ITERATOR_H

#include "STL/config__.h"
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





//***** std::next ******************************************************************************************************
//
// Реализовано по причине отсутствия в std IBM i
//

template <typename InputIt>
inline InputIt next(InputIt it, typename std::iterator_traits<InputIt>::difference_type n = 1)
{
  std::advance(it, n);
  return it;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





//***** std::prev ******************************************************************************************************
//
// Реализовано по причине отсутствия в std IBM i
//

template <typename BidirIt>
inline BidirIt prev(BidirIt it, typename std::iterator_traits<BidirIt>::difference_type n = 1)
{
  std::advance(it, -n);
  return it;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





STL_NAMESPACE_END //////////////////////////////////////////////////////////////////////////////////////////////////////

#endif // OLI_STL_ITERATOR_H

