//
//  variadic_templates.cpp
//
//  Modified by 罗铮 on 04/11/16.
//  http://eli.thegreenplace.net/2014/variadic-templates-in-c/
//  https://bartoszmilewski.com/2009/10/21/what-does-haskell-have-to-do-with-c/

#include <iostream>
#include <typeinfo>
#include <vector>
#include <list>
#include <map>
#include <memory>

///////////////////////////////////////////////////////////////////////////////
//Basic example
///////////////////////////////////////////////////////////////////////////////
template<typename T>
T adder(T v) {
  return v;
}

template<typename T, typename... Args>
T adder(T first, Args... args) {
  return first + adder(args...);
}

template<typename T>
void printarg(T t)
{
  std::cout << t << " ";
}

template<typename... Args>
void expand_with_comma(Args... args)
{
  int arr[] = {(printarg(args), 0)...};

  for (const auto& v : arr) {
    std::cout << v << ' ';
  }
  std::cout << "\n";
}

template<typename F, typename... Args>
void expand_with_forward(const F &f, Args... args)
{
  std::initializer_list<int> l{(f(std::forward<Args>(args)),0)...};

  for (const auto& v : l) {
    std::cout << v << ' ';
  }
  std::cout << "\n";
}

///////////////////////////////////////////////////////////////////////////////
//Some simple variations
///////////////////////////////////////////////////////////////////////////////
// In real-world code, we wouldn't compare floating point values like
// this. It would make sense to specialize this function for floating
// point types to use approximate comparison.
template<typename T, typename U>
bool pair_comparer(T a, U b) {
  return a == b;
}

template<typename T>
bool pair_comparer(T a) {
  return false;
}

template<typename T, typename U, typename... Args>
bool pair_comparer(T a, U b, Args... args) {
  return a == b && pair_comparer(args...);
}

///////////////////////////////////////////////////////////////////////////////
//Varidic data structures
///////////////////////////////////////////////////////////////////////////////
template <class... Ts> struct tuple {};

template <class T, class... Ts>
struct tuple<T, Ts...> : tuple<Ts...> {
  tuple(T t, Ts... ts) : tuple<Ts...>(ts...), tail(t) {}

  T tail;
};

template <size_t k, class... Ts>
struct elem_type_holder {};

template <class T, class... Ts>
struct elem_type_holder<0, tuple<T, Ts...>> {
  typedef T type;
};

template <size_t k, class T, class... Ts>
struct elem_type_holder<k, tuple<T, Ts...>> {
  typedef typename elem_type_holder<k - 1, tuple<Ts...>>::type type;
};

template <size_t k, class... Ts>
typename std::enable_if<
    k == 0, typename elem_type_holder<0, tuple<Ts...>>::type&>::type
get(tuple<Ts...>& t) {
  return t.tail;
}

template <size_t k, class T, class... Ts>
typename std::enable_if<
    k != 0, typename elem_type_holder<k, tuple<T, Ts...>>::type&>::type
get(tuple<T, Ts...>& t) {
  tuple<Ts...>& base = t;
  return get<k - 1>(base);
}

///////////////////////////////////////////////////////////////////////////////
//Variadic templates for catch-all functions
///////////////////////////////////////////////////////////////////////////////

// Implement << for pairs: this is needed to print out mappings where range
// iteration goes over (key, value) pairs.
template <typename T, typename U>
std::ostream& operator<<(std::ostream& out, const std::pair<T, U>& p) {
  out << "[" << p.first << ", " << p.second << "]";
  return out;
}

template <template <typename, typename...> class ContainerType,
          typename ValueType, typename... Args>
void print_container(const ContainerType<ValueType, Args...>& c) {
  for (const auto& v : c) {
    std::cout << v << ' ';
  }
  std::cout << '\n';
}

