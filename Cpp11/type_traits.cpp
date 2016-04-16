//
//  type_traits.cpp
//
//  Modified by 罗铮 on 04/10/16.
//  https://segmentfault.com/a/1190000003901666

#include <iostream>
#include <string>
#include <vector>
#include <list>

using namespace std;

template<typename T> struct integral_trait
    { enum { is_integral = false, }; };
template<> struct integral_trait<char>
    { enum { is_integral = true, }; };
template<> struct integral_trait<unsigned char>
    { enum { is_integral = true, }; };
template<> struct integral_trait<int>
    { enum { is_integral = true, }; };
template<> struct integral_trait<unsigned int>
    { enum { is_integral = true, }; };

template<typename T, typename Trait = integral_trait<T>>
bool is_integral_number(T) {
    return Trait::is_integral;
}

////////////////////////////////////////////////////////////
// My classes
struct big_integer {
    big_integer(const string&) { }
};

template<> struct integral_trait<big_integer>
    { enum { is_integral = true, }; };


////////////////////////////////////////////////////////////
template<typename T>
struct iterator_trait {
    typedef typename T::iterator iterator;
    static iterator begin(T& c) { return c.begin(); }
    static iterator end(T& c) { return c.end(); }
};

template<typename T, size_t N>
struct iterator_trait<T[N]> {
    typedef T* iterator;
    static iterator begin(T arr[N]) { return arr; }
    static iterator end(T arr[N]) { return arr + N; }
};

template<typename T, typename Trait = iterator_trait<T>>
// use reference to keep array from decaying to pointer
void print_each(T& container) {
    for(typename Trait::iterator i = Trait::begin(container);
            i != Trait::end(container); ++i)
        cout << *i << ' ';
    cout << endl;
}

int main()
{
    cout << is_integral_number(123) << endl; // 1
    cout << is_integral_number(123.) << endl; // 0
    cout << is_integral_number(big_integer("123")) << endl; // 1
    cout << is_integral_number("123") << endl; // 0

    /////////////////////////////////////////////////////////
    int arr_i[] = { 1, 2 };
    string arr_s[] = { "3", "4" };
    vector<float> vec_f = { 5.f, 6.f };
    list<double> lst_d = { 7., 8. };

    print_each(arr_i); // 1 2
    print_each(arr_s); // 3 4
    print_each(vec_f); // 5 6
    print_each(lst_d); // 7 8
}
