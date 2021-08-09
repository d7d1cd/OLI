////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  Тестирование классов группы String                                        //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include "OLITEST/testing.h"

void IString();

int main()
{
  try
  {
    IString();
  }
  catch (std::exception& e) { olitest::PrintException(e); }
  catch (...)               { olitest::PrintUnexpectedException(); }
}

