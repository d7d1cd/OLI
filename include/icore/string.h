/*
     _____ __    _____
    |     |  |  |_   _|  C++ Object Libraries for IBM i
    |  |  |  |__ _| |_   File:   ICORE/string.h
    |_____|_____|_____|  Author: Sergey Chebotarev

*/

#ifndef OLI_ICORE_STRING_H
#define OLI_ICORE_STRING_H

#include "ICORE/config__.h"
#include "STL/type_trait.h"
#include "STL/iterator.h"
#include "STL/cstddef.h"
#include <algorithm>
#include <array>
#include <string>

IBMI_NAMESPACE_BEGIN ///////////////////////////////////////////////////////////////////////////////////////////////////





//***** КОНСТАНТАНАЯ БАЗА СТРОКИ ***************************************************************************************
namespace string_detail {

template <typename T>
struct string_base_const;

template <template<std::size_t> class H, std::size_t C>
struct string_base_const<H<C>>
{
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



  enum blank_type : value_type { blank = '\x40' };
  enum : size_type { capacity = C };


  const_iterator         begin()   const { return const_iterator(data()); }
  const_iterator         cbegin()  const { return begin(); }
  const_iterator         end()     const { return const_iterator(stl::next(data(), C)); }
  const_iterator         cend()    const { return const_iterator(end()); }
  const_reverse_iterator rbegin()  const { return const_reverse_iterator(end()); }
  const_reverse_iterator crbegin() const { return rbegin(); }
  const_reverse_iterator rend()    const { return const_reverse_iterator(begin()); }
  const_reverse_iterator crend()   const { return rend(); }



  const_pointer data() const { return static_cast<const H<C>*>(this)->data_; }



  /*
     Длина строки
  */
  size_type length() const {
    std::tr1::array<value_type, C> empty;
    std::memset(empty.begin(), blank, C);

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


  /**
   * Проверка содержимого строки на пустоту
   * Пустая строка заполнена символом blank
   */
  bool empty() const {
    std::tr1::array<value_type, C> empty;
    std::memset(empty.begin(), blank, C);
    return !std::memcmp(data(), empty.begin(), C);
  }



  /* Получение стандартной строки
  */ std::string str() const { return std::string(begin(), length()); }

  /* Приведение к стандартной строке
  */ operator std::string () const { return str(); }


  /**
   *
   */
  int compare(const_pointer ptr, size_type size) const {
    int result = traits_type::compare(data(), ptr, std::min(static_cast<size_type>(capacity), size));
    if (result != 0)
        return result;
    if (capacity < size)
        return -1;
    if (capacity > size)
        return 1;
    return 0;
  }

  template <typename T>
  int compare(const string_base_const<T>& str) const {
    return compare(str.data(), str.capacity);
  }
};
} // namespace string_detail ///////////////////////////////////////////////////////////////////////////////////////////





//***** ИЗМЕНЯЕМАЯ БАЗА СТРОКИ *****************************************************************************************
namespace string_detail {

template <typename T>
struct string_base;

template <template<std::size_t> class H, std::size_t C>
struct string_base<H<C>> : public string_base_const<H<C>>
{
  private:
  typedef string_base_const<H<C>> base;



  public:
  using    base::begin;
  using    base::end;
  using    base::rbegin;
  using    base::rend;
  using    base::data;

  typename base::iterator         begin()  { return data(); }
  typename base::iterator         end()    { return stl::next(data(), C); }
  typename base::reverse_iterator rbegin() { return base::reverse_iterator(end()); }
  typename base::reverse_iterator rend()   { return base::reverse_iterator(begin()); }
  typename base::pointer          data()   { return static_cast<H<C>*>(this)->data_; }

  void clear() { fill_tail(begin()); }


  /**
   * Копирование в строку
   * Это основной метод копирования в строку, все остальные вызывают его
   */
  H<C>& assign(typename base::const_pointer start, typename base::size_type size) {
    if (start != data()) {
      auto less = std::min(size, C);
      std::memcpy(begin(), start, less);
      fill_tail(stl::next(begin(), less));
    }
    return *static_cast<H<C>*>(this);
  }

  template <typename T>
  H<C>& assign(const string_base_const<T>& src) {
    return assign(src.data(), src.capacity);
  }

  H<C>& assign(typename base::const_pointer src) {
    return assign(src, src == nullptr ? 0 : std::strlen(src));
  }

  H<C>& assign(const std::string& src) {
    return assign(src.c_str(), src.length());
  }

  template <typename T>
  H<C>& operator= (const string_base_const<T>& rhs) {
    return assign(rhs);
  }

  H<C>& operator= (typename base::const_pointer rhs) {
    return assign(rhs);
  }

  H<C>& operator= (const std::string& rhs) {
    return assign(rhs);
  }


