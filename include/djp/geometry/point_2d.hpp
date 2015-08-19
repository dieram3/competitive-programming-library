//          Copyright Jorge Aguirre July 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DJP_GEOMETRY_POINT_2D_HPP
#define DJP_GEOMETRY_POINT_2D_HPP

#include <utility>
#include <iostream>
#include <cmath>
#include <tuple>
#include <algorithm>

namespace djp {
/// 2D Euclidian vector representation.
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
  friend point operator*(const point &p, const T &k) {
    return {p.x * k, p.y * k};
  }

  friend point operator*(const T &k, const point &p) { return p * k; }

  friend point operator/(const point &p, const T &k) {
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

  friend bool operator>(const point &p, const point &q) { return q < p; }

  friend bool operator==(const point &p, const point &q) {
    return std::tie(p.x, p.y) == std::tie(q.x, q.y);
  }

  friend bool operator!=(const point& p, const point& q) { return !(p == q); }

  friend bool operator<=(const point &p, const point &q) { return !(p > q); }

  friend bool operator>=(const point &p, const point &q) { return !(p < q); }

  friend T norm2(const point &p) { return p * p; }

  friend auto norm(const point &p) -> decltype(std::sqrt(norm2(p))) {
    return std::sqrt(norm2(p));
  }

  friend T distance2(const point &p, const point &q) { return norm2(q - p); }

  friend auto distance(const point &p, const point &q) -> decltype(norm(p)) {
    return norm(q - p);
  }

  // I/O utilities
  friend std::istream &operator>>(std::istream &is, const point &p) {
    is >> p.x >> p.y;
    return is;
  }

  friend std::ostream &operator<<(std::ostream &os, const point &p) {
    os << '(' << p.x << ", " << p.y << ')';
    return os;
  }
};

} // djp

#endif // HEADER GUARD
