//          Copyright Diego Ramirez 2016
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef CPL_GEOMETRY_VECTOR_2D_HPP
#define CPL_GEOMETRY_VECTOR_2D_HPP

#include <cmath>       // sqrt
#include <ostream>     // ostream
#include <type_traits> // is_floating_point

namespace cpl {

/// \brief This struct represents a 2D vector, that is, a displacement in the
/// two-dimensional space.
///
template <typename T>
struct vector2d {
  T x, y;

  vector2d() = default;
  vector2d(T x0, T y0) : x{x0}, y{y0} {}
};

/// \brief Computes the sum of two vectors.
///
/// \relates vector2d
///
template <typename T>
vector2d<T> operator+(const vector2d<T>& u, const vector2d<T>& v) {
  return {u.x + v.x, u.y + v.y};
}

/// \brief Computes the difference between two vectors.
///
/// \relates vector2d
///
template <typename T>
vector2d<T> operator-(const vector2d<T>& u, const vector2d<T>& v) {
  return {u.x - v.x, u.y - v.y};
}

/// \brief Computes the scalar product between \p v and \p k.
///
/// \relates vector2d
///
template <typename T>
vector2d<T> operator*(const vector2d<T>& v, const T& k) {
  return {v.x * k, v.y * k};
}

/// \brief Computes the scalar product between \p k and \p v.
///
/// \relates vector2d
///
template <typename T>
vector2d<T> operator*(const T& k, const vector2d<T>& v) {
  return {k * v.x, k * v.y};
}

/// \brief Computes the dot product between two vectors.
///
/// \relates vector2d
///
template <typename T>
T dot(const vector2d<T>& u, const vector2d<T>& v) {
  return u.x * v.x + u.y * v.y;
}

/// \brief Computes the magnitude of the cross product between two vectors.
///
/// \relates vector2d
///
template <typename T>
T cross(const vector2d<T>& u, const vector2d<T>& v) {
  return u.x * v.y - u.y * v.x;
}

/// \brief Computes the squared magnitude of a vector.
///
/// \relates vector2d
///
template <typename T>
T norm(const vector2d<T>& v) {
  return dot(v, v);
}

/// \brief Computes the magnitude of a vector.
///
/// \relates vector2d
///
template <typename T>
T abs(const vector2d<T>& v) {
  static_assert(std::is_floating_point<T>::value, "");
  return std::sqrt(norm(v));
}

/// \brief Compares two vectors for equality.
///
/// \relates vector2d
///
template <typename T>
bool operator==(const vector2d<T>& u, const vector2d<T> v) {
  return u.x == v.x && u.y == v.y;
}

/// \brief Compares two vectors for inequality.
///
/// \relates vector2d
///
template <typename T>
bool operator!=(const vector2d<T>& u, const vector2d<T>& v) {
  return !(u == v);
}

/// \brief Serializes a vector.
///
/// Writes to the given ostream in the form <tt>(x,y)</tt>.
///
/// \relates vector2d
///
template <typename T>
std::ostream& operator<<(std::ostream& os, const vector2d<T>& v) {
  return os << '(' << v.x << ',' << v.y << ')';
}

} // end namespace cpl

#endif // Header guard
