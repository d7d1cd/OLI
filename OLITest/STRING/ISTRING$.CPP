#include "OLITEST/testing.h"
#include "STRING/istring.h"


//******************************************************************************
struct IString
{
  const size_t maxCapacity;

  IString(size_t);
  IString& operator= (const char*);
  ~IString() { olitest::PrintFunctionName(__FUNCTION__); }
};
////////////////////////////////////////////////////////////////////////////////





//***** ТЕСТИРОВАНИЕ КЛАССА IString ********************************************
void IString()
{
  olitest::PrintStartTests("oli::IString");

  struct IString iString(40000);                                                // Тестирование конструкторов
                                                                                //
  iString = "oli::IString::operator=(const char*)";                             // Тестирование операторов присваивания
                                                                                //
}
////////////////////////////////////////////////////////////////////////////////





//***** ТЕСТ КОНСТРУКТОРА ПО СОЗДАНИЮ ПУСТОЙ СТРОКИ ****************************
IString::IString(const size_t Capacity)
        : maxCapacity(Capacity)
{
  olitest::PrintFunctionName(__FUNCTION__);


  for (size_t capacity = 0; capacity <= maxCapacity; capacity += 100)
  {
    // Создание строк заданной емкости
    const oli::IString iStrConst(capacity);
    oli::IString iStr(capacity);

    // Проверка размера аллоцированной памяти
    assert((iStrConst.End() - iStrConst.Begin()) == capacity);
    assert((iStr.End() - iStr.Begin()) == capacity);

    // Проверка строк на пустоту
    for (auto it = iStrConst.Begin(); it != iStrConst.End(); ++it)
      assert(*it == oli::IString::BLANK);

    for (auto it = iStr.Begin(); it != iStr.End(); ++it)
      assert(*it == oli::IString::BLANK);
  }
}
////////////////////////////////////////////////////////////////////////////////





//***** ПРИСВАИВАНИЕ ИЗ СТРОКИ В СТИЛЕ СИ **************************************
struct IString& IString::operator= (const char* rhs)
{
  olitest::PrintFunctionName(__FUNCTION__);


  const size_t cstrLen = std::strlen(rhs);
  for (size_t capacity = 0; capacity <= maxCapacity; capacity += 100)
  {
    const char* cstr = rhs;
    size_t len = cstrLen;

    // Создание строк заданной емкости
    oli::IString iStr(capacity);
    iStr = cstr;

    // Проверка правильности скопированной строки
    auto it = iStr.Begin(), end = iStr.End();
//    while (csfor (; it != end; ++it)
//      assert(*it == *cstr++);

    // Проверка заполненности остатка строки пустотой
  }

  return *this;
}
////////////////////////////////////////////////////////////////////////////////

