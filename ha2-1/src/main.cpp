#include <iostream>
#include <cassert>
#include "linked_ptr.h"

#include <memory>

using namespace smart_ptr;

struct Base
{
    virtual bool is_base() { return true; }
    virtual ~Base() {}
};
struct Derived : Base
{
    virtual bool is_base() { return false; }
    virtual ~Derived() {}
};

static void test_empty_lptr()
{
    linked_ptr<int> lpi_empty;
    assert(!lpi_empty);
    lpi_empty.reset();
    assert(!lpi_empty);
    assert(!lpi_empty.get());
    assert(!lpi_empty.unique());
}

static void test_one_lptr()
{
    linked_ptr<int> lpi_single(new int(777));
    assert(lpi_single);
    lpi_single.reset(new int(666));
    assert(lpi_single);
    assert(lpi_single.get());
    assert(lpi_single.unique());
}

static void test_simple_assignment()
{
    using namespace std;
    linked_ptr<int> ptr1(new int(5));
    linked_ptr<int> ptr2(new int(6));
    assert(*ptr1 == 5);
    assert(*ptr2 == 6);
    ptr1 = ptr2;
    assert(*ptr1 == 6);
    assert(*ptr2 == 6);
}

static void test_many_lptr()
{
    linked_ptr<int> lp;
    lp = linked_ptr<int>(new int(777));
    auto lp1(lp);
    auto lp2(lp1);
    auto lp3(lp2);
    auto lp4(lp);
    auto lp5(lp1);
    auto lp6(lp1);
    auto lp7(lp3);
    auto lp8(lp7);
    auto lp9(lp5);
    lp7.reset();
    auto lp10(lp7);
    auto lp11(lp6);
    auto lp12(lp1);
    auto lp13(lp2);
    lp13.reset(new int(666));
    auto lp14(lp3);
    auto lp15(lp13);
    auto lp16(lp12);
    auto lp17(lp15);
}

static void test_lptr_conversions()
{
    linked_ptr<Base> lptr(new Base());
    assert(lptr->is_base());
    lptr = linked_ptr<Base>(new Derived());
    assert(!lptr->is_base());
}

static void test_lptr_delete()
{
    struct foo;
    linked_ptr<foo> lptr;
    struct foo {}; //compiler error if not defined here
}

static void test_constructors()
{
    linked_ptr<int> first;
    linked_ptr<int> second(first);
}

void test_swap()
{
    linked_ptr<int> lp1(new int);
    linked_ptr<int> lp2(lp1);
    linked_ptr<int> lp3(lp2);
    linked_ptr<int> lp4(lp3);

    lp1.swap(lp3); // swap non neighboring elements
    lp2.swap(lp3); // swap neighboring elements
    lp1.swap(lp1); // selfswap
}

void test_copy_from_derived()
{
    linked_ptr<Derived> derived(new Derived);
    linked_ptr<Base> base(derived);
}

int main()
{
//my tests
    test_swap();
    test_copy_from_derived();

//other tests
    test_constructors();
    test_empty_lptr();
    test_one_lptr();
    test_simple_assignment();
    test_many_lptr();
    test_lptr_conversions();
    test_lptr_delete();
    return 0;
}
