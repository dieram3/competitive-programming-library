//          Copyright Jorge Aguirre August 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef CPL_GEOMETRY_SEGMENT_INTERSECTION_HPP
#define CPL_GEOMETRY_SEGMENT_INTERSECTION_HPP

#include <algorithm> // For std::minmax, std::for_each, std::sort
#include <cstddef>   // For std::size_t
#include <iterator>  // For std::iterator_traits, std::next, std::prev
#include <set>       // For std::set
#include <tuple>     // For std::tie
#include <utility>   // For std::pair
#include <vector>    // For std::vector

namespace cpl {

/// \brief Check if two line segments intersect.
///
/// Checks whether the line segment defined by \p p0 and \p p1 intersects with
/// the line segment defined by \p q0 and \p q1.
///
template <typename Point>
bool segment_intersect(const Point &p0, const Point &p1, const Point &q0,
                       const Point &q1) {
  // Check bounding box first.
  if (std::max(p0.x, p1.x) < std::min(q0.x, q1.x) ||
      std::max(q0.x, q1.x) < std::min(p0.x, p1.x) ||
      std::max(p0.y, p1.y) < std::min(q0.y, q1.y) ||
      std::max(q0.y, q1.y) < std::min(p0.y, p1.y))
    return false;

  auto lrot = (p1 - p0) ^ (q0 - p0);
  auto rrot = (p1 - p0) ^ (q1 - p0);
  if (lrot * rrot > 0)
    return false;
  lrot = (q1 - q0) ^ (p0 - q0);
  rrot = (q1 - q0) ^ (p1 - q0);
  if (lrot * rrot > 0)
    return false;
  return true;
}

/// Checks if two line segments intersect.
///
template <typename Segment>
bool segment_intersect(const Segment &s0, const Segment &s1) {
  return segment_intersect(s0.a, s0.b, s1.a, s1.b);
}

template <typename Point>
struct segment {
  segment(const Point &p0, const Point &p1) {
    std::tie(a, b) = std::minmax(p0, p1);
  }

  Point a; // leftmost point of segment
  Point b; // rightmost point of segment

