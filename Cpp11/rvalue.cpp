//
//  rvalue.cpp
//
//  Modified by 罗铮 on 04/15/16.
//  http://www.cnblogs.com/qicosmos/p/4283455.html
//  http://eli.thegreenplace.net/2011/12/15/understanding-lvalues-and-rvalues-in-c-and-c

#include <iostream>
#include <vector>

int g_constructCount=0;
int g_copyConstructCount=0;
int g_destructCount=0;

struct A
{
    A(){
        std::cout << this << " construct: "<< ++g_constructCount << std::endl;
    }

    A(const A& a)
    {
        std::cout << this << " copy construct: "<< ++g_copyConstructCount << std::endl;
    }
    ~A()
    {
        std::cout << this << " destruct: "<< ++g_destructCount << std::endl;
    }
};

A GetA()
{
    return A();
}

class Intvec
{
public:
    explicit Intvec(size_t num = 0)
        : m_size(num), m_data(new int[m_size])
    {
        log("constructor");
    }

    ~Intvec()
    {
        log("destructor");
        if (m_data) {
            delete[] m_data;
            m_data = 0;
        }
    }

    Intvec(const Intvec& other)
        : m_size(other.m_size), m_data(new int[m_size])
    {
        log("copy constructor");
        for (size_t i = 0; i < m_size; ++i)
            m_data[i] = other.m_data[i];
    }

    Intvec& operator=(const Intvec& other)
    {
        log("copy assignment operator");
        Intvec tmp(other);
        std::swap(m_size, tmp.m_size);
        std::swap(m_data, tmp.m_data);
        return *this;
    }

    Intvec& operator=(Intvec&& other)
    {
        log("move assignment operator");
        std::swap(m_size, other.m_size);
        std::swap(m_data, other.m_data);
        return *this;
    }
private:
    void log(const char* msg)
    {
        std::cout << "[" << this << "] " << msg << "\n";
    }

    size_t m_size;
    int* m_data;
};

// http://www.cplusplus.com/reference/utility/forward/
// function with lvalue and rvalue reference overloads:
void overloaded (const int& x) {std::cout << "[lvalue]";}
void overloaded (int&& x) {std::cout << "[rvalue]";}

// function template taking rvalue reference to deduced type:
template <class T> void fn (T&& x) {
  overloaded (x);                   // always an lvalue
  overloaded (std::forward<T>(x));  // rvalue if argument is rvalue
}

int main()
{
  int i;

  std::cout << "calling fn with lvalue: ";
  fn (i);
  std::cout << '\n';

  std::cout << "calling fn with rvalue: ";
  fn (0);
  std::cout << '\n';

  const A &a = GetA();
  std::cout << "address of a in main " << &a << std::endl;

  Intvec v1(20);
  Intvec v2;

  std::cout << "assigning lvalue...\n";
  v2 = v1;
  std::cout << "ended assigning lvalue...\n";

  std::cout << "assigning rvalue...\n";
  v2 = Intvec(33);
  std::cout << "ended assigning rvalue...\n";

  std::string str = "Hello";
  std::vector<std::string> v;

  // http://en.cppreference.com/w/cpp/utility/move
  // uses the push_back(const T&) overload, which means
  // we'll incur the cost of copying str
  v.push_back(str);
  std::cout << "After copy, str is \"" << str << "\"\n";

  // uses the rvalue reference push_back(T&&) overload,
  // which means no strings will be copied; instead, the contents
  // of str will be moved into the vector.  This is less
  // expensive, but also means str might now be empty.
  v.push_back(std::move(str));
  std::cout << "After move, str is \"" << str << "\"\n";

  // undefined behavior if size() == 0: back() has a precondition !empty()
  std::cout << "str.back() = " << str.back() << std::endl;

  std::cout << "The contents of the vector are \"" << v[0]
                                       << "\", \"" << v[1] << "\"\n";
  return 0;
}
