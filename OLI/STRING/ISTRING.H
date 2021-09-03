/*
     _____ __    _____
    |     |  |  |_   _|  C++ Object Library for IBM i
    |  |  |  |__ _| |_   File:   STRING/istring.h
    |_____|_____|_____|  Author: Sergey Chebotarev

*/

#ifndef OLI_STRING_ISTRING_H
#define OLI_STRING_ISTRING_H

#include "CORE/olidef.h"
#include <cstddef>                                                                                                      // std::size_t, std::ptrdiff_t
#include <iterator>                                                                                                     // std::reverse_iterator

OLI_NAMESPACE_BEGIN ////////////////////////////////////////////////////////////////////////////////////////////////////





//***** КОНСТАНТНАЯ БАЗА СТРОКИ ****************************************************************************************
template <typename Heir>
class istring_base_const
{
  public:                                                                                                               // MEMBER TYPES
  typedef char value_type;                                                                                              // Тип символа строки
  typedef std::size_t size_type;                                                                                        // Размер
  typedef std::ptrdiff_t difference_type;                                                                               //
  typedef const value_type* const_pointer;                                                                              // Указатель на константу
  typedef const_pointer const_iterator;                                                                                 // Итератор на константу
  typedef std::reverse_iterator<const_iterator> const_reverse_iterator;                                                 // Обратный итератор на константу


  public:                                                                                                               // CONSTANTS
  static const value_type BLANK = '\x40';                                                                               // Бланк (символ пустоты в строке)


  public:                                                                                                               // ITERATORS
  const_reverse_iterator rbegin() const;                                                                                // Обратный итератор на начало константной строки
  const_reverse_iterator rend() const;                                                                                  // Обратный итератор на конец константной строки


  public:                                                                                                               // CAPACITY
//  size_t Length() const;                                                                                              // Длина строки


  private:                                                                                                              // SERVICE
  const_iterator begin() const;                                                                                         // Начало строки
  const_iterator end() const;                                                                                           // Конец строки
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





//***** БАЗА СТРОКИ ****************************************************************************************************
template <typename Heir>
class istring_base : public istring_base_const<Heir>
{
  public:                                                                                                               // MEMBER TYPES
  using typename istring_base_const<Heir>::value_type;                                                                  // Тип символа строки
  typedef value_type* pointer;                                                                                          // Указатель
  typedef pointer iterator;                                                                                             // Итератор
  typedef std::reverse_iterator<iterator> reverse_iterator;                                                             // Обратный итератор


  public:                                                                                                               // SPECIAL MEMBER FUNCTIONS
//  Heir& operator =(const char*);                                                                                      // Присваивание из строки в стиле С


  public:                                                                                                               // ITERATORS
  reverse_iterator rbegin();                                                                                            // Обратный итератор на начало строки
  reverse_iterator rend();                                                                                              // Обратный итератор на конец строки


  public:                                                                                                               // OPERATIONS
  void Clear();                                                                                                         // Очистка строки


  private:                                                                                                              // SERVICE
  iterator begin();                                                                                                     // Начало строки
  iterator end();                                                                                                       // Конец строки
  void fill_with(iterator whence, value_type agg);                                                                      // Заполнение строки
//  void     p_CopyFrom(ConstIterator start, const size_t size);                                                        // Копирование в строку
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





//***** СТРОКА *********************************************************************************************************
class istring : public istring_base<istring>
{
  public:                                                                                                               // SPECIAL MEMBER FUNCTIONS
  explicit istring(size_t capacity);                                                                                    // Создание пустой строки
  using istring_base<istring>::operator=;                                                                               // Операторы присваивания из базовго класса
  virtual ~istring();                                                                                                   // Деструктор


  public:                                                                                                               // ITERATORS
  const_iterator begin() const;                                                                                         // Итератор на начало константной строки
  const_iterator end() const;                                                                                           // Итератор на конец константной строки
  iterator begin();                                                                                                     // Итератор на начало строки
  iterator end();                                                                                                       // Итератор на конец строки


  public:                                                                                                               // CAPACITY
  size_t capacity() const;                                                                                              // Емкость строки


  private:                                                                                                              // DATA MEMBERS
  pointer m_data;                                                                                                       // Начало хранилища строки
  const size_t m_capacity;                                                                                              // Емкость строки
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





//***** ЗАПРЕТ ИНСТАНЦИРОВАНИЯ *****************************************************************************************
extern template class istring_base_const<istring>;
extern template class istring_base<istring>;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



OLI_NAMESPACE_END //////////////////////////////////////////////////////////////////////////////////////////////////////

#endif // OLI_STRING_ISTRING_H