  friend bool operator<(const segment &s0, const segment &s1) {
    if (s0.a == s1.a) {
      auto det = (s0.b - s0.a) ^ (s1.b - s0.a);
      return (det > 0) || (det == 0 && s0.b < s1.b);
    }
    if (s0.a.x < s1.a.x) {
      auto det = (s0.b - s0.a) ^ (s1.a - s0.a);
      if (det == 0)
        return (s0.a.y < s1.a.y) || (s0.a.y == s1.a.y && s0.b.x > s1.a.x);
      return (det > 0);
    } else if (s0.a.x > s1.a.x) {
      auto det = (s1.b - s1.a) ^ (s0.a - s1.a);
      if (det == 0)
        return (s0.a.y < s1.a.y) || (s0.a.y == s1.a.y && s0.b.x > s1.a.x);
      return (det < 0);
    }
    return s0.a.y < s1.a.y;
  }
};

/// \brief Checks whether a polygon is simple.
///
/// Uses the Shamos-Hoey algorithm to check if the polygon defined by the range
/// of segments <tt>[first, last)</tt> is simple. A polygon is simple if no
/// two line segments intersect. The only exception are the vertices that
/// connect adjacent segments (otherwise the polygon could not be chained).
///
/// \param first The beginning of range to check.
/// \param last The end of range to check.
///
/// \pre Segments must be \a less comparable. The comparison must return \c
/// true if the left-hand segment is considered \a below the right-hand segment.
///
/// \pre The given range must represent a valid polygon, that is, a sequence of
/// segments where the right-point of segment <tt>(i % N)</tt> is equal to the
/// left-point of segment <tt>((i + 1) % N)</tt> being \c N the total numbers of
/// segments.
///
/// \par Complexity
///  At most <tt>O(n*log(n))</tt> segment-comparisons where
/// <tt>n = std::distance(first, last)</tt>
///
template <typename ForwardIt>
bool simple_polygon(ForwardIt first, ForwardIt last) {
  using segment_t = typename std::iterator_traits<ForwardIt>::value_type;
  using event_t = std::pair<bool, segment_t>;

  std::vector<event_t> event_queue;
  event_queue.reserve(std::distance(first, last));

  std::for_each(first, last, [&event_queue](const segment_t &seg) {
    event_queue.emplace_back(true, seg);
    event_queue.emplace_back(false, seg);
  });

  auto event_less = [&](const event_t &lhs, const event_t &rhs) {
    auto &lpoint = lhs.first ? lhs.second.a : lhs.second.b;
    auto &rpoint = rhs.first ? rhs.second.a : rhs.second.b;
    return lpoint < rpoint;
  };
  std::sort(event_queue.begin(), event_queue.end(), event_less);

  auto edge_intersect = [](const segment_t &s0, const segment_t &s1) {
    if (s0.a == s1.a || s0.a == s1.b || s0.b == s1.a || s0.b == s1.b)
      return false;
    return segment_intersect(s0, s1);
  };
  std::set<segment_t> sweep_line;

  for (const auto &event : event_queue) {

    if (event.first) { // left event
      auto it = sweep_line.lower_bound(event.second);
      if (it != sweep_line.end() && edge_intersect(*it, event.second))
        return false;
      if (it != sweep_line.begin() &&
          edge_intersect(*std::prev(it), event.second))
        return false;
      sweep_line.insert(event.second);
    } else { // right event
      auto it = sweep_line.lower_bound(event.second);
      auto has_neighbours =
          std::next(it) != sweep_line.end() && it != sweep_line.begin();
      if (has_neighbours && edge_intersect(*std::next(it), *std::prev(it)))
        return false;
      sweep_line.erase(it);
    }
  }
  return true;
}

/// \brief Finds a pair of intersecting segments in the given range.
///
/// Uses a sweep line based on the Shamos-Hoey algorithm to find a pair of
/// intersecting segments in the range <tt>[first ,last)</tt>. The finding order
/// is given by the sweep line.
///
/// \param first The beginning range to check.
/// \param last The end of range to check.
///
/// \returns The first pair of intersecting segments which this function finds
/// according to the sweep line order. If no intersection exist then returns
/// <tt>std::make_pair(last, last)</tt>.
///
/// \pre Segments must be \a less comparable. The comparison must return \c
/// true if the left-hand segment is considered \a below the right-hand segment.
///
/// \par Complexity
///  At most <tt>O(n*log(n))</tt> segment-comparisons where
/// <tt>n = std::distance(first, last)</tt>
///
template <typename ForwardIt>
std::pair<ForwardIt, ForwardIt> find_intersection(ForwardIt first,
                                                  ForwardIt last) {
  using event_t = std::pair<bool, ForwardIt>;
  const size_t set_size = std::distance(first, last);

  std::vector<event_t> event_queue;
  event_queue.reserve(set_size);

  for (auto it = first; it != last; ++it) {
    event_queue.emplace_back(true, it);
    event_queue.emplace_back(false, it);
  }

  auto segment_less = [&](ForwardIt lhs, ForwardIt rhs) { return *lhs < *rhs; };
  std::set<ForwardIt, decltype(segment_less)> sweep_line(segment_less);

  auto event_less = [&](const event_t &lhs, const event_t &rhs) {
    auto &lpoint = lhs.first ? lhs.second->a : lhs.second->b;
    auto &rpoint = rhs.first ? rhs.second->a : rhs.second->b;
    return (lpoint < rpoint) || (lpoint == rpoint && (lhs.first && !rhs.first));
  };

  std::sort(event_queue.begin(), event_queue.end(), event_less);

  for (const auto &event : event_queue) {
    if (event.first) { // left event
      auto it = sweep_line.lower_bound(event.second);
      if (it != sweep_line.end() && segment_intersect(**it, *event.second))
        return {event.second, *it};
      if (it != sweep_line.begin() &&
          segment_intersect(**std::prev(it), *event.second))
        return {event.second, *std::prev(it)};
      sweep_line.insert(event.second);
    } else { // right event
      auto it = sweep_line.lower_bound(event.second);
      auto has_neighbours =
          std::next(it) != sweep_line.end() && it != sweep_line.begin();
      if (has_neighbours && segment_intersect(**std::next(it), **std::prev(it)))
        return {*std::prev(it), *std::next(it)};
      sweep_line.erase(it);
    }
  }
  return {last, last};
}

} // end namespace cpl

#endif // Header guard
