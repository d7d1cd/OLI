/*
     _____ __    _____
    |     |  |  |_   _|  C++ Object Libraries for IBM i
    |  |  |  |__ _| |_   File:   ICORE/string.h
    |_____|_____|_____|  Author: Sergey Chebotarev

*/

#ifndef OLI_ICORE_STRING_H
#define OLI_ICORE_STRING_H

#include "ICORE/define.h"
#include "STL/type_trait.h"
#include "STL/iterator.h"
#include "STL/cstddef.h"
#include <algorithm>
#include <array>
#include <string>

IBMI_NAMESPACE_BEGIN ///////////////////////////////////////////////////////////////////////////////////////////////////





//***** КОНСТАНТАНАЯ БАЗА СТРОКИ ***************************************************************************************
namespace string_detail {

template <typename Heir, std::size_t N = 0>
class string_base_const
{
  // MEMBER TYPES ------------------------------------------------------------------------------------------------------
  public:
  typedef char value_type;
  typedef std::char_traits<value_type> traits_type;
  typedef std::size_t size_type;
  typedef std::ptrdiff_t difference_type;
  typedef value_type* pointer;
  typedef const value_type* const_pointer;
  typedef value_type& reference;
  typedef const value_type& const_reference;
  typedef pointer iterator;
  typedef const_pointer const_iterator;
  typedef std::reverse_iterator<iterator> reverse_iterator;
  typedef std::reverse_iterator<const_iterator> const_reverse_iterator;



  // CONSTANTS ---------------------------------------------------------------------------------------------------------
  enum blank_type : value_type { blank = '\x40' };



  // ITERATORS ---------------------------------------------------------------------------------------------------------
  public:
  const_iterator         begin()   const { return data(); }
  const_iterator         cbegin()  const { return begin(); }
  const_iterator         end()     const { return stl::next(data(), capacity()); }
  const_iterator         cend()    const { return end(); }
  const_reverse_iterator rbegin()  const { return const_reverse_iterator(end()); }
  const_reverse_iterator crbegin() const { return rbegin(); }
  const_reverse_iterator rend()    const { return const_reverse_iterator(begin()); }
  const_reverse_iterator crend()   const { return rend(); }



  // ELEMENT ACCESS ----------------------------------------------------------------------------------------------------
  const_pointer data() const { return static_cast<const Heir*>(this)->m_data; }



  // CAPACITY ----------------------------------------------------------------------------------------------------------
  /*
     Емкость строки
  */
  template <std::size_t N2 = N, typename stl::enable_if<N2 != 0, int>::type = 0>                                        // Емкость строки для
  size_type capacity() const {                                                                                          // string_base
    static_assert(N2 == N, "");
    return N2;
  }

  template <std::size_t N2 = N, typename stl::enable_if<N2 == 0, int>::type = 0>                                        // Емкость строки для
  size_type capacity() const {                                                                                          // string_ref, _view
    static_assert(N2 == N, "");
    return static_cast<const Heir*>(this)->m_capacity;
  }

  /*
     Длина строки
  */
  template <std::size_t N2 = N, typename stl::enable_if<N2 != 0, int>::type = 0>
  size_type length() const {
    static_assert(N2 == N, "");
    std::tr1::array<value_type, N> empty;
    std::memset(empty.begin(), blank, N);

    auto left = cbegin(), right = cend(), pos = cbegin();

    while (pos != right) {
      if (std::memcmp(pos, empty.begin(), std::distance(pos, right)))
        left = stl::next(pos);
      else
        right = pos;
      pos = stl::next(left, std::distance(left, right) / 2);
    }

    return std::distance(begin(), pos);
  }

  template <std::size_t N2 = N, typename stl::enable_if<N2 == 0, int>::type = 0>
  size_type length() const {
    static_assert(N2 == N, "");
    auto cit = stl::prev(cend()), cend = stl::prev(cbegin());                                                           // Реверс-итераторы не используются, так как с ними медленнее
    while (cit != cend && *cit == blank) --cit;
    return std::distance(cend, cit);
  }



  // OPERATIONS --------------------------------------------------------------------------------------------------------
  /* Получение стандартной строки
  */ std::string stdstr() const {
    return std::string(begin(), length());
  }
};
} // namespace string_detail ///////////////////////////////////////////////////////////////////////////////////////////





//***** ИЗМЕНЯЕМАЯ БАЗА СТРОКИ *****************************************************************************************
namespace string_detail {

template <typename Heir, std::size_t N = 0>
class string_base : public string_base_const<Heir, N>
{
  typedef string_base_const<Heir, N> base;



