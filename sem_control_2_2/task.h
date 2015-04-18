#pragma once

#include <type_traits>
#include <sstream>

#include "io_streams.h"
#include "dict.h"


namespace serialization
{

// task1 -----------------

template <class T>
typename std::enable_if<std::is_pod<T>::value, void>::type
read(input_stream& is, T& data)
{
    is.read(&data, sizeof(T));
}

template <class T>
typename std::enable_if<std::is_pod<T>::value, void>::type
write(output_stream& is, const T& data)
{
    is.write(&data, sizeof(T));
}

template <class T>
typename std::enable_if<!(std::is_pod<T>::value), void>::type
read(input_stream& is, T& data)
{
    reflect([&is](auto& field, const std::string& name){read(is, field);}, data);
}

template <class T>
typename std::enable_if<!(std::is_pod<T>::value), void>::type
write(output_stream& os, const T& data)
{
    reflect([&os](auto& field, const std::string& name){write(os, field);}, const_cast<T&>(data));
}

//task2 -----------------

template <class T>
typename std::enable_if<std::is_arithmetic<T>::value, void>::type
read(const dict& d, T& data)
{
    std::stringstream stream;
    stream << d.value;
    stream >> data;
}

template <class T>
typename std::enable_if<std::is_arithmetic<T>::value, void>::type
write(dict& d, const T& data)
{
    std::stringstream stream;
    stream << data;
    stream >> d.value;
}

template <class T>
typename std::enable_if<!std::is_arithmetic<T>::value, void>::type
read(const dict& d, T& data)
{
    reflect([&d](auto& field, const std::string& name)
    {
        try
        {
            read(d.children.at(name), field);
        }
        catch(const std::out_of_range& e)
        {
            field = typename std::remove_reference<decltype(field)>::type();
        }
    }, data);
}

template <class T>
typename std::enable_if<!std::is_arithmetic<T>::value, void>::type
write(dict& d, const T& data)
{
    reflect([&d](auto& field, const std::string& name){ write(d.children[name], field); }, const_cast<T&>(data));
}

} // serialization

