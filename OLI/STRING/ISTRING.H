/*
     _____ __    _____
    |     |  |  |_   _|  C++ Object Library for IBM i
    |  |  |  |__ _| |_   File:   STRING/istring.h
    |_____|_____|_____|  Author: S.N. Chebotarev

*/

#ifndef OLI_STRING_ISTRING_H_
#define OLI_STRING_ISTRING_H_

#include "CORE/olidef.h"
#include <cstddef>                                                              // size_t
#include <iterator>                                                             // std::reverse_iterator

OLI_NAMESPACE_BEGIN ////////////////////////////////////////////////////////////



//***** ПЕРЕОПРЕДЕЛЕНИЯ ТИПОВ **************************************************
namespace {

typedef char Char;                                                              // Тип символа строки
typedef const Char* ConstPointer;                                               // Указатель на константу
typedef ConstPointer ConstIterator;                                             // Итератор на константу
typedef std::reverse_iterator<ConstIterator> ConstReverseIterator;              // Обратный итератор на константу
typedef Char* Pointer;                                                          // Указатель
typedef Pointer Iterator;                                                       // Итератор
typedef std::reverse_iterator<Iterator> ReverseIterator;                        // Обратный итератор

}
////////////////////////////////////////////////////////////////////////////////





//***** КОНСТАНТНАЯ БАЗА СТРОКИ ************************************************
template <typename Heir>
class IStringBaseConst
{
  // КОНСТАНТЫ -----------------------------------------------------------------
  public:
  static const Char BLANK = '\x40';                                             // Символ пустоты (пробел в строке)



  // ИТЕРАТОРЫ -----------------------------------------------------------------
  public:
  ConstReverseIterator Rbegin() const;                                          // Обратный итератор на начало константной строки
  ConstReverseIterator Rend() const;                                            // Обратный итератор на конец константной строки



  // СВОЙСТВА ------------------------------------------------------------------
  public:
  size_t Capacity() const;                                                      // Емкость строки
  size_t Length() const;                                                        // Длина строки



  // СЕРВИС --------------------------------------------------------------------
  private:
  ConstIterator p_Begin() const;                                                // Начало строки
  ConstIterator p_End() const;                                                  // Конец строки
};
////////////////////////////////////////////////////////////////////////////////





//***** БАЗА СТРОКИ ************************************************************
template <typename Heir>
class IStringBase : public IStringBaseConst<Heir>
{
  // ИТЕРАТОРЫ -----------------------------------------------------------------
  public:
  ReverseIterator Rbegin();                                                     // Обратный итератор на начало строки
  ReverseIterator Rend();                                                       // Обратный итератор на конец строки



  // ОПЕРАТОРЫ -----------------------------------------------------------------
  public:
  Heir& operator =(const char*);                                                // Присваивание из строки в стиле С



  // МОДИФИКАТОРЫ --------------------------------------------------------------
  public:
  void Clear();                                                                 // Очистка строки



  // СЕРВИС --------------------------------------------------------------------
  private:
  Iterator p_Begin();                                                           // Начало строки
  Iterator p_End();                                                             // Конец строки
  void     p_FillWith(Iterator whence, Char agg);                               // Заполнение строки
  void     p_CopyFrom(ConstIterator start, const size_t size);                  // Копирование в строку
};
////////////////////////////////////////////////////////////////////////////////





//***** СТРОКА *****************************************************************
class IString : public IStringBase<IString>
{
  // СОЗДАНИЕ ------------------------------------------------------------------
  public:
  explicit IString(const size_t capacity)                                       // Создание пустой строки
             : m_data(new Char[capacity]),                                      //
               m_capacity(capacity) {                                           //
    Clear();                                                                    //
  }                                                                             //

  virtual ~IString() {                                                          // Деструктор
    delete [] m_data;                                                           //
  }                                                                             //



  // ИТЕРАТОРЫ -----------------------------------------------------------------
  public:
  ConstIterator Begin() const { return m_data; }                                // Итератор на начало константной строки
  ConstIterator End()   const { return m_data + m_capacity; }                   // Итератор на конец константной строки
  Iterator      Begin()       { return m_data; }                                // Итератор на начало строки
  Iterator      End()         { return m_data + m_capacity; }                   // Итератор на конец строки



  // ОПЕРАТОРЫ -----------------------------------------------------------------
  public:
  using IStringBase<IString>::operator=;                                        // Операторы присваивания из базовго класса



  // ДАННЫЕ --------------------------------------------------------------------
  private:
  Pointer m_data;                                                               // Начало хранилища строки
  size_t  m_capacity;                                                           // Емкость строки
};
////////////////////////////////////////////////////////////////////////////////





//***** ЗАПРЕТ ИНСТАНЦИРОВАНИЯ *************************************************
extern template class IStringBaseConst<IString>;
extern template class IStringBase<IString>;
////////////////////////////////////////////////////////////////////////////////



OLI_NAMESPACE_END //////////////////////////////////////////////////////////////

#endif // OLI_STRING_ISTRING_H_

