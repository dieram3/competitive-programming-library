//          Copyright Jorge Aguirre 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef CPL_GEOMETRY_POINT_IN_POLYGON_HPP
#define CPL_GEOMETRY_POINT_IN_POLYGON_HPP

#include <cstddef> // size_t, ptrdiff_t
#include <vector>  // vector

namespace cpl {

/// \brief Checks if a point lies inside a simple polygon.
///
/// Uses a modern version of the winding number algorithm which does not involve
/// trigonometric operations to check whether the point \p p lies inside the
/// simple polygon defined by \p poly.
///
/// \param p The point to test.
/// \param poly The polygon to examine.
///
/// \pre The point \p p shall not be on the boundary of the polygon, otherwise
/// it is undefined behaviour.
///
/// \returns \c true if point \p p is inside the polygon \p poly, \c false
/// otherwise.
///
/// \par Complexity
/// Linear in <tt>poly.size()</tt>.
///
template <typename Point>
bool point_in_polygon(const Point& p, const std::vector<Point>& poly) {
  std::ptrdiff_t wn = 0;

  auto check_crossing = [&](const Point& q0, const Point& q1) {
    auto det = (q1 - q0) ^ (p - q0);
    if (q0.y <= p.y) {
      if (q1.y > p.y)
        wn += det > 0;
    } else {
      if (q1.y <= p.y)
        wn -= det < 0;
    }
  };
  const size_t n = poly.size();
  for (size_t i = 0; i + 1 < n; ++i)
    check_crossing(poly[i], poly[i + 1]);
  check_crossing(poly[n - 1], poly[0]);
  return wn != 0;
}

} // end namespace cpl

#endif // Header guard