  // SPECIAL MEMBER FUNCTIONS ------------------------------------------------------------------------------------------
  public:
  /*
     Копирование в строку
  */
  Heir& assign(typename base::const_pointer start, typename base::size_type size) {                                     // Из указателя и длины
    auto less = std::min(size, base::capacity());
    std::memcpy(begin(), start, less);
    fill_tail(stl::next(begin(), less));
    return *static_cast<Heir*>(this);
  }

  Heir& assign(const char* src) {                                                                                       // Из C строки
    return assign(src, src == nullptr ? 0 : std::strlen(src));
  }

  Heir& assign(const std::string& src) {                                                                                // Из стандартной строки
    return assign(src.c_str(), src.length());
  }



  // ITERATORS ---------------------------------------------------------------------------------------------------------
  using    base::begin;
  using    base::end;
  using    base::rbegin;
  using    base::rend;
  typename base::iterator         begin()  { return data(); }
  typename base::iterator         end()    { return stl::next(data(), base::capacity()); }
  typename base::reverse_iterator rbegin() { return base::reverse_iterator(end()); }
  typename base::reverse_iterator rend()   { return base::reverse_iterator(begin()); }



  // ELEMENT ACCESS ----------------------------------------------------------------------------------------------------
  using    base::data;
  typename base::pointer data() { return static_cast<Heir*>(this)->m_data; }



  // OPERATIONS --------------------------------------------------------------------------------------------------------
  void clear() { fill_tail(begin()); }


  // SERVICE -----------------------------------------------------------------------------------------------------------
  private:
  void fill_tail(typename base::iterator whence) {                                                                      // Заполнение хвоста
    std::memset(whence, base::blank, std::distance(whence, end()));
  }
};
} // namespace string_detail ///////////////////////////////////////////////////////////////////////////////////////////





//***** СТАТИЧЕСКАЯ СТРОКА *********************************************************************************************
template <std::size_t N>
class string : public string_detail::string_base<string<N>, N>
{
  static_assert(N > 0, "String capacity cannot be 0");
  friend class string_detail::string_base_const<string<N>, N>;
  friend class string_detail::string_base<string<N>, N>;
  typedef string_detail::string_base<string<N>, N> base;
  typename base::value_type m_data[N];



  // SPECIAL MEMBER FUNCTIONS ------------------------------------------------------------------------------------------
  public:
  /*
     Конструкторы
  */
  string()                       { base::clear(); }                                                                     // По умолчанию
  string(const char* src)        { base::assign(src); }                                                                 // Из С строки
  string(const std::string& src) { base::assign(src); }                                                                 // Из стандартной строки
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





//***** ССЫЛКА НА СТРОКУ ***********************************************************************************************
class string_ref : public string_detail::string_base<string_ref>
{
  friend class string_detail::string_base_const<string_ref>;
  friend class string_detail::string_base<string_ref>;
  typedef string_detail::string_base<string_ref> base;
  base::pointer m_data;
  base::size_type m_capacity;



  // SPECIAL MEMBER FUNCTIONS ------------------------------------------------------------------------------------------
  public:
  /* Конструктор от указателя на начало памяти и доступного размера (емкость строки)
     Параметр clear позволяет выполнить очистку памяти, передаваемую в управление
  */ string_ref(base::pointer begin, base::size_type capacity, bool clear = false)
     : m_data(begin), m_capacity(capacity) {
    if (clear)
      base::clear();
  }

  /* Конструктор от массива
     Параметр clear позволяет выполнить очистку памяти, передаваемую в управление
  */ template <std::size_t N>
     string_ref(base::value_type (&src)[N], bool clear = false)
     : string_ref(src, N, clear) {}

  string_ref(const string_ref&) = delete;
  string_ref& operator=(const string_ref&) = delete;
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





//***** ПРЕДСТАВЛЕНИЕ НА СТРОКУ ****************************************************************************************
class string_view : public string_detail::string_base_const<string_view>
{
  friend class string_detail::string_base_const<string_view>;
  typedef string_detail::string_base_const<string_view> base;
  base::const_pointer m_data;
  base::size_type m_capacity;



  // SPECIAL MEMBER FUNCTIONS ------------------------------------------------------------------------------------------
  public:
  template <typename T, std::size_t N>                                                                                  // Конструктор из массива
  string_view(T (&src)[N])
  : m_data(src), m_capacity(N) {}

  string_view(const string_view&) = delete;
  string_view& operator=(const string_view&) = delete;
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



IBMI_NAMESPACE_END /////////////////////////////////////////////////////////////////////////////////////////////////////

#endif // OLI_ICORE_STRING_H

