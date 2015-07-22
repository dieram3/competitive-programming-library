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
template<class BidirectionalIt>
BidirectionalIt make_convex_set(BidirectionalIt first, BidirectionalIt last,
                                bool with_collinear_points) {
  using point_t = typename BidirectionalIt::value_type;

  auto end = first;
  for (auto i = first; i != last; ++i) {
    auto is_clockwise = [&](const point_t &p0, const point_t &p1, const point_t &p2) {
      auto cross = [&](const point_t &p, const point_t &q) {
        return p.x * q.y - p.y * q.x;
      };

      return with_collinear_points ? cross(p1 - p0, p2 - p0) < 0 :
             cross(p1 - p0, p2 - p0) <= 0;
    };

    while (end - first >= 2 && is_clockwise(end[-2], end[-1], *i))
      --end;
    *end++ = std::move(*i);
  }
  return --end;
}

/// \brief Variation of Andrew's monotone chain convex hull algorithm
/// \pre The points shall be sorted lexicographically.
/// \pre Point class must implement the `-` operator and must have `x` and `y` as public members.
/// \returns The convex hull sorted by counterclockwise order.
/// \par Complexity
/// O(N) where N = last - first.
template<class ForwardIt>
std::vector<typename std::iterator_traits<ForwardIt>::value_type>
convex_hull(ForwardIt first, ForwardIt last,
            bool with_collinear_points = false) {
  std::size_t n = std::distance(first, last);
  using point_t = typename std::iterator_traits<ForwardIt>::value_type;

  std::vector<point_t> hull(2 * n);
  std::copy(first, last, hull.begin());
  auto middle = make_convex_set(hull.begin(), hull.begin() + n,
                                with_collinear_points);
  std::reverse_copy(first, last, middle);
  hull.erase(make_convex_set(middle, middle + n, with_collinear_points),
             hull.end());
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
/// \pre Point class must implement the `-` operator and must have `x` and `y` as public members.
/// \par Complexity
/// O(N) where N == std::distance(first, last)
template<class ForwardIt>
ForwardIt convex_hull_partition(ForwardIt first, ForwardIt last,
                                bool with_collinear_points = false) {
  using point_t = typename std::iterator_traits<ForwardIt>::value_type;
  assert(std::is_sorted(first, last));

  std::vector<point_t> lhull(first, last);
  std::vector<point_t> uhull(lhull.rbegin(), lhull.rend());
  lhull.erase(make_convex_set(begin(lhull), end(lhull), with_collinear_points),
              end(lhull));
  uhull.erase(make_convex_set(begin(uhull), end(uhull), with_collinear_points),
              end(uhull));

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
