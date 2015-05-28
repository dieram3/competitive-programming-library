//          Copyright Jorge Aguirre, Diego Ramírez April 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <djp/geometry/convex_hull.hpp>
#include <gtest/gtest.h>
#include <tuple>
#include <algorithm>
#include <iterator>
#include <cstdint>

namespace {

template <class T> struct point {
  T x, y, z;

  point(T x = 0, T y = 0, T z = 0) : x{x}, y{y}, z{z} {}

  friend bool operator<(const point &p, const point &q) {
    return std::tie(p.x, p.y, p.z) < std::tie(q.x, q.y, q.z);
  }

  friend point operator-(const point &p, const point &q) {
    return {(p.x - q.x), (p.y - q.y), (p.z - q.z)};
  }

  friend point cross(const point &p, const point &q) {
    return {(p.y * q.z - p.z * q.y), (q.z * p.x - p.x * q.z),
            (p.x * q.y - p.y * q.x)};
  }
};

template <class ForwardIt> bool is_ccw_sorted(ForwardIt first, ForwardIt last) {
  using point_t = typename std::iterator_traits<ForwardIt>::value_type;
  std::iter_swap(first, std::min_element(first, last));

  auto center = *first++;
  return std::is_sorted(first, last,
                        [&center](const point_t &lhs, const point_t &rhs) {
                          auto det = cross(lhs - center, rhs - center).z;
                          return det > 0;
                        });
}

} // Anonymous namespace

TEST(convex_hull, SortsPointsInCounterclockwiseOrder) {
  using scalar_t = int32_t;
  using point_t = point<scalar_t>;
  using vector_t = std::vector<point_t>;

  auto ccw = [](const point_t &p, const point_t &q, const point_t &r) {
    return cross(q - p, r - p).z > 0;
  };

  vector_t points = {{1, 1}, {1, -1}, {-1, -1}, {-1, 1}};

  std::sort(begin(points), end(points));

  auto hull = djp::convex_hull(begin(points), end(points), ccw);
  EXPECT_TRUE(is_ccw_sorted(begin(hull), end(hull)));

  points.erase(begin(points),
               djp::convex_hull_partition(begin(points), end(points), ccw));

  EXPECT_EQ(hull.size(), points.size());

  for (size_t i = 0; i < points.size(); ++i) {
    EXPECT_EQ(hull[i].x, points[i].x);
    EXPECT_EQ(hull[i].y, points[i].y);
  }

  EXPECT_TRUE(is_ccw_sorted(begin(points), end(points)));
}

TEST(convex_hull, WithCollinearPoints) {
  using scalar_t = int32_t;
  using point_t = point<scalar_t>;
  using vector_t = std::vector<point_t>;

  auto ccw = [](const point_t &p, const point_t &q, const point_t &r) -> bool {
    return cross(q - p, r - p).z >= 0;
  };

  vector_t points = {// Square
                     {0, 0},
                     {0, 5},
                     {5, 5},
                     {5, 0},
                     // Collinear points
                     {0, 2},
                     {2, 0},
                     {2, 5},
                     {5, 2},
                     // Inner points
                     {1, 1},
                     {2, 2},
                     {3, 3},
                     {4, 4},
                     {1, 2},
                     {3, 2}};

  std::sort(begin(points), end(points));

  auto hull = djp::convex_hull(begin(points), end(points), ccw);

  points.erase(begin(points),
               djp::convex_hull_partition(begin(points), end(points), ccw));

  const size_t expected_len = 8;
  EXPECT_EQ(expected_len, hull.size());
  EXPECT_EQ(expected_len, points.size());

  vector_t result = {
      {0, 0}, {2, 0}, {5, 0}, {5, 2}, {5, 5}, {2, 5}, {0, 5}, {0, 2}};

  for (size_t i = 0; i < expected_len; ++i) {
    EXPECT_EQ(result[i].x, points[i].x);
    EXPECT_EQ(result[i].y, points[i].y);
    EXPECT_EQ(result[i].x, hull[i].x);
    EXPECT_EQ(result[i].y, hull[i].y);
  }
}

TEST(convex_hull, WithoutCollinearPoints) {
  using scalar_t = int32_t;
  using point_t = point<scalar_t>;
  using vector_t = std::vector<point_t>;

  auto ccw = [](const point_t &p, const point_t &q, const point_t &r) {
    return cross(q - p, r - p).z > 0;
  };

  vector_t points = {// Square
                     {0, 0},
                     {0, 5},
                     {5, 5},
                     {5, 0},
                     // Collinear points
                     {0, 2},
                     {2, 0},
                     {2, 5},
                     {5, 2},
                     // Inner points
                     {1, 1},
                     {2, 2},
                     {3, 3},
                     {4, 4},
                     {1, 2},
                     {3, 2}};

  std::sort(begin(points), end(points));

  auto hull = djp::convex_hull(begin(points), end(points), ccw);

  points.erase(begin(points),
               djp::convex_hull_partition(begin(points), end(points), ccw));

  const size_t expected_len = 4;
  EXPECT_EQ(expected_len, hull.size());
  EXPECT_EQ(expected_len, points.size());

  vector_t result = {{0, 0}, {5, 0}, {5, 5}, {0, 5}};

  for (size_t i = 0; i < expected_len; ++i) {
    EXPECT_EQ(result[i].x, points[i].x);
    EXPECT_EQ(result[i].y, points[i].y);
    EXPECT_EQ(result[i].x, hull[i].x);
    EXPECT_EQ(result[i].y, hull[i].y);
  }
}
