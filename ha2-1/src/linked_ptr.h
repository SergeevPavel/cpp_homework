#ifndef LINKED_PTR_H
#define LINKED_PTR_H

#include <utility>

namespace smart_ptr
{

template <class T>
class linked_ptr
{
public:
    linked_ptr()
        : node(nullptr, nullptr)
        , pointee_(nullptr)
    {
    }

//    explicit linked_ptr(T* pointee)
//        : pointee_(pointee)
//        , left_(this)
//        , right_(this)
//    {
//    }

    template <class U>
    explicit linked_ptr(U* pointee)
        : node(nullptr, nullptr)
        , pointee_(pointee)
    {
    }

    linked_ptr(const linked_ptr& other)
        : node(&other.node, other.node.right_)
        , pointee_(other.pointee_)
    {
        // check selfcopy
        insert_me();
    }

    template <class U>
    linked_ptr(const linked_ptr<U>& other)
        : node(&other.node, other.node.right_)
        , pointee_(other.pointee_)
    {
        insert_me();
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

//    void reset(T* pointee)
//    {
//        // TODO
//    }

    template<class U>
    void reset(U* pointee)
    {
        linked_ptr temp(pointee);
        swap(temp);
    }

    void swap(linked_ptr& other)
    {
        if (!is_neighboring(other))
        {
            std::swap(pointee_, other.pointee_);
            std::swap(node, other.node);
            insert_me();
            other.insert_me();
        }
    }

    T* get() const
    {
        return pointee_;
    }

    bool unique() const
    {
        return is_one_in_list() && (pointee_ != nullptr);
    }

    T* operator-> () const
    {
        return pointee_;
    }

    T& operator* () const
    {
        // whats with empty smart_pointer
        return *pointee_;
    }

    explicit operator bool () const
    {
        return pointee_ != nullptr;
    }

    ~linked_ptr()
    {
        enum {T_IS_INCOMPLETE_TYPE = sizeof(T)};
        if (is_one_in_list())
        {
            delete pointee_;
        }
        else
        {
            pointee_ = nullptr;
            drop_me();
        }
    }

private:

    struct node_t
    {
        node_t(node_t const* left, node_t const* right)
            : left_(left)
            , right_(right)
        {
        }

        mutable node_t const* left_;
        mutable node_t const* right_;
    } node;

    void insert_me()
    {
        if (node.left_)
        {
            node.left_->right_ = &node;
        }
        if (node.right_)
        {
            node.right_->left_ = &node;
        }
    }

    void drop_me()
    {
        if (node.right_)
        {
            node.right_->left_ = node.left_;
        }
        if (node.left_)
        {
            node.left_->right_ = node.right_;
        }
    }

    bool is_neighboring(const linked_ptr& other) const
    {
        return node.right_ == &other.node || node.left_ == &other.node;
    }

    bool is_one_in_list() const
    {
        return node.right_ == nullptr && node.left_ == nullptr;
    }

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

