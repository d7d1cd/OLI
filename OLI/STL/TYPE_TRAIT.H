/*
     _____ _____ __
    |   __|_   _|  |     Standard library implementation for IBM i
    |__   | | | |  |__   File:   STL/type_trait.h
    |_____| |_| |_____|  Author: Sergey Chebotarev

*/

#ifndef OLI_STL_TYPE_TRAITS_H
#define OLI_STL_TYPE_TRAITS_H

#include "CORE/olidef.h"
#include <type_traits>

STL_NAMESPACE_BEGIN ////////////////////////////////////////////////////////////////////////////////////////////////////





//***** std::remove_reference ******************************************************************************************
//
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
// Реализовано по причине отсутствия в std IBM i
//

template <bool B, typename T = void>
struct enable_if {};

template <typename T>
struct enable_if<true, T> { typedef T type; };
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





//***** std::conditional ***********************************************************************************************
//
// Реализовано по причине отсутствия в std IBM i
//

template <bool, typename T1, typename T2>
struct conditional { typedef T2 type; };

template <typename T1, typename T2>
struct conditional<true, T1, T2> { typedef T1 type; };
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





//***** std::make_signed, std::make_unsigned ***************************************************************************
//
// Реализовано по причине отсутствия в std IBM i
//

namespace detail_make_signed_unsigned {

template<typename T>
struct Change_sign
{
  static_assert(
    ((std::tr1::is_integral<T>::value || std::tr1::is_enum<T>::value) &&
      !std::tr1::is_same<T, bool>::value),
    "make_signed<T>/make_unsigned<T> require that T shall be a (possibly "
    "cv-qualified) integral type or enumeration but not a bool type."
  );

  typedef
    typename conditional<std::tr1::is_same<T, signed char   >::value ||
               std::tr1::is_same<T, unsigned char >::value, signed char,
    typename conditional<std::tr1::is_same<T, short         >::value ||
               std::tr1::is_same<T, unsigned short>::value, short,
    typename conditional<std::tr1::is_same<T, int           >::value ||
               std::tr1::is_same<T, unsigned int  >::value, int,
    typename conditional<std::tr1::is_same<T, long          >::value ||
               std::tr1::is_same<T, unsigned long >::value, long,
    typename conditional<std::tr1::is_same<T, long long     >::value ||
               std::tr1::is_same<T, unsigned long long>::value, long long,
    typename conditional<sizeof (T) == sizeof (signed char), signed char,
    typename conditional<sizeof (T) == sizeof (short      ), short,
    typename conditional<sizeof (T) == sizeof (int        ), int,
    typename conditional<sizeof (T) == sizeof (long       ), long, long long>
    ::type>::type>::type>::type>::type>::type>::type>::type>::type Signed;

  typedef
    typename conditional<std::tr1::is_same<Signed, signed char>::value,
      unsigned char,
    typename conditional<std::tr1::is_same<Signed, short      >::value,
      unsigned short,
    typename conditional<std::tr1::is_same<Signed, int        >::value,
      unsigned int,
    typename conditional<std::tr1::is_same<Signed, long       >::value,
      unsigned long, unsigned long long>
    ::type>::type>::type>::type Unsigned;
};

template<typename T>
struct Change_sign<const T>
{
  typedef const typename Change_sign<T>::Signed Signed;
  typedef const typename Change_sign<T>::Unsigned Unsigned;
};

template<typename T>
struct Change_sign<volatile T>
{
  typedef volatile typename Change_sign<T>::Signed Signed;
  typedef volatile typename Change_sign<T>::Unsigned Unsigned;
};

template<typename T>
struct Change_sign<const volatile T>
{
  typedef const volatile typename Change_sign<T>::Signed Signed;
  typedef const volatile typename Change_sign<T>::Unsigned Unsigned;
};
} // namespace detail_make_signed_unsigned {

template <typename T>
struct make_signed {
  typedef typename detail_make_signed_unsigned::Change_sign<T>::Signed type;
};

template <typename T>
struct make_unsigned {
  typedef typename detail_make_signed_unsigned::Change_sign<T>::Unsigned type;
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





STL_NAMESPACE_END //////////////////////////////////////////////////////////////////////////////////////////////////////

#endif // OLI_STL_TYPE_TRAITS_H

