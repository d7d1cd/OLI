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
  typedef const value_type* const_pointer;                                                                              // Указатель на константу
  typedef const_pointer const_iterator;                                                                                 // Итератор на константу
  typedef std::reverse_iterator<const_iterator> const_reverse_iterator;                                                 // Обратный итератор на константу


  public:                                                                                                               // CONSTANTS
  static const value_type BLANK = '\x40';                                                                               // Бланк (символ пустоты в строке)


  public:                                                                                                               // SPECIAL MEMBER FUNCTIONS
  istring_base_const(size_type capacity);                                                                               // Конструктор


  public:                                                                                                               // ITERATORS
  const_iterator begin() const;                                                                                         // Начало строки
  const_iterator end() const;                                                                                           // Конец строки
  const_reverse_iterator rbegin() const;                                                                                // Обратный итератор на начало константной строки
  const_reverse_iterator rend() const;                                                                                  // Обратный итератор на конец константной строки


  public:                                                                                                               // CAPACITY
  size_type capacity() const;                                                                                           // Емкость строки
  size_type length() const;                                                                                             // Длина строки


  private:                                                                                                              // DATA MEMBERS AND SERVICE
  const size_type m_capacity;                                                                                           // Емкость строки
  const_pointer data() const;                                                                                           // Данные строки
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





//***** БАЗА СТРОКИ ****************************************************************************************************
template <typename Heir>
class istring_base : public istring_base_const<Heir>
{
  public:                                                                                                               // MEMBER TYPES
  using typename istring_base_const<Heir>::value_type;                                                                  // Тип символа строки
  using typename istring_base_const<Heir>::size_type;                                                                   // Размер
  using typename istring_base_const<Heir>::const_iterator;                                                              // Итератор на константу
  typedef value_type* pointer;                                                                                          // Указатель
  typedef pointer iterator;                                                                                             // Итератор
  typedef std::reverse_iterator<iterator> reverse_iterator;                                                             // Обратный итератор


  public:                                                                                                               // SPECIAL MEMBER FUNCTIONS
  istring_base(size_type capacity);                                                                                     // Конструктор
  Heir& operator=(const char* rhs);                                                                                     // Копирование из С строки


  public:                                                                                                               // ITERATORS
  iterator begin();                                                                                                     // Начало строки
  iterator end();                                                                                                       // Конец строки
  reverse_iterator rbegin();                                                                                            // Обратный итератор на начало строки
  reverse_iterator rend();                                                                                              // Обратный итератор на конец строки


  public:                                                                                                               // CAPACITY
  using istring_base_const<Heir>::capacity;                                                                             // Емкость


  public:                                                                                                               // OPERATIONS
  void clear();                                                                                                         // Очистка строки


  private:                                                                                                              // DATA MEMBERS AND SERVICE
  pointer data();                                                                                                       //
  void fill_with(iterator whence, value_type agg);                                                                      // Заполнение строки
  void copy_from(const_iterator start, const size_type size);                                                           // Копирование в строку
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





//***** СТРОКА *********************************************************************************************************
class istring : public istring_base<istring>
{
  public:                                                                                                               // SPECIAL MEMBER FUNCTIONS
  explicit istring(size_type capacity);                                                                                 // Создание пустой строки
  using istring_base<istring>::operator=;                                                                               // Операторы присваивания из базовго класса
  virtual ~istring();                                                                                                   // Деструктор


  public:                                                                                                               // ELEMENT ACCESS
  pointer data();                                                                                                       // Прямой доступ к
  const_pointer data() const;                                                                                           // строке


  private:                                                                                                              // DATA MEMBERS
  pointer m_data;                                                                                                       // Начало хранилища строки
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





//***** ЗАПРЕТ ИНСТАНЦИРОВАНИЯ *****************************************************************************************
extern template class istring_base_const<istring>;
extern template class istring_base<istring>;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





OLI_NAMESPACE_END //////////////////////////////////////////////////////////////////////////////////////////////////////

#endif // OLI_STRING_ISTRING_H

