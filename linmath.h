#ifndef LIN_MATH
#define LIN_MATH

#include <math.h>

namespace lm {

template <typename T, size_t len>
struct array {
    using Type = T;
    constexpr static size_t size() { return len; }
    T data[len];
    constexpr T &operator[](size_t i) { return data[i]; }
    constexpr const T &operator[](size_t i) const { return data[i]; }
};

template <typename T>
struct array<T, 2> {
    using Type = T;
    constexpr static size_t size() { return 2; }
    T x, y;
    constexpr T &operator[](size_t i) {
        switch (i) {
        case 0:
            return x;
        case 1:
            return y;
        }
    }
    constexpr const T &operator[](size_t i) const {
        switch (i) {
        case 0:
            return x;
        case 1:
            return y;
        }
    }
};

template <typename T>
struct array<T, 3> {
    using Type = T;
    constexpr static size_t size() { return 3; }
    T x, y, z;
    constexpr T &operator[](size_t i) {
        switch (i) {
        case 0:
            return x;
        case 1:
            return y;
        case 2:
            return z;
        }
    }
    constexpr const T &operator[](size_t i) const {
        switch (i) {
        case 0:
            return x;
        case 1:
            return y;
        case 2:
            return z;
        }
    }
};

template <typename T, size_t len>
constexpr array<T, len> operator-(const array<T, len> &a) {
    array<T, len> r = a;
    for (size_t i = 0; i < r.size(); i++) r[i] = -r[i];
    return r;
}

template <typename T, size_t len>
constexpr bool operator==(const array<T, len> &a, const array<T, len> &b) {
    for (int i = 0; i < len; i++)
        if (!(a[i] == b[i]))
            return false;
    return true;
}

template <typename T, size_t len>
constexpr array<T, len> &operator+=(array<T, len> &a, const array<T, len> &b) {
    for (int i = 0; i < len; i++)
        a[i] += b[i];
    return a;
}
template <typename T, size_t len>
constexpr array<T, len> &operator+=(array<T, len> &a, const T &s) {
    for (int i = 0; i < len; i++)
        a[i] += s;
    return a;
}
template <typename T, size_t len>
constexpr array<T, len> &operator-=(array<T, len> &a, const array<T, len> &b) {
    for (int i = 0; i < len; i++)
        a[i] -= b[i];
    return a;
}
template <typename T, size_t len>
constexpr array<T, len> &operator-=(array<T, len> &a, const T &s) {
    for (int i = 0; i < len; i++)
        a[i] -= s;
    return a;
}
template <typename T, size_t len>
constexpr array<T, len> &operator*=(array<T, len> &a, const array<T, len> &b) {
    for (int i = 0; i < len; i++)
        a[i] *= b[i];
    return a;
}
template <typename T, size_t len>
constexpr array<T, len> &operator*=(array<T, len> &a, const T &s) {
    for (int i = 0; i < len; i++)
        a[i] *= s;
    return a;
}
template <typename T, size_t len>
constexpr array<T, len> &operator/=(array<T, len> &a, const array<T, len> &b) {
    for (int i = 0; i < len; i++)
        a[i] /= b[i];
    return a;
}
template <typename T, size_t len>
constexpr array<T, len> &operator/=(array<T, len> &a, const T &s) {
    for (int i = 0; i < len; i++)
        a[i] /= s;
    return a;
}


template <typename T, size_t len>
constexpr array<T, len> operator+(const array<T, len> &a, const array<T, len> &b) {
    array<T, len> r = a;
    return r += b;
}
template <typename T, size_t len>
constexpr array<T, len> operator-(const array<T, len> &a, const array<T, len> &b) {
    array<T, len> r = a;
    return r -= b;
}
template <typename T, size_t len>
constexpr array<T, len> operator*(const array<T, len> &a, const array<T, len> &b) {
    array<T, len> r = a;
    return r *= b;
}
template <typename T, size_t len>
constexpr array<T, len> operator/(const array<T, len> &a, const array<T, len> &b) {
    array<T, len> r = a;
    return r /= b;
}


template <typename T, typename S, size_t len>
constexpr array<T, len> operator+(const array<T, len> &a, const S &s) {
    array<T, len> r = a;
    return r += s;
}
template <typename T, typename S, size_t len>
constexpr array<T, len> operator-(const array<T, len> &a, const S &s) {
    array<T, len> r = a;
    return r -= s;
}
template <typename T, typename S, size_t len>
constexpr array<T, len> operator*(const array<T, len> &a, const S &s) {
    array<T, len> r = a;
    return r *= s;
}
template <typename T, typename S, size_t len>
constexpr array<T, len> operator/(const array<T, len> &a, const S &s) {
    array<T, len> r = a;
    return r /= s;
}

template <typename T, typename S, size_t len>
constexpr array<T, len> operator+(const S &s, const array<T, len> &a) {
    array<T, len> r = a;
    return r += s;
}
template <typename T, typename S, size_t len>
constexpr array<T, len> operator-(const S &s, const array<T, len> &a) {
    array<T, len> r = -a;
    return r += s;
}
template <typename T, typename S, size_t len>
constexpr array<T, len> operator*(const S &s, const array<T, len> &a) {
    array<T, len> r = a;
    return r *= s;
}
template <typename T, typename S, size_t len>
constexpr array<T, len> operator/(const S &s, const array<T, len> &a) {
    array<T, len> r = a;
    for (size_t i = 0; i < a.size(); i++) r[i] = s / r[i];
    return r;
}


template <typename T, size_t len>
constexpr T sum(const array<T, len> &a) {
    T r = a[0];
    for (int i = 1; i < len; i++)
        r += a[i];
    return r;
}

template <typename T, size_t len>
constexpr T dot(const array<T, len> &a, const array<T, len> &b) {
    return sum(a * b);
}

template <typename T, size_t m, size_t n>
constexpr array<T, m> operator*(const array<array<T, n>, m> &mat,
                                const array<T, n> &vec) {
    array<T, m> r = {};
    for (size_t i = 0; i < m; i++)
        r[i] = dot(mat[i], vec);
    return r;
}

template <typename T, size_t m, size_t n, size_t l>
constexpr array<array<T, l>, m> operator*(const array<array<T, n>, m> &mat,
                                          const array<array<T, l>, n> &mat_) {
    array<array<T, l>, m> r = {};
    // TODO
    for (size_t i = 0; i < m; i++)
        for (size_t k = 0; k < l; k++)
            for (size_t j = 0; j < n; j++)
                r[i][k] += mat[i][j] * mat_[j][k];
    return r;
}

template <size_t len, typename real>
constexpr real length(const array<real, len> &a) {
    return sqrt(dot(a, a));
}

template <size_t len, typename real>
constexpr array<real, len> normalize(const array<real, len> &a) {
    return a * (real(1) / length(a));
}

template <typename T, size_t m, size_t n>
constexpr array<array<T, n>, m> transpose(const array<array<T, m>, n> &mat) {
    array<array<T, n>, m> r = {};
    for (size_t i = 0; i < m; i++)
        for (size_t j = 0; j < n; j++)
            r[i][j] = mat[j][i];
    return r;
}
template <typename T, size_t m, size_t n>
constexpr array<T, m * n> flatten(const array<array<T, m>, n> &mat) {
    array<T, m *n> r = {};
    for (size_t i = 0; i < n; i++)
        for (size_t j = 0; j < m; j++)
            r[i * n + j] = mat[i][j];
    return r;
}

template <typename T, size_t n>
constexpr array<T, n> diagonal(const array<array<T, n>, n> &mat) {
    array<T, n> r = {};
    for (size_t i = 0; i < n; i++)
        r[i] = mat[i][i];
    return r;
}

template <typename T, size_t n>
constexpr T trace(const array<array<T, n>, n> &mat) {
    return sum(diagonal(mat));
}

template <typename T, typename... Args>
constexpr array<T, sizeof...(Args) + 1> make_array(T h, Args... t) {
    return array<T, sizeof...(Args) + 1>{h, t...};
}

// generated
// python code:
//for t, a in [('int', 'i'), ('float', 'f'), ('double', 'd')]:
//    for d in [2,3,4]:
//        print("using {a}vec{d} = array<{type}, {d}>;".format(a=a,type=t,d=d))
//print()
//for t, a in [('int', 'i'), ('float', 'f'), ('double', 'd')]:
//    for d in [2,3,4]:
//        print("using {a}mat{d} = array<array<{type}, {d}>, {d}>;".format(a=a,type=t,d=d))
//print()
//for t, a in [('int', 'i'), ('float', 'f'), ('double', 'd')]:
//    for d1 in [2,3,4]:
//        for d2 in [2,3,4]:
//            print("using {a}mat{d1}{d2} = array<array<{type}, {d2}>, {d1}>;".format(a=a,type=t,d1=d1,d2=d2))

using ivec2 = array<int, 2>;
using ivec3 = array<int, 3>;
using ivec4 = array<int, 4>;
using fvec2 = array<float, 2>;
using fvec3 = array<float, 3>;
using fvec4 = array<float, 4>;
using dvec2 = array<double, 2>;
using dvec3 = array<double, 3>;
using dvec4 = array<double, 4>;

using imat2 = array<array<int, 2>, 2>;
using imat3 = array<array<int, 3>, 3>;
using imat4 = array<array<int, 4>, 4>;
using fmat2 = array<array<float, 2>, 2>;
using fmat3 = array<array<float, 3>, 3>;
using fmat4 = array<array<float, 4>, 4>;
using dmat2 = array<array<double, 2>, 2>;
using dmat3 = array<array<double, 3>, 3>;
using dmat4 = array<array<double, 4>, 4>;

using imat22 = array<array<int, 2>, 2>;
using imat23 = array<array<int, 3>, 2>;
using imat24 = array<array<int, 4>, 2>;
using imat32 = array<array<int, 2>, 3>;
using imat33 = array<array<int, 3>, 3>;
using imat34 = array<array<int, 4>, 3>;
using imat42 = array<array<int, 2>, 4>;
using imat43 = array<array<int, 3>, 4>;
using imat44 = array<array<int, 4>, 4>;
using fmat22 = array<array<float, 2>, 2>;
using fmat23 = array<array<float, 3>, 2>;
using fmat24 = array<array<float, 4>, 2>;
using fmat32 = array<array<float, 2>, 3>;
using fmat33 = array<array<float, 3>, 3>;
using fmat34 = array<array<float, 4>, 3>;
using fmat42 = array<array<float, 2>, 4>;
using fmat43 = array<array<float, 3>, 4>;
using fmat44 = array<array<float, 4>, 4>;
using dmat22 = array<array<double, 2>, 2>;
using dmat23 = array<array<double, 3>, 2>;
using dmat24 = array<array<double, 4>, 2>;
using dmat32 = array<array<double, 2>, 3>;
using dmat33 = array<array<double, 3>, 3>;
using dmat34 = array<array<double, 4>, 3>;
using dmat42 = array<array<double, 2>, 4>;
using dmat43 = array<array<double, 3>, 4>;
using dmat44 = array<array<double, 4>, 4>;

inline bool test() {
    static_assert(ivec2::size() == 2);
    static_assert(ivec3::size() == 3);
    static_assert(ivec4::size() == 4);

    constexpr imat3 eye = make_array(make_array(1, 0, 0), make_array(0, 1, 0),
                                     make_array(0, 0, 1));
    static_assert(trace(eye) == 3);
    static_assert(sum(flatten(eye)) == 3);
    static_assert(transpose(eye) == eye);

    static_assert(eye * make_array(1, 0, 0) == make_array(1, 0, 0));

    constexpr imat43 m = make_array(make_array(0, 0, 0), make_array(1, 0, 0),
                                    make_array(0, 1, 0), make_array(0, 0, 1));
    static_assert(m * make_array(1, 2, 3) == make_array(0, 1, 2, 3));

    static_assert(m * eye == m);
    static_assert(transpose(m) * m == eye);

    return true;
}
} // namespace lm
#endif
