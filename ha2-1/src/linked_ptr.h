#ifndef LINKED_PTR_H
#define LINKED_PTR_H

namespace smart_ptr
{

template <class T>
class smart_ptr
{
public:
    smart_ptr()
        : pointee_(nullptr)
        , left(this)
        , right(this)
    {
    }

    explicit smart_ptr(T* pointee)
        : pointee_(pointee)
        , left(this)
        , right(this)
    {

    }

    smart_ptr(const smart_ptr<T>& other)
    {
        // TODO
    }

    template <class U>
    explicit smart_ptr(U* pointee)
        : pointee_(pointee)
        , left(this)
        , right(this)
    {
    }

    template <class U>
    smart_ptr(const smart_ptr<U>& other)
    {
        // TODO
    }

    smart_ptr& operator= (const smart_ptr<T>& other)
    {
        // TODO
    }

    template <class U>
    smart_ptr& operator= (const smart_ptr<U>& other)
    {
        // TODO
    }

    void reset()
    {
        // TODO
    }

    void reset(T* pointee)
    {
        // TODO
    }

    template<class U>
    void reset(U* pointee)
    {
        // TODO
    }

    void swap(smart_ptr& other)
    {
        // TODO
    }

    T* get() const
    {
        // TODO
    }

    bool unique() const
    {
        // TODO
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

    // add comapasion operators

    explicit operator bool () const
    {
        return pointee_ != nullptr;
    }
private:
    T* pointee_;
    smart_ptr* left;
    smart_ptr* right;
};

} // smart_ptr

#endif // LINKED_PTR_H

