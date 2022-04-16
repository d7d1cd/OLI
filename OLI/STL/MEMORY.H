/*
     _____ _____ __
    |   __|_   _|  |     Standard library implementation for IBM i
    |__   | | | |  |__   File:   STL/memory.h
    |_____| |_| |_____|  Author: Sergey Chebotarev

*/

#ifndef OLI_STL_MEMORY_H
#define OLI_STL_MEMORY_H

#include "STL/utility.h"
#include <memory>

STL_NAMESPACE_BEGIN ////////////////////////////////////////////////////////////////////////////////////////////////////





//***** std::make_shared ***********************************************************************************************
//
// Реализовано по причине отсутствия в std IBM i
//

template <typename T, class... Types>
std::tr1::shared_ptr<T> make_shared(Types&&... args)
{
  return std::tr1::shared_ptr<T>(new T(stl::forward<Types>(args)...));
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





STL_NAMESPACE_END //////////////////////////////////////////////////////////////////////////////////////////////////////

#endif // OLI_STL_MEMORY_H

