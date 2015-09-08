//          Copyright Jorge Aguirre September 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DJP_GEOMETRY_POINT_IN_POLYGON_HPP
#define DJP_GEOMETRY_POINT_IN_POLYGON_HPP

#include <vector>  // For std::vector
#include <cstddef> // For std::size_t, std::ptrdiff_t

namespace djp {

/// \brief Checks if a point is inside or in the boundary of a simple polygon.
///
/// Uses a modern version of the Winding number algorithm which does not involve
/// trigonometric operations, to check whether the point \p p0 is in the
/// simple polygon defined by \p poly.
///
/// \param p0 The point to test.
/// \param poly The polygon to test.
///
/// \returns \c true if point \p p0 is in the polygon \p poly, \c false
/// otherwise.
///
/// \par Complexity
/// Linear in <tt>poly.size()</tt>.
///
template <typename Point>
bool point_in_polygon(const Point &p0, const std::vector<Point> &poly) {
  std::ptrdiff_t wn = 0;

  auto check_crossing = [&](const Point &q0, const Point &q1) {
    auto det = ((q1 - q0) ^ (p0 - q0));
    if (q0.y <= p0.y) {
      if (q1.y > p0.y)
        wn += det > 0;
    } else {
      if (q1.y <= p0.y)
        wn -= det < 0;
    }
    return det;
  };

  auto in_boundigbox = [&](const Point &q0, const Point &q1) {
    return (p0.x <= std::max(q0.x, q1.x) && p0.x >= std::min(q0.x, q1.x) &&
            p0.y <= std::max(q0.y, q1.y) && p0.y >= std::min(q0.y, q1.y));
  };

  const std::size_t N = poly.size();
  for (std::size_t i = 0; i + 1 < N; ++i) {
    if (check_crossing(poly[i], poly[i + 1]))
      continue;
    if (in_boundigbox(poly[i], poly[i + 1]))
      return true;
  }
  if (!check_crossing(poly[N - 1], poly[0])) {
    if (in_boundigbox(poly[N - 1], poly[0]))
      return true;
  }

  return wn != 0;
}

} // end namespace djp

#endif // Header guard
