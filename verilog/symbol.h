#ifndef __SYMBOL_H_DEFINED__
#define __SYMBOL_H_DEFINED__

#include <cstring>
#include <map>
#include "common.h"

template<typename T>
class symbol_table_t {
public:
    // types.
    typedef std::map<const char*, T, string_cmp_t> map_t;
    typedef typename map_t::iterator iterator;

protected:
    // variables
    map_t table;
public:
    symbol_table_t();
    virtual ~symbol_table_t();

    iterator get_symbol(const char* s);
    iterator end();
    void add_symbol(const char* s, T& val);
};

template<typename T>
symbol_table_t<T>::symbol_table_t()
{
}

template<typename T>
symbol_table_t<T>::~symbol_table_t()
{
}

template<typename T>
typename symbol_table_t<T>::iterator 
symbol_table_t<T>::get_symbol(const char* s)
{
    return table.find(s);
}

template<typename T>
typename symbol_table_t<T>::iterator 
symbol_table_t<T>::end()
{
    return table.end();
}

template<typename T>
void symbol_table_t<T>::add_symbol(const char* s, T& value)
{
    assert(table.find(s) == table.end());
    table[s] = value;
}

#endif // __SYMBOL_H_DEFINED__
