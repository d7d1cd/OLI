/*
     _____ __    _____
    |     |  |  |_   _|  C++ Object Libraries for IBM i
    |  |  |  |__ _| |_   File:   ISTRING/istring.hpp
    |_____|_____|_____|  Author: Sergey Chebotarev

*/

#ifndef OLI_ISTRING_ISTRING_HPP
#define OLI_ISTRING_ISTRING_HPP

#include "base"

OLI_NAMESPACE_BEGIN ////////////////////////////////////////////////////////////////////////////////////////////////////





//***** СТРОКА *********************************************************************************************************
template <std::size_t N>
class istring : public istring_detail::istring_base<istring<N>>
{
  public:                                                                                                               // SPECIAL MEMBER FUNCTIONS


  public:                                                                                                               // ELEMENT ACCESS


  private:                                                                                                              // DATA MEMBERS
  value_type m_data[N];
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





OLI_NAMESPACE_END //////////////////////////////////////////////////////////////////////////////////////////////////////

#endif // OLI_ISTRING_ISTRING_HPP

