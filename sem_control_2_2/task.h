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
serialize(input_stream& is, T& data)
{
    read(is, data);
}

template <class T>
typename std::enable_if<std::is_pod<T>::value, void>::type
serialize(output_stream& os, T& data)
{
    write(os, data);
}

template <class T>
typename std::enable_if<std::is_pod<T>::value, void>::type
read(input_stream& is, T& data)
{
    is.read(&data, sizeof(T));
}

template <class T>
typename std::enable_if<!(std::is_pod<T>::value), void>::type
read(input_stream& is, T& data)
{
    serialize(is, data);
}

template <class T>
typename std::enable_if<std::is_pod<T>::value, void>::type
write(output_stream& is, const T& data)
{
    is.write(&data, sizeof(T));
}

template <class T>
typename std::enable_if<!(std::is_pod<T>::value), void>::type
write(output_stream& is, const T& data)
{
    serialize(is, const_cast<T&>(data));
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
    serialize(d, data);
}

template <class T>
typename std::enable_if<!std::is_arithmetic<T>::value, void>::type
write(dict& d, const T& data)
{
    serialize(d, const_cast<T&>(data));
}

template <class T>
typename std::enable_if<std::is_arithmetic<T>::value, void>::type
serialize(dict& d, T& data)
{
    write(d, data);
}

template <class T>
typename std::enable_if<std::is_arithmetic<T>::value, void>::type
serialize(const dict& d, T& data)
{
    read(d, data);
}

template <class T>
void serialize(const dict& d, T& data, const std::string& name)
{
    try
    {
        serialize(d.children.at(name), data);
    }
    catch(const std::out_of_range& e)
    {
        data = T();
    }
}

template <class T>
void serialize(dict& d, T& data, const std::string& name)
{
    serialize(d.children[name], data);
}

} // serialization

