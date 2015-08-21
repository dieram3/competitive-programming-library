//          Copyright Jorge Aguirre July, August 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DJP_GEOMETRY_POINT_ORDER_HPP
#define DJP_GEOMETRY_POINT_ORDER_HPP

#include <algorithm> // For std::sort, std::is_sorted

#include <iostream>

namespace djp {
template<class Point>
unsigned clock_rotation(const Point &reference, const Point &p) {
  auto rotation = reference ^ p;
  if (rotation != 0)
    return rotation < 0 ? 1 : 3;
  const Point rotated_from = {reference.y, -reference.x};
  return (rotated_from ^ p) > 0 ? 0 : 2;
}

template<class Point>
int cw_compare(const Point &center, const Point &from, const Point &lhs, const Point &rhs) {
  const auto u = lhs - center;
  const auto v = rhs - center;

  unsigned urot = clock_rotation(from, u);
  unsigned vrot = clock_rotation(from, v);

  if (urot == vrot){
    if(urot == 0 || urot == 2)
      return 0;
    return (u ^ v);
  }
  return urot < vrot ? -1 : 1;
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
    return cw_compare(center, from, lhs, rhs) < 0;
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
    return cw_compare(center, from, lhs, rhs) > 0;
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
  if (first == last)
    return true;

  const auto from = *first - center;
  return std::is_sorted(first, last,
                        [center, from](const Point &lhs, const Point &rhs) {
                          return cw_compare(center, from, lhs, rhs) < 0;
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
  if (first == last)
    return true;

  const auto from = *first - center;
  return std::is_sorted(first, last,
                        [center, from](const Point &lhs, const Point &rhs) {
                          return cw_compare(center, from, lhs, rhs) > 0;
                        });
}

} // end namespace djp

#endif // Header guard
