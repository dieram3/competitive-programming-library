//          Copyright Jorge Aguirre August 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DJP_GEOMETRY_POINT_IN_POLYGON_HPP
#define DJP_GEOMETRY_POINT_IN_POLYGON_HPP

#include <algorithm> // For std::next
#include <cstdint>   // For std::int64_t
#include <vector>    // For std::vector

namespace djp {
/// \brief Uses winding number test for a point in a polygon.
///
/// Computes how many times the polygon \p poly winds around the point \p p0. A
/// point is outside only when the polygon doesn't wind around the point at all
/// which is when the winding number <tt>wn = 0</tt>.
///
/// \param p0 The point to test.
/// \param poly The polygon beign tested.
///
/// \par Complexity
/// Linear <tt>O(n)</tt> where <tt>n = poly.size()</tt>
///
/// \return \c true if point \p p0 is in polygon.
///
template <typename Point>
bool point_in_polygon(const Point &p0, const std::vector<Point> &poly) {
  std::int64_t wn = 0;

  auto check_crossing = [&](const Point &q0, const Point &q1) {
    if (q0.y <= p0.y) {
      if (q1.y > p0.y)
        wn += ((q1 - q0) ^ (p0 - q0)) > 0;
    } else {
      if (q1.y <= p0.y)
        wn -= ((q1 - q0) ^ (p0 - q0)) < 0;
    }
  };

  std::size_t N = poly.size();
  for (std::size_t i = 0; i + 1 < N; ++i)
    check_crossing(poly[i], poly[i + 1]);
  check_crossing(poly[N - 1], poly[0]);

  return wn != 0;
}
}
#endif // HEADER GUARD
