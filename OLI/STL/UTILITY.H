/*
     _____ _____ __
    |   __|_   _|  |     Standard library implementation for IBM i
    |__   | | | |  |__   File:   STL/utility.h
    |_____| |_| |_____|  Author: Sergey Chebotarev

*/

#ifndef OLI_STL_UTILITY_H_
#define OLI_STL_UTILITY_H_

#include "STL/typetraits.h"

STL_NAMESPACE_BEGIN ////////////////////////////////////////////////////////////////////////////////////////////////////





//***** std::move ******************************************************************************************************
//
// Since C++11
// Реализовано по причине отсутствия в std IBM i
//

template <typename T>
typename remove_reference<T>::type&& move(T&& t)
{
  return static_cast<typename remove_reference<T>::type&&>(t);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





//***** std::forward ***************************************************************************************************
//
// Since C++11
// Реализовано по причине отсутствия в std IBM i
//

template <typename T>
T&& forward(typename remove_reference<T>::type& Arg)
{
  return static_cast<T&&>(Arg);
}

template <typename T>
T&& forward(typename remove_reference<T>::type&& Arg)
{
  return static_cast<T&&>(Arg);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





STL_NAMESPACE_END //////////////////////////////////////////////////////////////////////////////////////////////////////

#endif // OLI_STL_UTILITY_H_

