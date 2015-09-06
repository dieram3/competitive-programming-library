//          Copyright Jorge Aguirre September 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DJP_GEOMETRY_SHOELACE_HPP
#define DJP_GEOMETRY_SHOELACE_HPP

#include <djp/geometry/point_2d.hpp> // For djp::point

#include <vector>  // For std::vector
#include <cassert> // For assert
#include <cmath>   // For std::abs
#include <cstddef> // For std::size_t

namespace djp {

/// \brief Computes the area of a simple polygon.
///
/// Uses the Gauss's area formula to compute the area of the polygon \p poly.
///
/// \param poly The polygon to determine area
///
/// \pre <tt> poly.size() >= 3 </tt>
///
/// \returns Twice the area of the polygon \p poly.
///
template <typename T>
T shoelace_area(const std::vector<point<T>> &poly) {
  assert(poly.size() >= 3);
  T sum = 0;
  const std::size_t N = poly.size();
  for (std::size_t i = 1; i + 1 < N; ++i)
    sum += poly[i].x * (poly[i + 1].y - poly[i - 1].y);
  sum += poly[0].x * (poly[1].y - poly[N - 1].y);
  sum += poly[N - 1].x * (poly[0].y - poly[N - 2].y);
  return std::abs(sum);
}

} // namespace djp

#endif // HEADER GUARD
