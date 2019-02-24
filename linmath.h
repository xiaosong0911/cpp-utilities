#ifndef LIN_MATH
#define LIN_MATH

#include <math.h>

namespace lm {

template <typename T, size_t len>
struct array {
    constexpr static size_t size() { return len; }
    T data[len];
    constexpr T & operator[](size_t i){return data[i];}
    constexpr const T & operator[](size_t i) const {return data[i];}
};

template <typename T, size_t len>
constexpr
bool operator==(const array<T, len> & a, const array<T, len> & b) {
    for (int i = 0; i < len; i++)
        if (!(a[i] == b[i]))
            return false;
    return true;
}

template <typename T, size_t len>
constexpr
array<T, len> & operator+=(array<T, len> & a, const array<T, len> & b) {
    for (int i = 0; i < len; i++) a[i] += b[i]; return a;
}
template <typename T, size_t len>
constexpr
array<T, len> & operator+=(array<T, len> & a, const T & s) {
    for (int i = 0; i < len; i++) a[i] += s; return a;
}
template <typename T, size_t len>
constexpr
array<T, len> & operator-=(array<T, len> & a, const array<T, len> & b) {
    for (int i = 0; i < len; i++) a[i] -= b[i]; return a;
}
template <typename T, size_t len>
constexpr
array<T, len> & operator-=(array<T, len> & a, const T & s) {
    for (int i = 0; i < len; i++) a[i] -= s; return a;
}
template <typename T, size_t len>
constexpr
array<T, len> & operator*=(array<T, len> & a, const array<T, len> & b) {
    for (int i = 0; i < len; i++) a[i] *= b[i]; return a;
}
template <typename T, size_t len>
constexpr
array<T, len> & operator*=(array<T, len> & a, const T & s) {
    for (int i = 0; i < len; i++) a[i] *= s; return a;
}
template <typename T, size_t len>
constexpr
array<T, len> & operator/=(array<T, len> & a, const array<T, len> & b) {
    for (int i = 0; i < len; i++) a[i] /= b[i]; return a;
}
template <typename T, size_t len>
constexpr
array<T, len> & operator/=(array<T, len> & a, const T & s) {
    for (int i = 0; i < len; i++) a[i] /= s; return a;
}

template <typename T, typename A, size_t len>
constexpr
array<T, len> operator+(const array<T, len> & a, const A & b) {
    array<T, len> r = a; return r += b;
}
template <typename T, typename A, size_t len>
constexpr
array<T, len> operator-(const array<T, len> & a, const A & b) {
    array<T, len> r = a; return r -= b;
}
template <typename T, typename A, size_t len>
constexpr
array<T, len> operator*(const array<T, len> & a, const A & b) {
    array<T, len> r = a; return r *= b;
}
template <typename T, typename A, size_t len>
constexpr
array<T, len> operator/(const array<T, len> & a, const A & b) {
    array<T, len> r = a; return r /= b;
}

template <typename T, size_t len>
constexpr
T sum(const array<T, len> & a) {
    T r = a[0]; for (int i = 1; i < len; i++) r += a[i]; return r;
}

template <typename T, size_t len>
constexpr
T dot(const array<T, len> & a, const array<T, len> & b) {
    return sum(a * b);
}

template <typename T, size_t m, size_t n>
constexpr
array<T, m> operator*(const array<array<T, n>, m> & mat, const array<T, n> & vec) {
    array<T, m> r = {};
    for (size_t i = 0; i < m; i++)
        r[i] = dot(mat[i], vec);
    return r;
}

template <typename T, size_t m, size_t n, size_t l>
constexpr
array<array<T, l>, m> operator*(const array<array<T, n>, m> & mat, const array<array<T, l>, n> & mat_) {
    array<array<T, l>, m> r = {};
    // TODO
    for (size_t i = 0; i < m; i++)
        for (size_t k = 0; k < l; k++)
            for (size_t j = 0; j < n; j++)
                r[i][k] += mat[i][j] * mat_[j][k];
    return r;
}

typedef array<int, 2> ivec2;
typedef array<int, 3> ivec3;
typedef array<int, 4> ivec4;

typedef array<float, 2> fvec2;
typedef array<float, 3> fvec3;
typedef array<float, 4> fvec4;


typedef array<array<int, 2>, 2> imat2;
typedef array<array<int, 3>, 3> imat3;
typedef array<array<int, 4>, 4> imat4;

typedef array<array<int, 2>, 2> imat22;
typedef array<array<int, 2>, 3> imat32;
typedef array<array<int, 2>, 4> imat42;
typedef array<array<int, 3>, 2> imat23;
typedef array<array<int, 3>, 3> imat33;
typedef array<array<int, 3>, 4> imat43;
typedef array<array<int, 4>, 2> imat24;
typedef array<array<int, 4>, 3> imat34;
typedef array<array<int, 4>, 4> imat44;

typedef array<array<float, 2>, 2> fmat22;
typedef array<array<float, 2>, 3> fmat32;
typedef array<array<float, 2>, 4> fmat42;
typedef array<array<float, 3>, 2> fmat23;
typedef array<array<float, 3>, 3> fmat33;
typedef array<array<float, 3>, 4> fmat43;
typedef array<array<float, 4>, 2> fmat24;
typedef array<array<float, 4>, 3> fmat34;
typedef array<array<float, 4>, 4> fmat44;

template <size_t len>
constexpr
float norm(const array<float, len> & a) { return sqrt(dot(a, a)); }

template <size_t len>
constexpr
array<float, len> normalize(const array<float, len> & a) {
    return a * (1.f/norm(a));
}

template <typename T, size_t m, size_t n>
constexpr
array<array<T, n>, m> transpose(const array<array<T, m>, n> & mat) {
    array<array<T, n>, m> r = {};
    for (size_t i = 0; i < m; i++)
        for (size_t j = 0; j < n; j++)
            r[i][j] = mat[j][i];
    return r;
}
template <typename T, size_t m, size_t n>
constexpr
array<T, m * n> flatten(const array<array<T, m>, n> & mat) {
    array<T, m * n> r = {};
    for (size_t i = 0; i < n; i++)
        for (size_t j = 0; j < m; j++)
            r[i * n + j] = mat[i][j];
    return r;
}

template <typename T, size_t n>
constexpr
array<T, n> diagonal(const array<array<T, n>, n> & mat) {
    array<T, n> r = {};
    for (size_t i = 0; i < n; i++)
        r[i] = mat[i][i];
    return r;
}

template <typename T, size_t n>
constexpr
T trace(const array<array<T, n>, n> & mat) {
    return sum(diagonal(mat));
}

template <typename T, typename... Args>
constexpr array<T, sizeof...(Args) + 1> make_array(T h, Args... t) {
    return array<T, sizeof...(Args) + 1>{h, t...};
}

inline bool test() {
    static_assert(ivec2::size() == 2);
    static_assert(ivec3::size() == 3);
    static_assert(ivec4::size() == 4);

    constexpr imat3 eye = make_array(
        make_array(1,0,0),
        make_array(0,1,0),
        make_array(0,0,1)
    );
    static_assert(trace(eye) == 3);
    static_assert(sum(flatten(eye)) == 3);
    static_assert(transpose(eye) == eye);

    static_assert(eye * make_array(1,0,0) == make_array(1,0,0));

    constexpr imat43 m = make_array(
        make_array(0,0,0),
        make_array(1,0,0),
        make_array(0,1,0),
        make_array(0,0,1)
    );
    static_assert(m * make_array(1,2,3) == make_array(0,1,2,3));

    static_assert(m * eye == m);
    static_assert(transpose(m) * m == eye);

    return true;
}
}
#endif
