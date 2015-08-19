//          Copyright Jorge Aguirre July 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DJP_GEOMETRY_SORT_POINTS_HPP
#define DJP_GEOMETRY_SORT_POINTS_HPP

#include <algorithm>

namespace djp {
/// \brief Sorts a range of points in clockwise order according to a center and an
/// angle of beginning.
/// \param center The center used as a reference for sorting.
/// \param start Point used to specify the 0 angle, formed by the vector
/// (start - center).
/// \param first The beginning of the range to be sorted.
/// \param last The end of the range to be sorted.
/// \par Complexity
/// O(N log N) where <tt>N = std::distance(first, last)</tt>
template <class RandomIt, class Point>
void clockwise_sort(const Point &center, const Point &start, RandomIt first,
                    RandomIt last) {
  const auto from = start - center;
  std::sort(first, last, [center, from](const Point &lhs, const Point &rhs) {
    const auto u = lhs - center;
    const auto v = rhs - center;

    const auto lhs_rotation = (from ^ u);
    const auto rhs_rotation = (from ^ v);

    if (lhs_rotation < 0 && rhs_rotation > 0)
      return true;
    if (lhs_rotation > 0 && rhs_rotation < 0)
      return false;

    const auto det = (u ^ v);
    return det == 0 ? norm2(u) < norm2(v) : det < 0;
  });
}
/// \brief Sorts a range of points in counter clockwise order according to a
/// center and an angle of beginning.
/// \param center The center used as a reference for sorting.
/// \param start Point used to specify the 0 angle, formed by the vector
/// (start - center).
/// \param first The beginning of the range to be sorted.
/// \param last The end of the range to be sorted.
/// \par Complexity
/// O(N log N) where <tt>N = std::distance(first, last)</tt>
template <class RandomIt, class Point>
void counter_clockwise_sort(const Point &center, const Point &start,
                            RandomIt first, RandomIt last) {
  const auto from = start - center;
  std::sort(first, last, [center, from](const Point &lhs, const Point &rhs) {
    const auto u = lhs - center;
    const auto v = rhs - center;

    const auto lhs_rotation = (from ^ u);
    const auto rhs_rotation = (from ^ v);

    if (lhs_rotation > 0 && rhs_rotation < 0)
      return true;
    if (lhs_rotation < 0 && rhs_rotation > 0)
      return false;

    const auto det = (u ^ v);
    return det == 0 ? norm2(u) < norm2(v) : det > 0;
  });
}
/// \brief Checks if the points in range [first, last) are sorted in clokwise order
/// according to a center.
/// \param first The beginning of the range to be examined.
/// \param last The end of the range to be examined.
/// \par Complexity
/// O(N) where <tt>N = std::distance(first, last) </tt>
template <class ForwardIt, class Point>
bool is_clockwise_sorted(const Point &center, ForwardIt first, ForwardIt last) {
  return std::is_sorted(first, last,
                        [&center](const Point &lhs, const Point &rhs) {
                          const auto u = lhs - center;
                          const auto v = rhs - center;

                          const auto det = (u ^ v);
                          return det == 0 ? norm2(u) < norm2(v) : det < 0;
                        });
}
/// \brief Checks if the points in range [first, last) are sorted in counter clokwise
/// order according to a center.
/// \param first The beginning of the range to be examined.
/// \param last The end of the range to be examined.
/// \par Complexity
/// O(N) where <tt>N = std::distance(first, last) </tt>
template <class ForwardIt, class Point>
bool is_ccw_sorted(const Point &center, ForwardIt first, ForwardIt last) {
  return std::is_sorted(first, last,
                        [&center](const Point &lhs, const Point &rhs) {
                          const auto u = lhs - center;
                          const auto v = rhs - center;

                          const auto det = (u ^ v);
                          return det == 0 ? norm2(u) < norm2(v) : det > 0;
                        });
}

} // djp

#endif // HEADER GUARD
