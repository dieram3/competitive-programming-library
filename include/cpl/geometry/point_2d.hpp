//          Copyright Jorge Aguirre 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef CPL_GEOMETRY_POINT_2D_HPP
#define CPL_GEOMETRY_POINT_2D_HPP

#include <cmath>       // sqrt
#include <tuple>       // tie
#include <type_traits> // is_arithmetic

namespace cpl {

/// 2D Euclidian vector representation.
template <typename T>
struct point {
  static_assert(std::is_arithmetic<T>::value, "T must be arithmetic");

  T x, y;

  point() = default;
  constexpr point(T x_, T y_) noexcept : x{x_}, y{y_} {}

  // Addition and subtraction
  friend point operator-(const point& p, const point& q) {
    return {p.x - q.x, p.y - q.y};
  }
  friend point operator+(const point& p, const point& q) {
    return {p.x + q.x, p.y + q.y};
  }

  // Scalar multiplication
  friend point operator*(const point& p, const T& k) {
    return {p.x * k, p.y * k};
  }
  friend point operator*(const T& k, const point& p) {
    return p * k;
  }
  friend point operator/(const point& p, const T& k) {
    return {p.x / k, p.y / k};
  }

  // Dot product
  friend T operator*(const point& p, const point& q) {
    return p.x * q.x + p.y * q.y;
  }
  // Wedge product
  friend T operator^(const point& p, const point& q) {
    return p.x * q.y - p.y * q.x;
  }

  // Lexicographical relational operators.
  friend bool operator<(const point& p, const point& q) {
    return std::tie(p.x, p.y) < std::tie(q.x, q.y);
  }
  friend bool operator>(const point& p, const point& q) {
    return q < p;
  }
  friend bool operator==(const point& p, const point& q) {
    return std::tie(p.x, p.y) == std::tie(q.x, q.y);
  }
  friend bool operator!=(const point& p, const point& q) {
    return !(p == q);
  }
  friend bool operator<=(const point& p, const point& q) {
    return !(p > q);
  }
  friend bool operator>=(const point& p, const point& q) {
    return !(p < q);
  }

  // Norm and distance
  friend T norm2(const point& p) {
    return p * p;
  }
  friend auto norm(const point& p) -> decltype(std::sqrt(norm2(p))) {
    return std::sqrt(norm2(p));
  }
  friend T distance2(const point& p, const point& q) {
    return norm2(q - p);
  }
  friend auto distance(const point& p, const point& q) -> decltype(norm(p)) {
    return norm(q - p);
  }
};

} // end namespace cpl

#endif // Header guard
