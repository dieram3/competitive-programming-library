//          Copyright Jorge Aguirre July 2015, August 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <djp/geometry/point_order.hpp>
#include <gtest/gtest.h>

#include <djp/geometry/point_2d.hpp>

#include <vector>  // For std::vector
#include <cstdint> // For std::int32_t

using namespace djp;
using point_i = point<std::int32_t>;

namespace {

const point_i origin = {0, 0};
const point_i p0_00 = {0, 1};
const point_i p1_30 = {1, 1};
const point_i p3_00 = {1, 0};
const point_i p4_30 = {1, -1};
const point_i p6_00 = {0, -1};
const point_i p7_30 = {-1, -1};
const point_i p9_00 = {-1, 0};
const point_i p10_30 = {-1, 1};

} // end anonymous namespace

static bool cw_sorted(const point_i &center,
                      const std::vector<point_i> &points) {
  const bool result = is_clockwise_sorted(center, points.begin(), points.end());
  return result;
}

// This version assumes that origin is the center.
static bool cw_sorted(std::vector<point_i> points) {
  const point_i center{3, 2}; // Random center.

  for (auto &p : points)
    p = p + center; // Translate points.

  return cw_sorted(center, points);
}

TEST(PointOrderTest, SortPointsInCounterClockWiseOrder) {
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
                                   {5, 5},
                                   {4, 4},
                                   {4, 6}};
  EXPECT_EQ(expected, points);
  EXPECT_TRUE(
      is_counter_clockwise_sorted(center, points.begin(), points.end()));
}

TEST(PointOrderTest, SortPointsInClockWiseOrder) {
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
                                   {5, 5},
                                   {4, 4},
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

TEST(IsClockwiseSortedTest, EmptyRangeTest) { EXPECT_TRUE(cw_sorted({})); }

TEST(IsClockwiseSortedTest, SinglePointTest) {
  EXPECT_TRUE(cw_sorted({origin}));
  EXPECT_TRUE(cw_sorted({p3_00}));
  EXPECT_TRUE(cw_sorted({p10_30}));
}

TEST(IsClockwiseSortedTest, MoreThan360DegreesTest) {
 // Giving small hops
 EXPECT_FALSE(cw_sorted({p0_00, p3_00, p7_30, p0_00}));
 EXPECT_FALSE(cw_sorted({p1_30, p4_30, p7_30, p10_30, p0_00, p3_00, p4_30}));

 // Giving large hops
 EXPECT_FALSE(cw_sorted({p6_00, p4_30, p6_00}));
 EXPECT_FALSE(cw_sorted({p7_30, p3_00, p6_00, p4_30}));

 // Giving 180 degrees hops
 EXPECT_FALSE(cw_sorted({p0_00, p6_00, p0_00}));
 EXPECT_FALSE(cw_sorted({p9_00, p3_00, p9_00}));
}

TEST(IsClockwiseSortedTest, TwoPointsTest) {
 // When rotates x degrees and x is in (0, 180)
 EXPECT_TRUE(cw_sorted({p9_00, p10_30}));
 EXPECT_TRUE(cw_sorted({p9_00, p0_00}));
 EXPECT_TRUE(cw_sorted({p9_00, p1_30}));

 // When rotates x degrees and x is in (180, 360)
 EXPECT_TRUE(cw_sorted({p3_00, p10_30}));
 EXPECT_TRUE(cw_sorted({p3_00, p0_00}));
 EXPECT_TRUE(cw_sorted({p3_00, p1_30}));

 // When rotates exactly 180 degrees.
 EXPECT_TRUE(cw_sorted({p0_00, p6_00}));
 EXPECT_TRUE(cw_sorted({p6_00, p0_00}));
 EXPECT_TRUE(cw_sorted({p4_30, p10_30}));
 EXPECT_TRUE(cw_sorted({p10_30, p4_30}));

 // When rotates 0 degrees
 EXPECT_TRUE(cw_sorted({p4_30, p4_30}));
 EXPECT_TRUE(cw_sorted({p6_00, 2 * p6_00}));
 EXPECT_TRUE(cw_sorted({2 * p6_00, p6_00}));
}

TEST(IsClockwiseSortedTest, IsUnsortedTrivialTest) {
  EXPECT_FALSE(cw_sorted({p0_00, p3_00, p1_30}));
  EXPECT_FALSE(cw_sorted({p0_00, p9_00, p10_30, p7_30}));
  EXPECT_FALSE(cw_sorted({p0_00, p6_00, p3_00}));
}

TEST(IsClockwiseSortedTest, IsSortedTrivialTest) {

 // When rotates 180 degrees.
 EXPECT_TRUE(cw_sorted({p3_00, p6_00, p0_00}));
 EXPECT_TRUE(cw_sorted({p3_00, 2 * p6_00, p0_00}));
 EXPECT_TRUE(cw_sorted({p3_00, p6_00, 2 * p0_00}));

 // When rotates 0 degrees.
 EXPECT_TRUE(cw_sorted({p6_00, p7_30, p7_30}));
 EXPECT_TRUE(cw_sorted({p6_00, 2 * p7_30, p7_30}));
 EXPECT_TRUE(cw_sorted({p6_00, p7_30, 2 * p7_30}));

 // When rotates arbitrary degrees.
 EXPECT_TRUE(cw_sorted({p0_00, p1_30, p4_30, p9_00}));
 EXPECT_TRUE(cw_sorted({p9_00, p3_00, p6_00, p7_30}));
 EXPECT_TRUE(cw_sorted({p9_00, p6_00, p7_30}));
}
