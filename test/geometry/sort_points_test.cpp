//          Copyright Jorge Aguirre July 2015, August 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <djp/geometry/sort_points.hpp>
#include <gtest/gtest.h>

#include <djp/geometry/point_2d.hpp>

#include <vector>  // For std::vector
#include <cstdint> // For std::int32_t

using namespace djp;

using point_i = point<std::int32_t>;

TEST(SortPointsTest, SortPointsInCounterClockWiseOrder) {
  std::vector<point_i> points = {{1, 2},
                                 {5, 4},
                                 {2, 1},
                                 {2, 3},
                                 {4, 6},
                                 {1, 5},
                                 {4, 1},
                                 {6, 2},
                                 {5, 5},
                                 {3, 2},
                                 {3, 1},
                                 {4, 4}};
  point_i center = {3, 3};
  point_i start = {3, 4};
  counter_clockwise_sort(center, start, points.begin(), points.end());
  std::vector<point_i> expected = {{1, 5},
                                   {2, 3},
                                   {1, 2},
                                   {2, 1},
                                   {3, 2},
                                   {3, 1},
                                   {4, 1},
                                   {6, 2},
                                   {5, 4},
                                   {4, 4},
                                   {5, 5},
                                   {4, 6}};
  EXPECT_EQ(expected, points);
  EXPECT_TRUE(is_ccw_sorted(center, points.begin(), points.end()));
}

TEST(SortPointsTest, SortPointsInClockWiseOrder) {
  std::vector<point_i> points = {{1, 2},
                                 {5, 4},
                                 {2, 1},
                                 {2, 3},
                                 {4, 6},
                                 {1, 5},
                                 {4, 1},
                                 {6, 2},
                                 {5, 5},
                                 {3, 2},
                                 {3, 1},
                                 {4, 4}};
  std::vector<point_i> expected = {{4, 6},
                                   {4, 4},
                                   {5, 5},
                                   {5, 4},
                                   {6, 2},
                                   {4, 1},
                                   {3, 2},
                                   {3, 1},
                                   {2, 1},
                                   {1, 2},
                                   {2, 3},
                                   {1, 5}};
  point_i center = {3, 3};
  point_i start = {3, 4};

  clockwise_sort(center, start, points.begin(), points.end());

  EXPECT_EQ(expected, points);
  EXPECT_TRUE(is_clockwise_sorted(center, points.begin(), points.end()));
}
