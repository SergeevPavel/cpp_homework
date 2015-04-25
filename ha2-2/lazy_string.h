#ifndef LAZY_STRING_H
#define LAZY_STRING_H

#include <string>
#include <memory>

#include <cstddef>
#include <cstring>

namespace std_utils
{

// TODO add noexcept

template < class charT,
           class traits = std::char_traits<charT> >
class lazy_basic_string
{
public:
    using traits_type = traits;
    using value_type = typename traits_type::char_type;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using difference_type = ptrdiff_t;
    using size_type = size_t;

    lazy_basic_string(const lazy_basic_string& other) = default;

    lazy_basic_string()
        : buffer(new_copy(""))
    {
    }

    lazy_basic_string(const charT* cstr)
        : buffer(new_copy(cstr))
    {
    }

    lazy_basic_string(size_type count, const charT symbol)
        : buffer(new charT[count + 1], std::default_delete<charT[]>())
    {
        traits::assign(buffer.get(), count, symbol);
        traits::assign(buffer.get()[count], traits::eof());
    }

    ~lazy_basic_string() = default;

    lazy_basic_string& operator=(lazy_basic_string const& other) = default;

    lazy_basic_string& operator=(lazy_basic_string&& other)
    {
        swap(other);
        other.clear();
        return *this;
    }

    lazy_basic_string& operator+=(lazy_basic_string const& other)
    {
        // TODO
    }

    charT const& operator[](size_type index) const
    {
        return buffer.get()[index];
    }

    // add non const operator[]

    // add comparasion operators

    void swap(lazy_basic_string& other)
    {
        buffer.swap(other.buffer);
    }

    void clear()
    {
        buffer = new_copy("");
    }

    size_type size() const
    {
        return traits::length(buffer.get());
    }

    const charT* c_str() const
    {
        return buffer.get();
    }

    bool empty() const
    {
        return size() == 0;
    }

private:
    std::shared_ptr<charT> new_copy(charT const* src) const
    {
        size_type src_len = traits::length(src) + 1;
        std::shared_ptr<charT> dst = std::shared_ptr<charT>(new charT[src_len], std::default_delete<charT[]>());
        traits::copy(dst.get(), src, src_len);
        return dst;
    }

    std::shared_ptr<charT> buffer;
};

template<class charT, class traits>
lazy_basic_string<charT, traits> operator+(lazy_basic_string<charT, traits> const& left,
                                           lazy_basic_string<charT, traits> const& right)
{
    // TODO
}

template<class charT, class traits>
void swap(lazy_basic_string<charT, traits>& left,
          lazy_basic_string<charT, traits>& right)
{
    left.swap(right);
}

using lazy_string = lazy_basic_string<char>;
using lazy_wstring = lazy_basic_string<wchar_t>;

} // std_utils

#endif // LAZY_STRING_H

