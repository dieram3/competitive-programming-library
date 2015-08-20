//          Copyright Jorge Aguirre July, August 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DJP_GEOMETRY_POINT_ORDER_HPP
#define DJP_GEOMETRY_POINT_ORDER_HPP

#include <algorithm> // For std::sort, std::is_sorted

#include <iostream>

namespace djp {

/// \brief Check if two points are in clockwise order according to a
/// center and a start angle.
///
/// \param center The reference point used as a center.
/// \param start Point used to specify the zero angle which is formed by the
/// vector <tt>(start - center)</tt>.
/// \param lhs Left hand side point.
/// \param rhs Right hand side point.
///
template<class Point>
bool cw_less(const Point &center, const Point &from, const Point &lhs, const Point &rhs) {
  const auto u = lhs - center;
  const auto v = rhs - center;

  const auto lhs_rotation = (from ^ u);
  const auto rhs_rotation = (from ^ v);

  if (lhs_rotation <= 0 && rhs_rotation > 0)
    return true;
  if (lhs_rotation > 0 && rhs_rotation <= 0)
    return false;

  if (lhs_rotation == 0 && rhs_rotation == 0) {
    // rotate 90 degrees in clockwise order
    const Point rotated_from = {from.y, -from.x};
    return (rotated_from ^ u) >= 0 && (rotated_from ^ v) < 0;
  }

  return (u ^ v) < 0;
}

/// \brief Check if two points are in counter clockwise order according to a
/// center and a start angle.
///
/// \param center The reference point used as a center.
/// \param start Point used to specify the zero angle which is formed by the
/// vector <tt>(start - center)</tt>.
/// \param lhs Left hand side point.
/// \param rhs Right hand side point.
///
template<class Point>
bool ccw_less(const Point &center, const Point &from, const Point &lhs, const Point &rhs) {
  const auto u = lhs - center;
  const auto v = rhs - center;

  const auto lhs_rotation = (from ^ u);
  const auto rhs_rotation = (from ^ v);

  if (lhs_rotation >= 0 && rhs_rotation < 0)
    return true;
  if (lhs_rotation < 0 && rhs_rotation >= 0)
    return false;

  if (lhs_rotation == 0 && rhs_rotation == 0) {
    // rotate 90 degrees in clockwise order
    const Point rotated_from = {from.y, -from.x};
    return (rotated_from ^ u) <= 0 && (rotated_from ^ v) > 0;
  }

  return (u ^ v) > 0;
}

/// \brief Sorts a range of points in clockwise order according to a center and
/// a start point.
///
/// \param center The reference point used as a center.
/// \param start Point used to specify the zero angle which is formed by the
/// vector <tt>(start - center)</tt>.
/// \param first The beginning of the range to be sorted.
/// \param last The end of the range to be sorted.
///
/// \par Complexity
/// Linearithmic in <tt>std::distance(first, last)</tt>.
///
template <class RandomIt, class Point>
void clockwise_sort(const Point &center, const Point &start, RandomIt first,
                    RandomIt last) {
  const auto from = start - center;
  std::sort(first, last, [center, from](const Point &lhs, const Point &rhs) {
    return cw_less(center, from, lhs, rhs);
  });
}

/// \brief Sorts a range of points in counter clockwise order according to a
/// center and a start angle.
///
/// \param center The reference point used as a center.
/// \param start Point used to specify the zero angle which is formed by the
/// vector <tt>(start - center)</tt>.
/// \param first The beginning of the range to be sorted.
/// \param last The end of the range to be sorted.
///
/// \par Complexity
/// Linearithmic in <tt>std::distance(first, last)</tt>.
///
template <class RandomIt, class Point>
void counter_clockwise_sort(const Point &center, const Point &start,
                            RandomIt first, RandomIt last) {
  const auto from = start - center;
  std::sort(first, last, [center, from](const Point &lhs, const Point &rhs) {
    return ccw_less(center, from, lhs, rhs);
  });
}

/// \brief Checks whether a range of points is sorted in clockwise order
/// according to a given center.
///
/// \param center The reference point used as a center.
/// \param first The beginning of the range to be examined.
/// \param last The end of the range to be examined.
///
/// \par Complexity
/// Linear in <tt>std::distance(first, last)</tt>.
///
template <class ForwardIt, class Point>
bool is_clockwise_sorted(const Point &center, ForwardIt first, ForwardIt last) {
  if (std::distance(first, last) == 0)
    return true;

  const auto from = *first - center;
  return std::is_sorted(first, last,
                        [center, from](const Point &lhs, const Point &rhs) {
                          return cw_less(center, from, lhs, rhs);
                        });
}

/// \brief Checks whether a range of points is sorted in counter clockwise order
/// according to a given center.
///
/// \param center The reference point used as a center.
/// \param first The beginning of the range to be examined.
/// \param last The end of the range to be examined.
///
/// \par Complexity
/// Linear in <tt>std::distance(first, last)</tt>.
///
template <class ForwardIt, class Point>
bool is_counter_clockwise_sorted(const Point &center, ForwardIt first,
                                 ForwardIt last) {
  if (std::distance(first, last) == 0)
    return true;

  const auto from = *first - center;
  return std::is_sorted(first, last,
                        [center, from](const Point &lhs, const Point &rhs) {
                          return ccw_less(center, from, lhs, rhs);
                        });
}

} // end namespace djp

#endif // Header guard
