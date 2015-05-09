#include <iostream>
#include <cassert>
#include <cstring>
#include "lazy_string.h"

using namespace std_utils;


void test_internal_typedefs()
{
    assert(!lazy_string::traits_type::compare(
            "abc", "abc", sizeof("abc")));
    lazy_string::value_type chr = 'a';
    lazy_string::reference chr_ref = chr;
    lazy_string::const_reference cchr_ref = chr;
    lazy_string::pointer chr_ptr = &chr;
    lazy_string::const_pointer cchr_ptr = &cchr_ref;
    lazy_string::difference_type diff = chr_ptr - chr_ptr;
    lazy_string::size_type size = chr_ptr - chr_ptr;
    (void)chr;
    (void)chr_ref;
    (void)cchr_ref;
    (void)chr_ptr;
    (void)cchr_ptr;
    (void)diff;
    (void)size;
}

template<class STRING>
bool str_equal(const STRING& str1, const STRING& str2)
{
    if (str1.size() != str2.size())
        return false;

    for (size_t i = 0; i < str1.size(); ++i)
    {
        if (STRING::traits_type::compare(&str1[i], &str2[i], 1))
            return false;
    }
    return true;
}

void test_empty_string()
{
    lazy_string str_non_empty("abc");
    assert(!str_non_empty.empty());
    lazy_string str_empty;
    assert(str_empty.empty());
    assert(str_empty.size() == 0);
    str_empty.clear();
    assert(str_empty.empty());
}

void test_constructors()
{
    const char *c_str = "abcdefghijklmnop";
    lazy_string str1 = c_str;
    lazy_string str2 = str1;
    lazy_string str3(3, 'a');
    assert(str_equal<lazy_string>(c_str, str1));
    assert(str_equal<lazy_string>(str1, str2));
    assert(str_equal<lazy_string>(str3, "aaa"));
    lazy_string moved(std::move(str1));
    assert(str_equal<lazy_string>(c_str, moved));
}

void test_assignment_operator()
{
    lazy_string str1("abcdefg");
    lazy_string str2("77712312ASD ASD sdasd");
    str1 = std::move(str2);
//    assert(str2.empty()); // one of possible states after move.
}

void test_plus_operator()
{
    lazy_string str("abc");
    str += 'c';
    assert(str_equal<lazy_string>(str, "abcc"));

    str = "abc";
    assert(str_equal<lazy_string>('c' + str, "cabc"));
    assert(str_equal<lazy_string>(str + str, "abcabc"));
    assert(str_equal<lazy_string>("123" + str, "123abc"));
    assert(str_equal<lazy_string>(str + "123", "abc123"));
}

void test_index_operator()
{
    const lazy_string cstr("1234567890");
    lazy_string str("1234567890");
    assert(cstr[0] == '1');
    auto str_0 = str[0];
    assert(str[0] == '1');
    assert(str_0 == '1');

    str[0] = 'x';
    assert(str_equal<lazy_string>(str, "x234567890"));
    str[1] = 'y';
    assert(str_equal<lazy_string>(str, "xy34567890"));
    char c = str[0];
    assert(c == 'x');
}

void test_relational_operators()
{
    assert(lazy_string("fgh") < lazy_string("fgh1"));
    assert(lazy_string("z") > lazy_string("fgh1"));
    assert(lazy_string("fgH") == lazy_string("fgH"));
    assert(lazy_string("fgH") <= lazy_string("fgH"));
    assert(lazy_string("fgH") >= lazy_string("fgH"));
}

void test_c_str()
{
    const char* cstr = "I am cstr!";
    lazy_string str(cstr);
    assert(!strcmp(cstr, str.c_str()));
    cstr = str.c_str();
}

void test_swap()
{
    lazy_string str1;
    lazy_string str2;
    str1.swap(str2);
    str1 = "123";
    str1.swap(str2);
    assert(str2 == "123");
}

void test_lazy_wstring()
{
    lazy_wstring str1(L"Hell\xF6\x0A");
    lazy_wstring str2(L"Hell\xF6\x0A");
    assert(str1 == str2);
}

void test_lazy_istring()
{
    lazy_istring s( "AbCdE" );

    assert( s == "abcde" );
    assert( s == "ABCDE" );

    assert( strcmp( s.c_str(), "AbCdE" ) == 0 );
    assert( strcmp( s.c_str(), "abcde" ) != 0 );
}

int main() {
    test_internal_typedefs();
    test_empty_string();
    test_constructors();
    test_assignment_operator();
    test_plus_operator();
    test_index_operator();
    test_relational_operators();
    test_c_str();
    test_swap();
    test_lazy_wstring();
    test_lazy_istring();

    std::cout << "ok!" << std::endl;
    return 0;
}
