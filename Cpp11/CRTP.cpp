//
//  CRTP.cpp
//
//  Modified by 罗铮 on 04/10/16.
//  https://segmentfault.com/a/1190000003901666

#include <iostream>

using namespace std;

template<typename T>
struct add_impl {
    static T add(T l, const T& r) {
        typedef decltype(*r.begin()) val;
        transform(r.begin(), r.end(), l.begin(), l.begin(),
            [](const val& vl, const val& rl) { return vl + rl; });
        return l;
    }
};

template<typename Base, template<typename> class Impl>
struct add_ops {
    Base& self;
    add_ops(Base& s) : self(s) { }

    template<typename T1>
    auto operator+(const T1& other) const ->
            decltype(Impl<Base>::add(self, other)) {
        return Impl<Base>::add(self, other);
    }

    template<typename T1>
    auto operator+=(const T1& other) -> decltype(self) {
        return (self = operator+(other));
    }
};

template<typename T>
struct fmt_impl {
    static void fmt(ostream& os, const T& t) {
        typedef decltype(*t.begin()) val;
        os << "[ ";
        for_each(t.begin(), t.end(), [&](const val& v) {
            os << v << ' ';
        });
        os << "]";
    }
};

template<typename T> struct cmplx;
template<typename T>
struct fmt_impl<cmplx<T>> {
    static void fmt(ostream& os, const cmplx<T>& c) {
        os << c[0] << "+" << c[1] << "i";
    }
};

template<typename Base, template<typename> class Impl>
struct fmt_ops {
    Base& self;
    fmt_ops(Base& s) : self(s) { }
};
template<typename Base, template<typename> class Impl>
inline ostream& operator<<(ostream& os, const fmt_ops<Base, Impl> ops) {
    Impl<Base>::fmt(os, ops.self);
    return os;
}

/*
template<typename T>
struct cmplx : public array<T, 2>,
        public add_ops<cmplx<T>, add_impl>,
        public fmt_ops<cmplx<T>, fmt_impl>
{

    typedef array<T, 2> array_t;
    typedef add_ops<cmplx<T>, add_impl> add_ops_t;
    typedef fmt_ops<cmplx<T>, fmt_impl> fmt_ops_t;

    cmplx() : add_ops_t(*this), fmt_ops_t(*this) { }
    cmplx(const cmplx& c) : array_t(c), add_ops_t(*this), fmt_ops_t(*this) { }
    cmplx(initializer_list<T> l) : add_ops_t(*this), fmt_ops_t(*this)
        { copy_n(l.begin(), 2, this->begin()); }
    cmplx& operator=(const cmplx& c)
        { array_t::operator=(c); return *this; }
};

template<typename T, size_t N>
struct vec : public array<T, N>,
        public add_ops<vec<T, N>, add_impl>,
        public fmt_ops<vec<T, N>, fmt_impl>
{

    typedef array<T, N> array_t;
    typedef add_ops<vec<T, N>, add_impl> add_ops_t;
    typedef fmt_ops<vec<T, N>, fmt_impl> fmt_ops_t;

    vec() : add_ops_t(*this), fmt_ops_t(*this) { }
    vec(const vec& v) : array_t(v), add_ops_t(*this), fmt_ops_t(*this) { }
    vec(initializer_list<T> l) : add_ops_t(*this), fmt_ops_t(*this)
        { copy_n(l.begin(), N, this->begin()); }
    vec& operator=(const vec& v)
        { array_t::operator=(v); return *this; }
};

template<typename T, size_t N, size_t M>
struct mat : public array<vec<T, N>, M>,
        public add_ops<mat<T, N, M>, add_impl>,
        public fmt_ops<mat<T, N, M>, fmt_impl>
{

    typedef array<vec<T, N>, M> array_t;
    typedef add_ops<mat<T, N, M>, add_impl> add_ops_t;
    typedef fmt_ops<mat<T, N, M>, fmt_impl> fmt_ops_t;

    mat() : add_ops_t(*this), fmt_ops_t(*this) { }
    mat(const mat& m) : array_t(m), add_ops_t(*this), fmt_ops_t(*this) { }
    mat(initializer_list<vec<T, N>> l) : add_ops_t(*this), fmt_ops_t(*this)
        { copy_n(l.begin(), M, this->begin()); }
    mat& operator=(const mat& m)
        { array_t::operator=(m); return *this; }
};

typedef cmplx<double> c;
typedef vec<double, 3> vec3;
typedef vec<c, 3> vec3c;
typedef mat<double, 3, 2> mat32;
*/
int main()
{
/*
    vec3 v1{1, 2, 3};
    vec3 v2{4, 5, 6};
    vec3 v3 = v1 + v2;
    cout << v3 << endl; // [ 5 7 9 ]

    vec3c vc1{c{0, 1}, c{1, 2}, c{2, 3}};
    vec3c vc2{c{4, 5}, c{5, 6}, c{6, 7}};
    vec3c vc3 = vc1 + vc2;
    cout << vc3 << endl; // [ 4+6i 6+8i 8+10i ]
    vc3 += vc1;
    cout << vc3 << endl; // [ 4+7i 7+10i 10+13i ]

    mat32 m1{
        vec3{1, 2, 3},
        vec3{4, 5, 6},
    };
    mat32 m2{
        vec3{4, 5, 6},
        vec3{7, 8, 9},
    };
    mat32 m3 = m2 + m1;
    cout << m3 << endl; // [ [ 5 7 9 ] [ 11 13 15 ] ]*/
}
