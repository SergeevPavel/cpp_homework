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
private:
    class proxy;

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

    lazy_basic_string(lazy_basic_string&& other)
        : buffer_(other.buffer_)
        , length_(other.length_)
    {
        other.clear();
    }

    lazy_basic_string()
        : buffer_(from_c_str(""))
        , length_(0)
    {
    }

    lazy_basic_string(charT const* cstr)
        : buffer_(from_c_str(cstr))
        , length_(traits::length(cstr))
    {
    }

    lazy_basic_string(size_type count, const charT symbol)
        : buffer_(new charT[count + 1], std::default_delete<charT[]>())
        , length_(symbol)
    {
        traits::assign(buffer_.get(), count, symbol);
        traits::assign(buffer_.get()[count], traits::eof());
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
        std::shared_ptr<charT> dst = std::shared_ptr<charT>(new charT[length_ + other.length_ + 1],
                std::default_delete<charT[]>());
        traits::copy(dst.get(), buffer_.get(), length_);
        traits::copy(dst.get() + length_, other.buffer_.get(), other.length_ + 1);
        buffer_.swap(dst);
        length_ += other.length_;
        return *this;
    }

    lazy_basic_string& operator+=(const charT c)
    {
        std::shared_ptr<charT> dst = std::shared_ptr<charT>(new charT[length_ + 1],
                std::default_delete<charT[]>());
        traits::copy(dst.get(), buffer_.get(), length_);
        traits::assign(dst.get()[length_], c);
        traits::assign(dst.get()[length_ + 1], traits::eof());
        buffer_.swap(dst);
        length_++;
        return *this;
    }

    lazy_basic_string::proxy operator[](size_type index)
    {
        return proxy(*this, index);
    }

    charT const& operator[](size_type index) const
    {
        //TODO check index range
        return buffer_.get()[index];
    }

    // add comparasion operators

    void swap(lazy_basic_string& other)
    {
        std::swap(length_, other.length_);
        buffer_.swap(other.buffer_);
    }

    void clear()
    {
        buffer_ = from_c_str("");
        length_ = 0;
    }

    size_type size() const
    {
        return length_;
    }

    const charT* c_str() const
    {
        return buffer_.get();
    }

    bool empty() const
    {
        return size() == 0;
    }

    int compare(const lazy_basic_string& other) const
    {
        int result = traits::compare(buffer_.get(), other.buffer_.get(), std::min(length_, other.length_));
        if (result == 0)
        {
            if (length_ == other.length_)
            {
                return 0;
            }
            else
            {
                if (length_ < other.length_)
                {
                    return -1;
                }
                else
                {
                    return 1;
                }
            }
        }
        return result;
    }

private:
    std::shared_ptr<charT> from_c_str(charT const* src) const
    {
        size_type src_len = traits::length(src) + 1;
        std::shared_ptr<charT> dst = std::shared_ptr<charT>(new charT[src_len], std::default_delete<charT[]>());
        traits::copy(dst.get(), src, src_len);
        return dst;
    }

    void create_own_buffer()
    {
        if (!buffer_.unique())
        {
            std::shared_ptr<charT> new_buffer = from_c_str(buffer_.get());
            std::swap(buffer_, new_buffer);
        }
    }

    class proxy
    {
    public:
        proxy(lazy_basic_string<charT>& owner, size_type index)
            : owner_(owner)
            , index_(index)
        {
        }

        proxy& operator=(const charT c)
        {
            if (!traits::eq(symbol(), c))
            {
                owner_.create_own_buffer();
                symbol() = c;
            }
            return *this;
        }

        operator charT()
        {
            return symbol();
        }

    private:
        charT& symbol()
        {
            return owner_.buffer_.get()[index_];
        }

        lazy_basic_string<charT>& owner_;
        size_type index_;
    };

    std::shared_ptr<charT> buffer_;
    size_type length_;
};

template<class charT, class traits = std::char_traits<charT>>
lazy_basic_string<charT, traits> operator+(lazy_basic_string<charT, traits> const& left,
                                           lazy_basic_string<charT, traits> const& right)
{
    lazy_basic_string<charT, traits> result(left);
    result += right;
    return result;
}

template<class charT, class traits = std::char_traits<charT>>
lazy_basic_string<charT, traits> operator+(charT const* left,
                                           lazy_basic_string<charT, traits> const& right)
{
    return lazy_basic_string<charT>(left) + right;
}

template<class charT, class traits = std::char_traits<charT>>
lazy_basic_string<charT, traits> operator+(lazy_basic_string<charT, traits> const& left,
                                           charT const* right)
{
    return left + lazy_basic_string<charT>(right);
}

