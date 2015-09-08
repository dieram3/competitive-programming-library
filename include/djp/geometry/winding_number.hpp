//          Copyright Jorge Aguirre September 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DJP_GEOMETRY_POINT_IN_POLYGON_HPP
#define DJP_GEOMETRY_POINT_IN_POLYGON_HPP

#include <vector>  // For std::vector
#include <cstddef> // For std::size_t, std::ptrdiff_t

namespace djp {

/// \brief Checks if a point is inside a polygon.
///
/// Uses a modern version of the Winding number algorithm which does not involve
/// trigonometric operations, to check whether the point \p p0 is inside (but
/// not in the boundary) the polygon defined by \p poly . The polygon is allowed
/// to be nonsimple.
///
/// \param p0 The point to test.
/// \param poly The polygon to test.
///
/// \returns \c true if point \p p0 is inside the polygon \p poly, \c false
/// otherwise.
///
/// \par Complexity
/// Linear in <tt>poly.size()</tt>.
///
template <typename Point>
bool point_in_polygon(const Point &p0, const std::vector<Point> &poly) {
  std::ptrdiff_t wn = 0;

  auto check_crossing = [&](const Point &q0, const Point &q1) {
    if (q0.y <= p0.y) {
      if (q1.y > p0.y)
        wn += ((q1 - q0) ^ (p0 - q0)) > 0;
    } else {
      if (q1.y <= p0.y)
        wn -= ((q1 - q0) ^ (p0 - q0)) < 0;
    }
  };
  const std::size_t N = poly.size();
  for (std::size_t i = 0; i + 1 < N; ++i)
    check_crossing(poly[i], poly[i + 1]);
  check_crossing(poly[N - 1], poly[0]);

  return wn != 0;
}

} // end namespace djp

#endif // Header guard