///////////////////////////////////////////////////////////////////////////////
//Variadic templates for forwarding
//http://en.cppreference.com/w/cpp/memory/unique_ptr/make_unique
///////////////////////////////////////////////////////////////////////////////
/*template<typename T, typename... Args>
std::unique_ptr<T> make_unique(Args... args)
{
    return std::unique_ptr<T>(new T(args...));
}
*/
template<typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args)
{
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

struct Vec3
{
    int x, y, z;
    Vec3() : x(0), y(0), z(0) { }
    Vec3(int x, int y, int z) :x(x), y(y), z(z) { }
    friend std::ostream& operator<<(std::ostream& os, Vec3& v) {
        return os << '{' << "x:" << v.x << " y:" << v.y << " z:" << v.z  << '}';
    }
};

///////////////////////////////////////////////////////////////////////////////
//Factorial
///////////////////////////////////////////////////////////////////////////////
template<int n> struct
fact {
    static const int value = n * fact<n - 1>::value;
};

template<> struct
fact<0> { // specialization for n = 0
    static const int value = 1;
};

///////////////////////////////////////////////////////////////////////////////
//Lists
///////////////////////////////////////////////////////////////////////////////
// Just a declaration
template<class... list> struct
count;

template<> struct
count<> {
    static const int value = 0;
};

template<class head, class... tail> struct
count<head, tail...> {
    static const int value = 1 + count<tail...>::value;
};

///////////////////////////////////////////////////////////////////////////////
//Higher-Order Functions and Closures
///////////////////////////////////////////////////////////////////////////////
template<class T> struct
isPtr {
    static const bool value = false;
};

template<class U> struct
isPtr<U*> {
    static const bool value = true;
};

template<class U> struct
isPtr<U * const> {
    static const bool value = true;
};

template<class T> struct
isConst {
    static const bool value = false;
};

template<class T> struct
isConst<const T> {
    static const bool value = true;
};

template<template<class> class f1, template<class> class f2> struct
or_combinator {
    template<class T> struct
    lambda {
        static const bool value = f1<T>::value || f2<T>::value;
    };
};

///////////////////////////////////////////////////////////////////////////////
//Higher-Order Functions Operating on Lists
///////////////////////////////////////////////////////////////////////////////
template<template<class> class predicate, class... list> struct
all;

template<template<class> class predicate> struct
all<predicate> {
    static const bool value = true;
};

template<
    template<class> class predicate,
    class head,
    class... tail> struct
    all<predicate, head, tail...> {
      static const bool value =
        predicate<head>::value
        && all<predicate, tail...>::value;
};

template<class T, int u> struct add
{
  static const int value = 1 + u;
};

template<template<class, int> class, int, class...> struct
fold_right;

template<template<class, int> class f, int init> struct
fold_right<f, init> {
    static const int value = init;
};

template<template<class, int> class f, int init, class head, class...tail> struct
fold_right<f, init, head, tail...> {
    static const int value = f<head, fold_right<f, init, tail...>::value>::value;
};

///////////////////////////////////////////////////////////////////////////////
//List of Numbers
///////////////////////////////////////////////////////////////////////////////
template<int...> struct
sum;

template<> struct
sum<> {
    static const int value = 0;
};

template<int i, int... tail> struct
sum<i, tail...> {
    static const int value = i + sum<tail...>::value;
};

///////////////////////////////////////////////////////////////////////////////
//List Comprehension
///////////////////////////////////////////////////////////////////////////////
template<class T> struct
one {
    static const int value = 1;
};

template<class... lst> struct
counter {
    static const int value = sum<one<lst>::value...>::value;
};

template<class... lst> struct
countPtrs {
    static const int value = sum<isPtr<lst>::value ...>::value;
};

///////////////////////////////////////////////////////////////////////////////
//Continuations
///////////////////////////////////////////////////////////////////////////////
template<typename T>
int map_cont_run(int (*cont)(std::list<T> list), int (*f)(T), const std::list<T> &lst) {
    std::list<T> tmp;
    for (auto it = lst.begin(); it != lst.end(); ++it)
        tmp.push_front(f(*it));
    return cont(tmp);
}

template<typename T> struct
fun {
  typedef T type;
};

template<template<class...> class counter,
         template<class> class f,
         class... lst> struct
map_cont {
    static const int value =
        counter<typename f<lst>::type ...>::value;
};

///////////////////////////////////////////////////////////////////////////////
//mailist<T> n
///////////////////////////////////////////////////////////////////////////////
int main()
{
  long lsum = adder(1, 2, 3, 8, 7);
  std::cout << lsum << "\n";

  std::cout << adder(1.1, 2.2) << "\n";

  std::string s1 = "x", s2 = "aa", s3 = "bb", s4 = "yy";
  std::string ssum = adder(s1, s2, s3, s4);
  std::cout << ssum << "\n";

  expand_with_comma(4,3,2,1);
  expand_with_forward([](int i) {std::cout << i << " ";}, 8,7,6,5);

  /////////////////////////////////////////////////////////////////////////////

  std::cout << std::boolalpha << pair_comparer(1.5, 1.5, 2, 2, 6, 6)    << "\n";
  std::cout << std::boolalpha << pair_comparer(1.5, 1, 2, 2, 6, 6)      << "\n";
  std::cout << std::boolalpha << pair_comparer(1.5, 1.5, 2, 2, 6, 6, 7) << "\n";

  /////////////////////////////////////////////////////////////////////////////

  tuple<double, uint64_t, const char*> t1(12.2, 42, "big");

  std::cout << "0th elem is " << get<0>(t1) << "\n";
  std::cout << "1th elem is " << get<1>(t1) << "\n";
  std::cout << "2th elem is " << get<2>(t1) << "\n";

  get<1>(t1) = 103;

  std::cout << "1th elem is " << get<1>(t1) << "\n";

  tuple<int, int> t2(25, 42);

  std::cout << "0th elem is " << get<0>(t2) << "\n";
  std::cout << "1th elem is " << get<1>(t2) << "\n";

  tuple<char, char, char> t3('a', 'b', 'c');

  std::cout << "0th elem is " << get<0>(t3) << "\n";
  std::cout << "1th elem is " << get<1>(t3) << "\n";
  std::cout << "2th elem is " << get<2>(t3) << "\n";

  typename elem_type_holder<1, tuple<double, int, const char*>>::type foo;
  std::cout << typeid(foo).name() << "\n";

  /////////////////////////////////////////////////////////////////////////////
  std::vector<double> vd{3.14, 8.1, 3.2, 1.0};
  print_container(vd);

  std::list<int> li{1, 2, 3, 5};
  print_container(li);

  std::map<std::string, int> msi{{"foo", 42}, {"bar", 81}, {"bazzo", 4}};
  print_container(msi);

  /////////////////////////////////////////////////////////////////////////////
  // http://en.cppreference.com/w/cpp/memory/unique_ptr/make_unique
  // Use the default constructor.
  std::unique_ptr<Vec3> v1 = make_unique<Vec3>();
  // Use the constructor that matches these arguments
  std::unique_ptr<Vec3> v2 = make_unique<Vec3>(0, 1, 2);
  // Create a unique_ptr to an array of 5 elements
  //std::unique_ptr<Vec3[]> v3 = make_unique<Vec3[]>(5);

  std::cout << "make_unique<Vec3>():      " << *v1 << '\n'
            << "make_unique<Vec3>(0,1,2): " << *v2 << '\n';
  //          << "make_unique<Vec3[]>(5):   " << '\n';
  //for (int i = 0; i < 5; i++) {
  //    std::cout << "     " << v3[i] << '\n';
  //}

  std::cout << "Factorial of 5 = " << fact<5>::value << std::endl;
  std::cout << "count<int, char, long>::value = " << count<int, char, long>::value << std::endl;

  std::cout << "or_combinator<isPtr, isConst>::lambda<const int>::value = "
            << or_combinator<isPtr, isConst>::lambda<const int>::value
            << std::endl;

  std::cout << "or_combinator<isPtr, isConst>::lambda<int*>::value = "
            << or_combinator<isPtr, isConst>::lambda<int*>::value
            << std::endl;

  std::cout << "or_combinator<isPtr, isConst>::lambda<int>::value = "
            << or_combinator<isPtr, isConst>::lambda<int>::value
            << std::endl;

  std::cout << "all<isPtr>::value = " << all<isPtr, int*, char*, long>::value << std::endl;
  std::cout << "all<isPtr>::value = " << all<isPtr, int*, char*, long*>::value << std::endl;

  std::cout << "Fold right = " << fold_right<add, 3, int, double, char>::value << std::endl;

  std::cout << "counter<int, char, long>::value = " << counter<int, char, long>::value << std::endl;
  std::cout << "map_cont<counter, fun, int, char, long, double>::value = "
            << map_cont<counter, fun, int, char, long, double>::value << std::endl;

  return 0;
}
