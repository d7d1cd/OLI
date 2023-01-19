/*
     _____ __    _____
    |     |  |  |_   _|  C++ Object Libraries for IBM i
    |  |  |  |__ _| |_   File:   IOBJECT/fs.h
    |_____|_____|_____|  Author: Sergey Chebotarev

*/

#ifndef OLI_OBJECT_FS_H
#define OLI_OBJECT_FS_H

#include "ICORE/string.h"

IBMI_NAMESPACE_BEGIN ///////////////////////////////////////////////////////////////////////////////////////////////////
FS_NAMESPACE_BEGIN





//***** ПУТЬ К ОБЪЕКТУ ФАЙЛОВОЙ СИСТЕМЫ IBM I **************************************************************************
/*
   ИНВАРИАНТЫ
   1. Для выполнения требований функций API имена всех компонентов пути должны быть строкой ibmi;
   2. Для использования пути в API функциях имя объекта в памяти должно располагаться непосредственно
      перед именем библиотеки;
   3. Функции API требуют, чтобы специальное имя библиотеки (*LIBL, *CURLIB и т.д.), а так же специальное имя
      мембера (*FIRST) были указаны в верхнем регистре;
   4. Имя библиотеки не может быть пустым (если не задано пользователем, то должно иметь значение *LIBL);
   5. Путь может быть пуст (все компоненты пути пусты).
*/
struct path
{
  /* Тип имени компонента пути
  */ typedef ibmi::string<10> name_type;



  /* Стандартные значения IBM i для определения имени библиотеки и имени мемебра
  */ static name_type::const_pointer const libl;
     static name_type::const_pointer const curlib;
     static name_type::const_pointer const first_member;



  path() = default;
  path(const char* p)        { parse_path(p); }
  path(const std::string& p) { parse_path(p); }
  path(const std::string& lib, const std::string& obj, const std::string& mbr = std::string());



  /* Доступ к квалифицированному имени объекта (требуется в вызовах функций API IBM i)
  */
  name_type::const_pointer qname() const {
    return m_object.data(); // TODO Этот метод будет не нужен
  }

  name_type::pointer qname() { return m_object.data(); } // TODO Этот метод будет не нужен



  /* Очистка
  */ void clear();

  /* Извлечение компонентов пути
     TODO возвращать тип name_type
  */ std::string library() const { return m_library.stdstr(); }
     std::string object()  const { return m_object.stdstr(); }
     std::string member()  const { return m_member.stdstr(); }

  /* Путь к объекту в "родном" для файловой системы IBM i формате.
     Например, путь к объекту будет выглядеть как "LIB/OBJECT", а путь к мемберу файла - "LIB/FILE(MEMBER)".
  */ std::string native() const;



  private:
  /* Парсинг пути к объекту
     Формат пути должен соответствовать шаблону [LIBNAME/]OBJNAME[(MBRNAME)], где LIBNAME - имя библиотеки,
     OBJNAME - имя объекта, MBRNAME - имя мембера (если объектом является физический файл), либо быть пустым.
     Имена указанные в [] являются не обязательными. Имя библиотеки может иметь значение *LIBL, *CURLIB.
     Имя мембера может иметь значение *FIRST. Все имена могут быть набраны в любом регистре.
     В случае несоответствия пути шаблону бросается исключение std::invalid_argument.
  */ void parse_path(std::string);

  /*
  */ std::string combine(std::string lib, const std::string& obj, const std::string& mbr) const;

  /* Компоненты пути к объекту по порядку: имя объекта, имя библиотеки, имя мембера файла
     TODO Тип можно сделать и std::string, но тогда у нас появятся аллокации, а сейчас все на стеке
  */ name_type m_object;
     name_type m_library;
     name_type m_member;
};



/* Данная проверка нужна чтобы убедится в отсутствии паддингов в классе path.
   При наличии паддинга между m_object и m_library нарушится инвариант №2.
   TODO Не будет нужна после выполнения всех изменений, указанных выше
*/ static_assert(sizeof(path) == sizeof(path::name_type) * 3, "Violation of invariant #2");
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





FS_NAMESPACE_END
IBMI_NAMESPACE_END /////////////////////////////////////////////////////////////////////////////////////////////////////

#endif // OLI_OBJECT_FS_H

