/*
     _____ _____ __
    |   __|_   _|  |     Standard library implementation for IBM i
    |__   | | | |  |__   File:   STL/type_trait.h
    |_____| |_| |_____|  Author: Sergey Chebotarev

*/

#ifndef OLI_STL_TYPE_TRAITS_H_
#define OLI_STL_TYPE_TRAITS_H_

#include "CORE/olidef.h"

#include <type_traits>

STL_NAMESPACE_BEGIN ////////////////////////////////////////////////////////////////////////////////////////////////////





//***** std::remove_reference ******************************************************************************************
//
// Since C++11
// Реализовано по причине отсутствия в std IBM i удаления правой ссылки
//

template <typename T>
struct remove_reference      { typedef T type; };

template <typename T>
struct remove_reference<T&>  { typedef T type; };

template <typename T>
struct remove_reference<T&&> { typedef T type; };
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





//***** std::enable_if *************************************************************************************************
//
// Since C++11
// Реализовано по причине отсутствия в std IBM i
//

template<bool B, typename T = void>
struct enable_if {};

template<typename T>
struct enable_if<true, T> { typedef T type; };
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





STL_NAMESPACE_END //////////////////////////////////////////////////////////////////////////////////////////////////////

#endif // OLI_STL_TYPE_TRAITS_H_

