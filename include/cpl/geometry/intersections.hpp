//          Copyright Diego Ramirez 2016
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef CPL_GEOMETRY_INTERSECTIONS_HPP
#define CPL_GEOMETRY_INTERSECTIONS_HPP

#include <cpl/geometry/point_2d.hpp>  // point_2d
#include <cpl/geometry/vector_2d.hpp> // vector_2d, cross
#include <type_traits>                // is_floating_point

namespace cpl {

/// \brief Finds the intersection point between two non-parallel lines.
///
/// \param p0 The first point of the first line.
/// \param p1 The second point of the first line.
/// \param q0 The first point of the second line.
/// \param q1 The first point of the second line.
///
/// \returns The intersection point.
///
/// \pre The line defined by `p0` and `p1` shall not be parallel to the line
/// defined by `q0` and `q1`.
///
/// \par Complexity
/// Constant.
///
template <typename T>
point2d<T> find_lines_intersection(const point2d<T>& p0, const point2d<T>& p1,
                                   const point2d<T>& q0, const point2d<T>& q1) {
  static_assert(std::is_floating_point<T>::value, "");

  const vector2d<T> u{p1 - p0}, v{q1 - q0}, w{p0 - q0};
  const T s = cross(w, v) / cross(v, u);
  return p0 + s * u;
}

} // end namespace cpl

#endif // Header guard
