#include "STL/typetraits.h"
#include "OLITEST/testing.h"



//***** ТЕСТИРОВАНИЕ remove_reference ******************************************
void RemoveReference()
{
  olitest::PrintStartTests("stl::remove_reference");

  bool same = std::tr1::is_same<int, int>::value;
  assert(same);

  same = std::tr1::is_same<int, int&>::value;
  assert(!same);

  same = std::tr1::is_same<int, int&&>::value;
  assert(!same);

  same = std::tr1::is_same<int, stl::remove_reference<int>::type>::value;
  assert(same);

  same = std::tr1::is_same<int, stl::remove_reference<int&>::type>::value;
  assert(same);

  same = std::tr1::is_same<int, stl::remove_reference<int&&>::type>::value;
  assert(same);
}
////////////////////////////////////////////////////////////////////////////////