  private:
  void fill_tail(typename base::iterator whence) {                                                                      // Заполнение хвоста
    std::memset(whence, base::blank, std::distance(whence, end()));
  }
};
} // namespace string_detail ///////////////////////////////////////////////////////////////////////////////////////////





//***** СТАТИЧЕСКАЯ СТРОКА *********************************************************************************************
template <std::size_t C>
struct string : public string_detail::string_base<string<C>>
{
  private:
  static_assert(C > 0, "String capacity cannot be 0");
  friend class string_detail::string_base_const<string<C>>;
  friend class string_detail::string_base<string<C>>;
  typedef string_detail::string_base<string<C>> base;


  public:
  using base::operator=;


  /**
   * Конструктор по умолчанию
   */
  explicit string() {
    base::clear();
  }

  /**
   * Конструктор от любого наследника string_base_const
   */
  template <typename T>
  explicit string(const string_detail::string_base_const<T>& src) {
    base::assign(src);
  }

  /**
   * Конструктор от С строки
   */
  explicit string(const char* src) {
    base::assign(src);
  }

  /**
   * Конструктор от стандартной строки
   */
  explicit string(const std::string& src) {
    base::assign(src);
  }

  /**
   * Конструктор копии
   */
  string(const string&) = default;

  /**
   *
   */
  string& operator= (const string& rhs) {
    return base::assign(rhs);
  }


  private:
  typename base::value_type data_[C];
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





//***** ССЫЛКА НА СТРОКУ ***********************************************************************************************
template <std::size_t C>
struct string_ref : public string_detail::string_base<string_ref<C>>
{
  private:
  static_assert(C > 0, "String capacity cannot be 0");
  friend class string_detail::string_base_const<string_ref<C>>;
  friend class string_detail::string_base<string_ref<C>>;
  typedef string_detail::string_base<string_ref<C>> base;



  public:
  using base::operator=;


  /**
   * Конструктор от массива символов
   */
  template <typename base::size_type N>
  explicit string_ref(typename base::value_type (&src)[N], bool clear = false)
  : data_(src) {
    static_assert(C <= N, "");
    if (clear)
      base::clear();
  }

  /**
   * Конструктор от любого наследника strign_base
   */
  template <typename T>
  explicit string_ref(string_detail::string_base<T>& src)
  : data_(src.data()) {
    static_assert(C <= src.capacity, "");
  }

  /**
   * Конструктор копии
   */
  string_ref(const string_ref&) = default;

  /**
   * string_ref ведет себя как ссылка, поэтому оператор присваивания из такого же типа должен копировать данные,
   * а не изменять ссылку на управляемый контейнер
   */
  string_ref& operator= (const string_ref& rhs) {
    return base::assign(rhs);
  }


  private:
  typename base::pointer data_;
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





//***** ПРЕДСТАВЛЕНИЕ НА СТРОКУ ****************************************************************************************
template <std::size_t C>
struct string_view : public string_detail::string_base_const<string_view<C>>
{
  private:
  friend class string_detail::string_base_const<string_view<C>>;
  typedef string_detail::string_base_const<string_view<C>> base;


  public:
  /**
   * Конструктор от массива символов
   */
  template <typename base::size_type N>
  explicit string_view(const typename base::value_type (&src)[N])
  : data_(src) {
    static_assert(C <= N, "");
  }

  /**
   * Конструктор от любого наследника string_base_const
   */
  template <typename T>
  explicit string_view(const string_detail::string_base_const<T>& src)
  : data_(src.data()) {
    static_assert(C <= src.capacity, "");
  }

  /**
   * Конструктор копии
   */
  string_view(const string_view& src) = default;

  /**
   *
   */
  string_view& operator= (const string_view&) = delete;


  private:
  typename base::const_pointer data_;
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





//**********************************************************************************************************************

/**
 * Сложение строк
 */
template <typename LT, typename RT>
auto operator+ (const string_detail::string_base_const<LT>& lhs, const string_detail::string_base_const<RT>& rhs)
  -> string<LT::capacity + RT::capacity>
{
  string<LT::capacity + RT::capacity> result(lhs);
  std::memcpy(result.begin() + LT::capacity, rhs.begin(), RT::capacity);
  return result;
}


/**
 * Сравнение строк
 */
//template <typename LT, typename RT>
//bool operator== (const string_detail::string_base_const<LT>& lhs, const string_detail::string_base_const<RT>& rhs)
//{
//  constexpr std::size_t tail_size = LT::capacity >= RT::capacity ?
//                                   (LT::capacity - RT::capacity) : (RT::capacity - LT::capacity);
//  std::tr1::array<LT::value_type, tail_size> empty;
//  return true;
//}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



IBMI_NAMESPACE_END /////////////////////////////////////////////////////////////////////////////////////////////////////

#endif // OLI_ICORE_STRING_H

