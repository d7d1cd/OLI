/*
     _____ __    _____
    |     |  |  |_   _|  C++ Object Libraries for IBM i
    |  |  |  |__ _| |_   File:   ICORE/string.h
    |_____|_____|_____|  Author: Sergey Chebotarev

*/

#ifndef OLI_CORE_STRING_H
#define OLI_CORE_STRING_H

#include "STL/type_trait.h"
#include "STL/iterator.h"
#include <algorithm>
#include <array>

namespace ibmi { ///////////////////////////////////////////////////////////////////////////////////////////////////////





//***** ТИПЫ И КОНСТАНТЫ СТРОКИ ****************************************************************************************
namespace string_detail {

struct types_holder
{
  // MEMBER TYPES ------------------------------------------------------------------------------------------------------
  typedef char value_type;
  typedef std::tr1::array<value_type, 10> blanks_type;
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
  static const blanks_type blanks;
};
} // namespace string_detail ///////////////////////////////////////////////////////////////////////////////////////////





//***** КОНСТАНТАНАЯ БАЗА СТРОКИ ***************************************************************************************
namespace string_detail {

template <typename Heir, std::size_t N = 0>
class string_base_const : public types_holder
{
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

  size_type length() const {                                                                                            // Длина строки
    auto crit = crbegin(), end = crend();
    while (crit != end && *crit == blank) ++crit;
    return std::distance(crit, end);
  }

  size_type length2() const {
    return 0;
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
  Heir& assign(typename base::const_pointer start, typename base::size_type size) {                                     // Копирование из указателя и длины
    auto less = std::min(size, base::capacity());
    std::memcpy(begin(), start, less);
    fill_tail(stl::next(begin(), less));
    return *static_cast<Heir*>(this);
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
  string() {                                                                                                            // Конструктор по умолчанию
    base::clear();
  }
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
  template <typename T, std::size_t N>                                                                                  // Конструктор из массива
  string_ref(T (&src)[N], bool clear = false)
  : m_data(src), m_capacity(N) {
    if (clear)
      base::clear();
  }

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



} // namespace ibmi ////////////////////////////////////////////////////////////////////////////////////////////////////

#endif // OLI_CORE_STRING_H

