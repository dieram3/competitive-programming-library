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

// Sorts in clockwise order. If two points have same angle, the nearest one to
// the center comes first.
template <class RandomIt>
static void cw_sort(const point_i &center, const point_i &start, RandomIt first,
                    RandomIt last) {
  auto less = [=](const point_i &lhs, const point_i &rhs) {
    if (cw_less(center, start, lhs, rhs))
      return true;
    if (cw_less(center, start, rhs, lhs))
      return false;
    return distance2(lhs, center) < distance2(rhs, center);
  };

  std::sort(first, last, less);
}

static bool cw_sorted(const point_i &center,
                      const std::vector<point_i> &points) {

  if (points.empty())
    return true;

  const point_i start = points.front() - center;
  const point_i rev_start = points.back() - center;

  auto cw_less = [=](const point_i &lhs, const point_i &rhs) {
    return djp::cw_less(center, start, lhs, rhs);
  };
  auto ccw_less = [=](const point_i &lhs, const point_i &rhs) {
    return djp::ccw_less(center, rev_start, lhs, rhs);
  };

  const bool result = std::is_sorted(points.begin(), points.end(), cw_less);
  // Points in same angle should be considered equal i.e no one is less than
  // other.
  const bool rev_result =
      std::is_sorted(points.rbegin(), points.rend(), ccw_less);

  EXPECT_EQ(result, rev_result);

  return result;
}

// This version assumes that origin is the center.
static bool cw_sorted(std::vector<point_i> points) {
  const point_i center{3, 2}; // Random center.

  for (auto &p : points)
    p = p + center; // Translate points.

  return cw_sorted(center, points);
}

TEST(PointOrderTest, SortPointsInClockWiseOrder) {
  std::vector<point_i> points = {{1, 2},
                                 {5, 5},
                                 {6, 2},
                                 {3, -2},
                                 {3, 1},
                                 {5, 5},
                                 {2, 2},
                                 {1, 5},
                                 {2, 3},
                                 {4, 4},
                                 {6, 2},
                                 {6, 2},
                                 {4, 6},
                                 {3, 2},
                                 {1, 1},
                                 {3, 0},
                                 {3, -1},
                                 {6, 6},
                                 {4, 1},
                                 {5, 4},
                                 {2, 1}};

  std::vector<point_i> expected = {{4, 6},
                                   {4, 4},
                                   {5, 5},
                                   {5, 5},
                                   {6, 6},
                                   {5, 4},
                                   {6, 2},
                                   {6, 2},
                                   {6, 2},
                                   {4, 1},
                                   {3, 2},
                                   {3, 1},
                                   {3, 0},
                                   {3, -1},
                                   {3, -2},
                                   {2, 1},
                                   {2, 2},
                                   {1, 1},
                                   {1, 2},
                                   {2, 3},
                                   {1, 5}};
  point_i center = {3, 3};
  point_i start = {0, 1};

  cw_sort(center, start, points.begin(), points.end());

  EXPECT_EQ(expected, points);
  EXPECT_TRUE(cw_sorted(center, points));
}

TEST(PointOrderTest, MoreThan360DegreesTest) {
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

TEST(PointOrderTest, TwoPointsTest) {
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

TEST(PointOrderTest, IsUnsortedTrivialTest) {
  EXPECT_FALSE(cw_sorted({p0_00, p3_00, p1_30}));
  EXPECT_FALSE(cw_sorted({p0_00, p9_00, p10_30, p7_30}));
  EXPECT_FALSE(cw_sorted({p0_00, p6_00, p3_00}));
}

TEST(PointOrderTest, IsSortedTrivialTest) {

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
