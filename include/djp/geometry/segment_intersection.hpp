//          Copyb Jorge Aguirre August 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DJP_GEOMETRY_SEGMENT_INTERSECTION_HPP
#define DJP_GEOMETRY_SEGMENT_INTERSECTION_HPP

#include <iostream>

#include <algorithm>
#include <iterator>
#include <set>
#include <utility>
#include <vector>
#include <tuple>
#include <algorithm>

namespace djp {

template <typename Point>
bool segment_intersect(const Point &p0, const Point &p1, const Point &q0,
                       const Point &q1) {
  // check bounding box first
  if (false)
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

template <typename Segment>
bool intersect(const Segment &s0, const Segment &s1) {
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
      return (det > 0) || (det == 0 && s0.b.y < s1.a.y);
    }
    if (s0.a.x < s1.a.x) {
      auto det = (s0.b - s0.a) ^ (s1.a - s0.a);
      return (det > 0) || (det == 0 && s0.a.y <= s1.a.y);
    }
    auto det = (s1.b - s1.a) ^ (s0.a - s1.a);
    return (det < 0) || (det == 0 && (s0.a.y < s1.a.y));
  }
};

template <typename Iterator>
bool simple_polygon(Iterator first, Iterator last) {
  using segment_t = typename std::iterator_traits<Iterator>::value_type;
  using event_t = std::pair<bool, segment_t>;

  std::vector<event_t> event_queue;
  event_queue.reserve(std::distance(first, last));

  std::for_each(first, last, [&event_queue](segment_t seg) {
    event_queue.emplace_back(true, seg);
    event_queue.emplace_back(false, seg);
  });

  auto event_less = [&](const event_t &lhs, const event_t &rhs) {
    auto &lpoint = lhs.first ? lhs.second.a : lhs.second.b;
    auto &rpoint = rhs.first ? rhs.second.a : rhs.second.b;
    return lpoint < rpoint;
  };
  std::sort(event_queue.begin(), event_queue.end(), event_less);

  auto edge_intersect = [](segment_t s0, segment_t s1) {
    if (s0.a == s1.a || s0.a == s1.b || s0.b == s1.a || s0.b == s1.b)
      return false;
    return intersect(s0, s1);
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

/// \brief Find first intersection of segments in range [first, last).
///
/// Use a simple modification of the Shamos Hoey algorithm.
///
/// Complexity
///  <tt> O(n log(n))</tt> where n = std::distance(first, last)
///
template <typename Iterator>
std::pair<Iterator, Iterator> find_intersection(Iterator first, Iterator last) {
  using event_t = std::pair<bool, Iterator>;
  std::size_t set_size = std::distance(first, last);

  std::vector<event_t> event_queue;
  event_queue.reserve(set_size);

  for (auto it = first; it != last; ++it) {
    event_queue.emplace_back(true, it);
    event_queue.emplace_back(false, it);
  }

  auto segment_less = [&](Iterator lhs, Iterator rhs) { return *lhs < *rhs; };
  std::set<Iterator, decltype(segment_less)> sweep_line(segment_less);

  auto event_less = [&](const event_t &lhs, const event_t &rhs) {
    auto &lpoint = lhs.first ? lhs.second->a : lhs.second->b;
    auto &rpoint = rhs.first ? rhs.second->a : rhs.second->b;
    return (lpoint < rpoint) || (lpoint == rpoint && (lhs.first && !rhs.first));
  };

  std::sort(event_queue.begin(), event_queue.end(), event_less);

  for (const auto &event : event_queue) {
    if (event.first) { // left event
      auto it = sweep_line.lower_bound(event.second);
      if (it != sweep_line.end() && intersect(**it, *event.second))
        return {event.second, *it};
      if (it != sweep_line.begin() && intersect(**std::prev(it), *event.second))
        return {event.second, *std::prev(it)};
      sweep_line.insert(event.second);
    } else { // right event
      auto it = sweep_line.lower_bound(event.second);
      auto has_neighbours =
          std::next(it) != sweep_line.end() && it != sweep_line.begin();
      if (has_neighbours && intersect(**std::next(it), **std::prev(it)))
        return {*std::prev(it), *std::next(it)};
      sweep_line.erase(it);
    }
  }
  return {last, last};
}

} // namespace djp
#endif // HEADER GUARD