template<class charT, class traits = std::char_traits<charT>>
lazy_basic_string<charT, traits> operator+(lazy_basic_string<charT, traits> const& left,
                                           const charT right)
{
    lazy_basic_string<charT, traits> result(left);
    result += right;
    return result;
}

template<class charT, class traits = std::char_traits<charT>>
lazy_basic_string<charT, traits> operator+(const charT left,
                                           lazy_basic_string<charT, traits> const& right)
{
    lazy_basic_string<charT, traits> result;
    result += left;
    result += right;
    return result;
}

template<class charT, class traits = std::char_traits<charT>>
void swap(lazy_basic_string<charT, traits>& left,
          lazy_basic_string<charT, traits>& right)
{
    left.swap(right);
}

// comparation operators

template<class charT, class traits = std::char_traits<charT>>
bool operator ==(lazy_basic_string<charT, traits> const& left,
                 lazy_basic_string<charT, traits> const& right)
{
    return left.compare(right) == 0;
}

template<class charT, class traits = std::char_traits<charT>>
bool operator !=(lazy_basic_string<charT, traits> const& left,
                 lazy_basic_string<charT, traits> const& right)
{
    return left.compare(right) != 0;
}


template<class charT, class traits = std::char_traits<charT>>
bool operator <(lazy_basic_string<charT, traits> const& left,
                lazy_basic_string<charT, traits> const& right)
{
    return left.compare(right) < 0;
}


template<class charT, class traits = std::char_traits<charT>>
bool operator <=(lazy_basic_string<charT, traits> const& left,
                 lazy_basic_string<charT, traits> const& right)
{
    return left.compare(right) <= 0;
}

template<class charT, class traits = std::char_traits<charT>>
bool operator >(lazy_basic_string<charT, traits> const& left,
                lazy_basic_string<charT, traits> const& right)
{
    return left.compare(right) > 0;
}


template<class charT, class traits = std::char_traits<charT>>
bool operator >=(lazy_basic_string<charT, traits> const& left,
                 lazy_basic_string<charT, traits> const& right)
{
    return left.compare(right) >= 0;
}

// maybe try use boost operators

template<class charT, class traits = std::char_traits<charT>>
bool operator ==(charT const* left,
                 lazy_basic_string<charT, traits> const& right)
{
    return left.compare(right) == 0;
}

template<class charT, class traits = std::char_traits<charT>>
bool operator !=(charT const* left,
                 lazy_basic_string<charT, traits> const& right)
{
    return left.compare(right) != 0;
}


template<class charT, class traits = std::char_traits<charT>>
bool operator <(charT const* left,
                lazy_basic_string<charT, traits> const& right)
{
    return left.compare(right) < 0;
}


template<class charT, class traits = std::char_traits<charT>>
bool operator <=(charT const* left,
                 lazy_basic_string<charT, traits> const& right)
{
    return left.compare(right) <= 0;
}

template<class charT, class traits = std::char_traits<charT>>
bool operator >(charT const* left,
                lazy_basic_string<charT, traits> const& right)
{
    return left.compare(right) > 0;
}


template<class charT, class traits = std::char_traits<charT>>
bool operator >=(charT const* left,
                 lazy_basic_string<charT, traits> const& right)
{
    return left.compare(right) >= 0;
}

//

template<class charT, class traits = std::char_traits<charT>>
bool operator ==(lazy_basic_string<charT, traits> const& left,
                 charT const* right)
{
    return left.compare(right) == 0;
}

template<class charT, class traits = std::char_traits<charT>>
bool operator !=(lazy_basic_string<charT, traits> const& left,
                 charT const* right)
{
    return left.compare(right) != 0;
}


template<class charT, class traits = std::char_traits<charT>>
bool operator <(lazy_basic_string<charT, traits> const& left,
                charT const* right)
{
    return left.compare(right) < 0;
}


template<class charT, class traits = std::char_traits<charT>>
bool operator <=(lazy_basic_string<charT, traits> const& left,
                 charT const* right)
{
    return left.compare(right) <= 0;
}

template<class charT, class traits = std::char_traits<charT>>
bool operator >(lazy_basic_string<charT, traits> const& left,
                charT const* right)
{
    return left.compare(right) > 0;
}


template<class charT, class traits = std::char_traits<charT>>
bool operator >=(lazy_basic_string<charT, traits> const& left,
                 charT const* right)
{
    return left.compare(right) >= 0;
}

// end of comparation operators

using lazy_string = lazy_basic_string<char>;
using lazy_wstring = lazy_basic_string<wchar_t>;

} // std_utils

#endif // LAZY_STRING_H

