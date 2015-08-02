//          Copyright Jorge Aguirre July 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DJP_GEOMETRY_CONVEX_POINT_2D_HPP
#define DJP_GEOMETRY_CONVEX_POINT_2D_HPP

#include <utility>
#include <iostream>
#include <cmath>
#include <algorithm>

namespace djp {
template <typename T> struct point {
  T x, y;
  point() {}
  point(T x_, T y_) : x{x_}, y{y_} {}

  friend point operator-(const point &p, const point &q) {
    return {p.x - q.x, p.y - q.y};
  }
  friend point operator+(const point &p, const point &q) {
    return {p.x + q.x, p.y + q.y};
  }

  // scalar multiplication
  template <class Scalar>
  friend point operator*(const point &p, const Scalar &k) {
    return {p.x * k, p.y * k};
  }

  template <class Scalar>
  friend point operator/(const point &p, const Scalar &k) {
    return {p.x / k, p.y / k};
  }

  // Dot product
  friend T operator*(const point &p, const point &q) {
    return p.x * q.x + p.y * q.y;
  }
  // Wedge product
  friend T operator^(const point &p, const point &q) {
    return p.x * q.y - p.y * q.x;
  }

  // lexicographically commparisons
  friend bool
  operator<(const point &p, const point &q) {
    return std::tie(p.x, p.y) < std::tie(q.x, q.y);
  }
  friend bool operator>(const point &p, const point &q) {
    return std::tie(p.x, p.y) > std::tie(q.x, q.y);
  }
  friend bool operator==(const point &p, const point &q) {
    return std::tie(p.x, p.y) == std::tie(q.x, q.y);
  }
  friend bool operator<=(const point &p, const point &q) { return !(p > q); }
  friend bool operator>=(const point &p, const point &q) { return !(p < q); }

  friend T norm2(const point &p) { return p * p; }
  friend double norm(const point &p) { return std::sqrt(norm2(p)); }

  friend double distance2(const point &p, const point &q) {
    return norm2(p - q);
  }
  friend double distance(const point &p, const point &q) { return norm(p - q); }

  // I/O utilities
  friend std::istream &operator>>(std::istream &is, const point &p) {
    is >> p.x >> p.y;
    return is;
  }

  friend std::ostream &operator<<(std::ostream &os, const point &p) {
    os << '(' << p.x << ' ' << p.y << ')';
    return os;
  }

  // Start sorting in 90 degrees, if you need some other angle of start, it is
  // needed to modify the first three conditions. For example if we need start
  // sort from 0 degrees then the first conditions must be:
  // - if (v.y >= 0 && u.y < 0) return false; // u is lesser than v
  // - if (v.y < 0 && u.y >= 0) return true;  // v is lesser than u
  // - if (v.x == 0 && u.x == 0) // closer to center
  //    return v.x >= 0 || u.x >= 0 ? v.x < u.x : u.x < v.x;
  friend bool cw_less(const point &lhs, const point &rhs, const point &center) {
    const point v = (lhs - center);
    const point u = (rhs - center);
    if (v.x >= 0 && u.x < 0)
      return true;
    if (v.x < 0 && u.x >= 0)
      return false;
    if (v.x == 0 && u.x == 0)
      return v.y >= 0 || u.y >= 0 ? v.y < u.y : u.y < v.y;
    const T det = (v ^ u);
    return det == 0 ? norm2(v) < norm2(u) : det < 0;
  }
  friend bool ccw_less(const point &lhs, const point &rhs,
                       const point &center) {
    const point v = (lhs - center);
    const point u = (rhs - center);
    if (v.x >= 0 && u.x < 0)
      return false;
    if (v.x < 0 && u.x >= 0)
      return true;
    if (v.x == 0 && u.x == 0)
      return v.y >= 0 || u.y >= 0 ? v.y < u.y : u.y < v.y;
    const T det = (v ^ u);
    return det == 0 ? norm2(v) < norm2(u) : det > 0;
  }

  template <class RandomIt>
  friend void counter_clockwise_sort(const point &center, RandomIt first,
                                     RandomIt last) {
    std::sort(first, last, [center](const point &lhs, const point &rhs) {
      return ccw_less(lhs, rhs, center);
    });
  }

  template <class RandomIt>
  friend void clockwise_sort(const point &center, RandomIt first,
                             RandomIt last) {
    std::sort(first, last, [center](const point &lhs, const point &rhs) {
      return cw_less(lhs, rhs, center);
    });
  }

  template <class RandomIt>
  friend void clockwise_sort(const point &center, RandomIt first,
                             RandomIt last) {
    std::sort(first, last, [center](const point &lhs, const point &rhs) {
      return cw_less(lhs, rhs, center);
    });
  }

  template <class ForwardIt>
  friend bool is_ccw_sorted(const point &center, ForwardIt first,
                            ForwardIt last) {
    return std::is_sorted(first, last,
                          [&center](const point &lhs, const point &rhs) {
                            return ccw_less(lhs, rhs, center);
                          });
  }

  template <class ForwardIt>
  friend bool is_clockwise_sorted(const point &center, ForwardIt first,
                                  ForwardIt last) {
    return std::is_sorted(first, last,
                          [&center](const point &lhs, const point &rhs) {
                            return cw_less(lhs, rhs, center);
                          });
  }
};

} // djp

#endif // HEADER GUARD
