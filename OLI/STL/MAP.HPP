/*
     _____ _____ __
    |   __|_   _|  |     Standard library implementation for IBM i
    |__   | | | |  |__   File:   STL/map.hpp
    |_____| |_| |_____|  Author: Sergey Chebotarev

*/

#ifndef OLI_STL_MAP_H
#define OLI_STL_MAP_H

#include "STL/xiter.hpp"
#include <map>

STL_NAMESPACE_BEGIN ////////////////////////////////////////////////////////////////////////////////////////////////////





//***** std::map *******************************************************************************************************
//
// Изменения и добавления в стандартную реализацию std IBM i
//

template <typename Key, typename T,
          typename Compare = std::less<Key>,
          typename Allocator = std::allocator<std::pair<const Key, T> > >
class map : public std::map<Key, T, Compare, Allocator>
{
  public:
  typedef std::map<Key, T, Compare, Allocator> base;


  // std::map::operator[]
  // Реализовано по причине бага в std IBM i: независимо от наличия ключа в контейнере
  // вызывается конструктор по умолчанию для T.
  // Обсуждение: https://www.cyberforum.ru/cpp/thread2548864.html
  typename base::mapped_type& operator[](const typename base::key_type& key_val)
  {
    auto where = base::lower_bound(key_val);
    if (where == base::end() ||
        base::comp(key_val, base::_Key(where._Mynode())))
      where = base::insert(where, typename base::value_type(key_val, typename base::mapped_type()));

    return where->second;
  }


  // std::map::at
  // Реализовано по причине отсутствия в std IBM i
  typename base::mapped_type& at(const typename base::key_type& key_val)
  {
    auto where = base::lower_bound(key_val);
    if (where == base::end() ||
        base::comp(key_val, base::_Key(where._Mynode())))
      throw std::out_of_range("invalid map<K, T> key");

    return where->second;
  }

  const typename base::mapped_type& at(const typename base::key_type& key_val) const
  {
    auto where = base::lower_bound(key_val);
    if (where == base::end() ||
        base::comp(key_val, base::_Key(where._Mynode())))
      throw std::out_of_range("invalid map<K, T> key");

    return where->second;
  }
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





STL_NAMESPACE_END //////////////////////////////////////////////////////////////////////////////////////////////////////

#endif // OLI_STL_MAP_H

