//          Copyright Diego Ramirez 2016
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef CPL_GEOMETRY_ROTATIONS_HPP
#define CPL_GEOMETRY_ROTATIONS_HPP

#include <cmath>       // sin, cos
#include <type_traits> // is_floating_point, is_same

namespace cpl {

/// \brief Rotates a point 90 degrees counterclockwise about the origin.
///
/// \param p The point to be rotated.
///
/// \returns The rotated point.
///
template <typename Point>
Point rotate_90_ccw(const Point& p) {
  return Point(-p.y, p.x);
}

/// \brief Rotates a point 180 degrees counterclockwise about the origin.
///
/// \param p The point to be rotated.
///
/// \returns The rotated point.
///
template <typename Point>
Point rotate_180_ccw(const Point& p) {
  return Point(-p.x, -p.y);
}

/// \brief Rotates a point 270 degrees counterclockwise about the origin.
///
/// \param p The point to be rotated.
///
/// \returns The rotated point.
///
template <typename Point>
Point rotate_270_ccw(const Point& p) {
  return Point(p.y, -p.x);
}

/// \brief Rotates a point counterclockwise through an angle \p theta about the
/// origin.
///
/// \param p The point to be rotated.
/// \param theta The rotation angle in radians.
///
/// \returns The rotated point.
///
template <typename Point, typename T>
Point rotate_ccw(const Point& p, const T theta) {
  static_assert(std::is_floating_point<T>::value, "");
  static_assert(std::is_same<decltype(p.x), T>::value, "");

  return Point(p.x * std::cos(theta) - p.y * std::sin(theta),
               p.x * std::sin(theta) + p.y * std::cos(theta));
}

} // end namespace cpl

#endif // Header guard
