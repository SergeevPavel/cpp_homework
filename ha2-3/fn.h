#ifndef FN_H
#define FN_H

#include <type_traits>
#include <utility>
#include <memory>

namespace fn
{

namespace
{

template<unsigned n>
using index = std::integral_constant<unsigned, n>;

index<1u> _1;
index<2u> _2;

template <class A, class... UnusedArgs>
inline A resolve(A&& a, UnusedArgs&&... args)
{
    return a;
}

template <class A, class... UnusedArgs>
inline A resolve(index<1u> _, A&& a, UnusedArgs&&... args)
{
    (void)_;
    return a;
}

template <class A, class B, class... UnusedArgs>
inline A resolve(index<2u> _, B&& b, A&& a, UnusedArgs&&... args)
{
    (void)_;
    (void)b;
    return a;
}

template <class R>
class binder0
{
public:
    using Function = R(*)();
    binder0(Function f)
        : f_(f)
    {}

    binder0(binder0 const& other) = default;

    binder0(binder0&& other) = default;

    template <class... Args>
    inline R operator()(Args&&... args)
    {
        return f_();
    }

private:
    Function f_;
};

template <class R, class A1, class P1>
class binder1
{
public:
    using Function = R(*)(A1);
    binder1(Function f, P1 p1)
        : f_(f)
        , p1_(std::forward<P1>(p1))
    {}

    binder1(binder1 const& other) = default;

    binder1(binder1&& other) = default;

    template <class... Args>
    inline R operator()(Args&&... args)
    {
        return f_((resolve(p1_, std::forward<Args>(args)...)));
    }

private:
    Function f_;
    P1 p1_;
};

template <class R, class A1, class A2, class P1, class P2>
class binder2
{
public:
    using Function = R(*)(A1, A2);
    binder2(Function f, P1 p1, P2 p2)
        : f_(f)
        , p1_(std::forward<P1>(p1))
        , p2_(std::forward<P2>(p2))
    {}

    binder2(binder2 const& other) = default;

    binder2(binder2&& other) = default;

    template <class... Args>
    inline R operator()(Args&&... args)
    {
        return f_(resolve(p1_, std::forward<Args>(args)...),
                  resolve(p2_, std::forward<Args>(args)...));
    }

private:
    Function f_;
    P1 p1_;
    P2 p2_;
};

} // namespace

template <class R>
binder0<R> bind(R (*f)())
{
    return binder0<R>(f);
}

template <class R, class A1, class P1>
binder1<R, A1, P1> bind(R (*f)(A1), P1 p1)
{
    return binder1<R, A1, P1>(f, p1);
}

template <class R, class A1, class A2, class P1, class P2>
binder2<R, A1, A2, P1, P2> bind(R (*f)(A1, A2), P1 p1, P2 p2)
{
    return binder2<R, A1, A2, P1, P2>(f, p1, p2);
}

} // namespace fn

#endif // FN_H

