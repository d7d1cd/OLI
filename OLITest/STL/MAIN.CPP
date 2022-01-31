////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  Тестирование классов группы STL                                           //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include "OLITEST/testing.h"

void RemoveReference();
void Forward();

int main()
{
  try
  {
    RemoveReference();
    Forward();
  }
  catch (std::exception& e) { olitest::PrintException(e); }
  catch (...)               { olitest::PrintUnexpectedException(); }
}

