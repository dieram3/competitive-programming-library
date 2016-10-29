//          Copyright Jorge Aguirre 2015, Diego Ramirez 2016
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef CPL_GEOMETRY_POINT_2D_HPP
#define CPL_GEOMETRY_POINT_2D_HPP

#include <cpl/geometry/vector_2d.hpp> // vector2d
#include <cmath>                      // sqrt
#include <ostream>                    // ostream
#include <tuple>                      // tie
#include <type_traits>                // is_arithmetic, is_floating_point

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

// ===--------------------------------------------------===
//                    Point 2D Version 2
// ===--------------------------------------------------===

/// \brief This struct represents a 2D point, that is, a location in the
/// two-dimensional space.
///
template <typename T>
struct point2d {
  T x, y;

  point2d() = default;
  point2d(T x0, T y0) : x{x0}, y{y0} {}
};

/// \brief Adds a vector to a point.
///
/// \relates point2d
///
template <typename T>
point2d<T> operator+(const point2d<T>& p, const vector2d<T>& v) {
  return {p.x + v.x, p.y + v.y};
}

/// \brief Adds a vector to a point.
///
/// \relates point2d
///
template <typename T>
point2d<T> operator+(const vector2d<T>& v, const point2d<T>& p) {
  return {v.x + p.x, v.y + p.y};
}

/// \brief Subtracts a vector from a point.
///
/// \relates point2d
///
template <typename T>
point2d<T> operator-(const point2d<T>& p, const vector2d<T>& v) {
  return {p.x - v.x, p.y - v.y};
}

/// \brief Computes the displacement from \p q to \p p.
///
/// \relates point2d
///
template <typename T>
vector2d<T> operator-(const point2d<T>& p, const point2d<T>& q) {
  return {p.x - q.x, p.y - q.y};
}

/// \brief Compares two points for equality.
///
/// \relates point2d
///
template <typename T>
bool operator==(const point2d<T>& p, const point2d<T>& q) {
  return p.x == q.x && p.y == q.y;
}

/// \brief Compares two points for inequality.
///
/// \relates point2d
///
template <typename T>
bool operator!=(const point2d<T>& p, const point2d<T>& q) {
  return !(p == q);
}

/// \brief Serializes a point.
///
/// Writes to the given ostream in the form <tt>(x,y)</tt>.
///
/// \relates point2d
///
template <typename T>
std::ostream& operator<<(std::ostream& os, const point2d<T>& p) {
  return os << '(' << p.x << ',' << p.y << ')';
}

/// \brief Computes the squared distance between two points.
///
/// \relates point2d
///
template <typename T>
T squared_distance(const point2d<T>& p, const point2d<T>& q) {
  return norm(p - q);
}

/// \brief Computes the distance between two points.
///
/// \relates point2d
///
template <typename T>
T distance(const point2d<T>& p, const point2d<T>& q) {
  static_assert(std::is_floating_point<T>::value, "");
  return abs(p - q);
}

} // end namespace cpl

#endif // Header guard
