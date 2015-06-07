#include <iostream>
#include <cassert>
#include <type_traits>
#include <functional>
#include <utility>


#include <fn.h>


using namespace fn;

int global_var;
static bool post_increment_global_var(int amount=1)
{
    int old_val = global_var;
    global_var += amount;
    return old_val;
}

static void test_1_arg_func_bind()
{
    global_var = 0;
    assert(bind(post_increment_global_var, 777)() == 0);
    assert(global_var == 777);
    global_var = 0;
    assert(bind(post_increment_global_var, _1)(777) == 0);
    assert(global_var == 777);
}

static int take_first(int first, int second)
{
    return first;
}

static int take_second(int first, int second)
{
    return second;
}

static void test_2_arg_func_bind()
{
    assert(bind(take_first, 111, 222)() == 111);
    assert(bind(take_first, _1, 222)(111) == 111);
    assert(bind(take_first, 111, _1)(222) == 111);
    assert(bind(take_first, _1, _2)(111, 222) == 111);
    assert(bind(take_first, _2, _1)(111, 222) == 222);

    assert(bind(take_second, 111, 222)() == 222);
    assert(bind(take_second, _1, 222)(111) == 222);
    assert(bind(take_second, 111, _1)(222) == 222);
    assert(bind(take_second, _1, _2)(111, 222) == 222);
    assert(bind(take_second, _2, _1)(111, 222) == 111);
}

template<class T>
typename std::decay<T>::type func_arb_arg(T &&arg)
{
    return arg;
}

static int arg_by_val(int i)
{
    return i;
}

static void test_passing_by_value()
{
    int i = 777;
    auto binder = bind(arg_by_val, i);
    assert(i == binder());
}

static int arg_by_ref(int &i)
{
    return i;
}

static void test_passing_by_ref()
{
    int i = 777;
    auto binder = bind(arg_by_ref, i);
    assert(i == binder());
}

static int arg_by_const_ref(const int &i)
{
    return i;
}

static void test_passing_by_const_ref()
{
    int i = 777;
    auto binder = bind(arg_by_const_ref, i);
    assert(i == binder());
}

static void test_passing_by_rvalue_ref()
{
    struct test_struct
    {
        test_struct()
            : was_moved_(false)
        {
            std::cout << "create" << std::endl;
        }

        test_struct(const test_struct &src)
            : was_moved_(false)
        {
            std::cout << "copy" << std::endl;
        }

        test_struct(test_struct &&src)
            : was_moved_(false)
        {
            std::cout << "moved" << std::endl;
            src.was_moved_ = true;
        }

        bool was_moved_;
    };

    test_struct arg;
    auto src = bind(func_arb_arg<test_struct&&>, std::move(arg));
    assert(arg.was_moved_);
    (void)src;
}

static void test_placeholder_validity()
{
    auto _1_copy(_1);
    _1_copy = _1;
    auto _1_ptr = &_1;
    bind(take_second, *_1_ptr, 2)(1);
}

static void test_binder_move_constructor()
{
    struct test_struct
    {
        test_struct()
            : was_moved_(false)
        {}

        test_struct(const test_struct &src)
            : was_moved_(src.was_moved_)
        {}

        test_struct(test_struct &&src)
            : was_moved_(true)
        {
            src.was_moved_ = true;
        }

        bool was_moved_;
    };

    test_struct arg;
    auto src = bind(func_arb_arg<test_struct&>, arg);
    src();
    auto moved = std::move(src);
    assert(moved().was_moved_);
}

static void test_binder_copy_constructor()
{

    auto src = bind(take_first, 1, _1);
    auto copy(src);
    assert(src(2) == copy(2));
}

static void test_assignment()
{
    /* There is no assignment in standard bind. */
    /* So ignore this task. */
    /*auto inst1 = bind(take_first, 1, _1);
    auto inst2 = bind(take_second, 2, _1);
    assert(inst1(2) == 1);
    assert(inst2(1) == 1);
    inst2 = inst1;
    assert(inst1(2) == 1);*/
}

int foo()
{
    return 52;
}

int foo1(int x)
{
    return x;
}

void test()
{
    int y = 0;
    int& ry = y;
    int x = fn::bind(foo1, 37)();
    std::cout << x;
}

int main()
{
    test_1_arg_func_bind();
    test_2_arg_func_bind();
    test_passing_by_value();
    test_passing_by_ref();
    test_passing_by_const_ref();
    test_passing_by_rvalue_ref();
    test_placeholder_validity();
    test_binder_move_constructor();
    test_binder_copy_constructor();
    test_assignment();
    std::cout << "Ok!" << std::endl;
    test();
    return 0;
}
