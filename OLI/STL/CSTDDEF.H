/*
     _____ _____ __
    |   __|_   _|  |     Standard library implementation for IBM i
    |__   | | | |  |__   File:   STL/cstddef.h
    |_____| |_| |_____|  Author: Sergey Chebotarev

*/

#ifndef OLI_STL_CSTDDEF_H
#define OLI_STL_CSTDDEF_H

#include "ICORE/olidef.h"
#include <cstddef>

STL_NAMESPACE_BEGIN ////////////////////////////////////////////////////////////////////////////////////////////////////





//***** std::nullptr_t *************************************************************************************************
//
// Since C++11
// Реализовано по причине отсутствия в std IBM i
//

typedef decltype(nullptr) nullptr_t;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





STL_NAMESPACE_END //////////////////////////////////////////////////////////////////////////////////////////////////////

#endif // OLI_STL_CSTDDEF_H

