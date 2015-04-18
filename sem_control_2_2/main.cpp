#include <type_traits>
#include <cstdlib>

#include "dict.h"
#include "task.h"

//using namespace std;
using std::is_pod;

// Part 1 input/output stream serialization
// Here you need to implement your own function for serializing not_pod_struct.

struct pod_struct
{
    int a;
    double b;
};

void test_stream_pod()
{
    static_assert(is_pod<pod_struct>::value, "pod_struct not pod!?");
    pod_struct ps{ 1, 3.14 };
    serialization::output_stream os;
    serialization::write(os, ps);
    serialization::input_stream is(os.data());
    pod_struct ps_read;
    serialization::read(is, ps_read);
    assert(ps.a == ps_read.a);
    assert(ps.b == ps_read.b);
}

struct not_pod_struct
{
    not_pod_struct()
        : not_pod_struct(0, 0, 0)
    {
    }

    not_pod_struct(int a, double b, char c)
        : a(a),
          b(b),
          c(c)
    {
    }

    int a;
    double b;

    char get_c()
    {
        return c;
    }

    template<class visitor_t>
    friend void reflect(const visitor_t& visitor, not_pod_struct& r);
private:
    char c;
};

template<class visitor_t>
void reflect(const visitor_t& visitor, not_pod_struct& r)
{
    visitor(r.a, "a");
    visitor(r.b, "b");
    visitor(r.c, "c");
}


void test_stream_not_pod()
{
    static_assert(!is_pod<not_pod_struct>::value, "not_pod_struct is pod!?");
    not_pod_struct ps{ 1, 3.14, 'P' };
    serialization::output_stream os;
    serialization::write(os, ps);
    serialization::input_stream is(os.data());
    not_pod_struct ps_read;
    serialization::read(is, ps_read);
    assert(ps.a == ps_read.a);
    assert(abs(ps.b - ps_read.b) < 0.001);
    assert(ps.get_c() == ps_read.get_c());
}

void test_stream_serialization()
{
    test_stream_pod();
    test_stream_not_pod();
}


// Part 2 dictionary serialization
// Here you need to implement your own function for serializing custom_record and small_record struct, because they are not arithmetic types.


struct small_record
{
    char letter;
    bool flag;

    small_record()
        : letter(0)
        , flag(0)
    {
    }
};


template <class visitor_t>
void reflect(const visitor_t& visitor, small_record& sr)
{
    visitor(sr.letter, "letter");
    visitor(sr.flag, "flag");
}


struct custom_record
{
    custom_record()
        : dvalue(0)
        , ivalue(0)
    {
    }

    double dvalue;
    int    ivalue;
    small_record small;
};

template <class visitor_t>
void reflect(const visitor_t& visitor, custom_record& cr)
{
    visitor(cr.dvalue, "dvalue");
    visitor(cr.ivalue, "ivalue");
    visitor(cr.small, "small");
}

void test_dict_arithmetic()
{
    serialization::dict d;

    int i = 42;
    write(d, i);

    int o;
    read(d, o);

    assert(i == o);

}

void test_dict_struct()
{
    serialization::dict d;

    custom_record cr;
    cr.dvalue = 3.14;
    cr.small.letter = 'P';
    write(d, cr);

    custom_record cr2;
    read(d, cr2);

    assert(abs(cr2.dvalue - 3.14) < 0.001);
    assert(cr2.small.letter == 'P');
}

struct ver0
{
    int a;
    short b;
    char c;
};

template <class visitor_t>
void reflect(const visitor_t& visitor, ver0& v0)
{
    visitor(v0.a, "a");
    visitor(v0.b, "b");
    visitor(v0.c, "c");
}

struct ver1
{
    int a;
    short b;
};

template <class visitor_t>
void reflect(const visitor_t& visitor, ver1& v1)
{
    visitor(v1.a, "a");
    visitor(v1.b, "b");
}

// serialize ver1

void test_dict_field_removed()
{
    serialization::dict d;
    ver0 v0{ 1, 2, 'c' };
    write(d, v0);

    ver1 v1;
    read(d, v1);
    assert(v0.a == v1.a);
    assert(v0.b == v1.b);
}

void test_dict_field_added()
{
    serialization::dict d;
    auto v1 = ver1{ 3, 4 };
    write(d, v1);

    auto v0 = ver0{};
    read(d, v0);
    assert(v0.a == v1.a);
    assert(v0.b == v1.b);
    assert(v0.c == 0);
}

void test_dict_versioning()
{
    test_dict_field_added();
    test_dict_field_removed();
}

void test_dict_serialization()
{
    test_dict_arithmetic();
    test_dict_struct();
    test_dict_versioning();
}

//// Part 3 - common reflect function.
//// You need to replace serialize functions for not_pod_struct, custom_record, small_record with one common function called reflect
//// Reflect doesn't known anything about target storage of serialized data, it can be stream or dict.

int main()
{
    test_stream_serialization();
    test_dict_serialization();
    return 0;
}
