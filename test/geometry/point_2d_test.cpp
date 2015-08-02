//          Copyright Jorge Aguirre July 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <djp/geometry/point_2d.hpp>
#include <gtest/gtest.h>

#include <iostream>
#include <algorithm>

namespace {
// typedefs
using point_i = djp::point<int32_t>;
using point_d = djp::point<double>;
} // unnamed

TEST(point_2d, SortPointsInCounterClockWiseOrder) {
  std::vector<point_i> points = {{1, 2}, {5, 4}, {2, 1}, {2, 3},
                                 {4, 6}, {1, 5}, {4, 1}, {6, 2},
                                 {5, 5}, {3, 2}, {3, 1}, {4, 4}};
  point_i center = {3, 3};
  counter_clockwise_sort(center, points.begin(), points.end());
  std::vector<point_i> expected = {{1, 5}, {2, 3}, {1, 2}, {2, 1},
                                   {3, 2}, {3, 1}, {4, 1}, {6, 2},
                                   {5, 4}, {4, 4}, {5, 5}, {4, 6}};
  EXPECT_TRUE(std::equal(points.begin(), points.end(), expected.begin()));

  EXPECT_TRUE(is_ccw_sorted(center, points.begin(), points.end()));
}

TEST(point_2d, SortPointsInClockWiseOrder) {
  std::vector<point_i> points = {{1, 2}, {5, 4}, {2, 1}, {2, 3},
                                 {4, 6}, {1, 5}, {4, 1}, {6, 2},
                                 {5, 5}, {3, 2}, {3, 1}, {4, 4}};
  point_i center = {3, 3};
  clockwise_sort(center, points.begin(), points.end());
  std::vector<point_i> expected = {{4, 6}, {4, 4}, {5, 5}, {5, 4},
                                   {6, 2}, {4, 1}, {3, 2}, {3, 1},
                                   {2, 1}, {1, 2}, {2, 3}, {1, 5}};
  EXPECT_TRUE(std::equal(points.begin(), points.end(), expected.begin()));
  EXPECT_TRUE(is_clockwise_sorted(center, points.begin(), points.end()));
}
