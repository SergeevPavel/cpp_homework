#ifndef LINKED_PTR_H
#define LINKED_PTR_H

#include <utility>

namespace smart_ptr
{

namespace
{
class node_t
{
public:
    node_t()
        : left_(nullptr)
        , right_(nullptr)
    {
    }

    node_t(node_t const* left)
        : left_(left)
        , right_(left->right_)
    {
        insert_me();
    }

    void insert_me() const    {
        if (left_)
        {
            left_->right_ = this;
        }
        if (right_)
        {
            right_->left_ = this;
        }
    }

    void drop_me() const
    {
        if (right_)
        {
            right_->left_ = left_;
        }
        if (left_)
        {
            left_->right_ = right_;
        }
    }

    bool is_neighboring(const node_t& other) const
    {
        return right_ == &other || left_ == &other;
    }

    bool is_one_in_list() const
    {
        return right_ == nullptr && left_ == nullptr;
    }

    void swap(const node_t& other)
    {
        if (!is_neighboring(other))
        {
            std::swap(left_, other.left_);
            std::swap(right_, other.right_);
            insert_me();
            other.insert_me();
        }
    }

    ~node_t()
    {
        drop_me();
    }

private:
    mutable node_t const* left_;
    mutable node_t const* right_;
};
} // anonymous namespace for node_t

template <class T>
class linked_ptr
{
public:
    linked_ptr()
        : pointee_(nullptr)
    {
    }

    template <class U>
    explicit linked_ptr(U* pointee)
        : pointee_(pointee)
    {
    }

    linked_ptr(const linked_ptr& other)
        : node_(&other.node_)
        , pointee_(other.pointee_)
    {
    }

    template <class U>
    linked_ptr(const linked_ptr<U>& other)
        : node_(&other.node_)
        , pointee_(other.pointee_)
    {
    }

    linked_ptr& operator= (linked_ptr other)
    {
        swap(other);
        return *this;
    }

    template <class U>
    linked_ptr& operator= (const linked_ptr<U>& other)
    {
        linked_ptr temp(other);
        swap(temp);
        return *this;
    }

    void reset()
    {
        linked_ptr temp;
        swap(temp);
    }

    template<class U>
    void reset(U* pointee)
    {
        linked_ptr temp(pointee);
        swap(temp);
    }

    void swap(linked_ptr& other)
    {
        node_.swap(other.node_);
        std::swap(pointee_, other.pointee_);
    }

    T* get() const
    {
        return pointee_;
    }

    bool unique() const
    {
        return node_.is_one_in_list() && (pointee_ != nullptr);
    }

    T* operator-> () const
    {
        return pointee_;
    }

    T& operator* () const
    {
        return *pointee_;
    }

    explicit operator bool () const
    {
        return pointee_ != nullptr;
    }

    ~linked_ptr()
    {
        enum {T_IS_INCOMPLETE_TYPE = sizeof(T)};
        if (node_.is_one_in_list())
        {
            delete pointee_;
        }
    }

private:
    template <class U>
    friend class linked_ptr;

    node_t node_;
    T* pointee_;
};

template <class T, class U>
bool operator == (linked_ptr<T> left, linked_ptr<U> right)
{
    return left.get() == right.get();
}

template <class T, class U>
bool operator != (linked_ptr<T> left, linked_ptr<U> right)
{
    return left.get() != right.get();
}

template <class T, class U>
bool operator < (linked_ptr<T> left, linked_ptr<U> right)
{
    return left.get() < right.get();
}

template <class T, class U>
bool operator <= (linked_ptr<T> left, linked_ptr<U> right)
{
    return left.get() <= right.get();
}

template <class T, class U>
bool operator > (linked_ptr<T> left, linked_ptr<U> right)
{
    return left.get() > right.get();
}

template <class T, class U>
bool operator >= (linked_ptr<T> left, linked_ptr<U> right)
{
    return left.get() >= right.get();
}

} // linked_ptr

#endif // LINKED_PTR_H

