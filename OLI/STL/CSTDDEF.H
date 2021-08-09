/*
     _____ _____ __
    |   __|_   _|  |     Standard library implementation for IBM i
    |__   | | | |  |__   File:   STL/cstddef.h
    |_____| |_| |_____|  Author: Sergey Chebotarev

*/

#ifndef OLI_STL_CSTDDEF_H_
#define OLI_STL_CSTDDEF_H_

#include "CORE/olidef.h"
#include <cstddef>

STL_NAMESPACE_BEGIN ////////////////////////////////////////////////////////////



// std::nullptr_t --------------------------------------------------------------
typedef decltype(nullptr) nullptr_t;



STL_NAMESPACE_END //////////////////////////////////////////////////////////////

#endif // OLI_STL_CSTDDEF_H_

