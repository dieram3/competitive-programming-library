//          Copyright Diego Ramirez 2016
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef CPL_GEOMETRY_ROTATIONS_HPP
#define CPL_GEOMETRY_ROTATIONS_HPP

#include <cpl/geometry/vector_2d.hpp> // vector2d
#include <cmath>                      // sin, cos
#include <type_traits>                // is_floating_point

namespace cpl {

/// \brief Rotates a vector 90 degrees counterclockwise.
///
/// \param v The vector to be rotated.
///
/// \returns The rotated vector.
///
template <typename T>
vector2d<T> rotate_90_ccw(const vector2d<T>& v) {
  return {-v.y, v.x};
}

/// \brief Rotates a vector 180 degrees counterclockwise.
///
/// \param v The vector to be rotated.
///
/// \returns The rotated vector.
///
template <typename T>
vector2d<T> rotate_180_ccw(const vector2d<T>& v) {
  return -v;
}

/// \brief Rotates a vector 270 degrees counterclockwise.
///
/// \param v The vector to be rotated.
///
/// \returns The rotated vector.
///
template <typename T>
vector2d<T> rotate_270_ccw(const vector2d<T>& v) {
  return {v.y, -v.x};
}

/// \brief Rotates a vector counterclockwise through an angle \p theta.
///
/// \param v The vector to be rotated.
/// \param theta The rotation angle in radians.
///
/// \returns The rotated vector.
///
template <typename T>
vector2d<T> rotate_ccw(const vector2d<T>& v, const T theta) {
  static_assert(std::is_floating_point<T>::value, "");

  return {v.x * std::cos(theta) - v.y * std::sin(theta),
          v.x * std::sin(theta) + v.y * std::cos(theta)};
}

} // end namespace cpl

#endif // Header guard
