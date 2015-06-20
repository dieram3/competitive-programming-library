//          Copyright Jorge Aguirre, Diego Ramírez April 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
/// \file
/// \brief Implements the Convex Hull algorithm.

#ifndef DJP_GEOMETRY_CONVEX_HULL_HPP
#define DJP_GEOMETRY_CONVEX_HULL_HPP

#include <algorithm>
#include <iterator>
#include <vector>
#include <cassert>
#include <type_traits>

namespace djp {

/// \brief Converts the range [first, last) in a convex set.
/// \returns An iterator to the end of the convex set.
/// \par Complexity
/// O(N), where N = last - first.
template <class BidirectionalIt, class ClockRotation>
BidirectionalIt make_convex_set(BidirectionalIt first, BidirectionalIt last,
                                ClockRotation &&ccw) {
  auto end = first;
  for (auto i = first; i != last; ++i) {
    while (end - first >= 2 && !ccw(end[-2], end[-1], *i))
      --end;
    *end++ = std::move(*i);
  }
  return --end;
}

/// \brief Variation of Andrew's monotone chain convex hull algorithm
/// \pre The points shall be sorted lexicographically.
/// \returns The convex hull sorted by counterclockwise order.
/// \par Complexity
/// O(N) where N = last - first.
template <class ForwardIt, class ClockRotation>
std::vector<typename std::iterator_traits<ForwardIt>::value_type>
convex_hull(ForwardIt first, ForwardIt last, ClockRotation &&ccw) {
  std::size_t n = std::distance(first, last);
  using point_t = typename std::iterator_traits<ForwardIt>::value_type;

  std::vector<point_t> hull(2 * n);
  std::copy(first, last, hull.begin());
  auto middle = make_convex_set(hull.begin(), hull.begin() + n, ccw);
  std::reverse_copy(first, last, middle);
  hull.erase(make_convex_set(middle, middle + n, ccw), hull.end());
  return hull;
}

/// \brief Variation of Andrew's monotone chain convex hull algorithm.
///
/// Reorders the points in the range [first, last] in such a way that all
/// elements inside the convex hull are placed in the range
/// [first, ch_begin) sorted lexicographically and all elements in the boundary
/// of the convex hull are placed in the range [ch_begin, last) sorted by
/// counterclockwise order.
/// \returns ch_begin.
/// \pre The range [first, last) shall be sorted lexicographically.
/// \par Complexity
/// O(N) where N == std::distance(first, last)
template <class ForwardIt, class ClockRotation>
ForwardIt convex_hull_partition(ForwardIt first, ForwardIt last,
                                ClockRotation &&ccw) {
  using point_t = typename std::iterator_traits<ForwardIt>::value_type;
  assert(std::is_sorted(first, last));

  std::vector<point_t> lhull(first, last);
  std::vector<point_t> uhull(lhull.rbegin(), lhull.rend());
  lhull.erase(make_convex_set(begin(lhull), end(lhull), ccw), end(lhull));
  uhull.erase(make_convex_set(begin(uhull), end(uhull), ccw), end(uhull));

  std::vector<point_t> tmp;
  std::set_difference(first, last, begin(lhull), end(lhull),
                      back_inserter(tmp));
  auto ch_begin = std::set_difference(begin(tmp), end(tmp), uhull.rbegin(),
                                      uhull.rend(), first);
  auto ch_middle = std::copy(begin(lhull), end(lhull), ch_begin);
#ifndef NDEBUG
  auto ch_end =
#endif
      std::copy(begin(uhull), end(uhull), ch_middle);
  assert(ch_end == last);
  return ch_begin;
}

} // end namespace djp

#endif // Header guard
