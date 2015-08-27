
#include <djp/geometry/segment_intersection.hpp>
#include <gtest/gtest.h>

#include <djp/geometry/point_2d.hpp>

#include <cstdint> // For std::int32_t
#include <vector>  // For std::vector

using namespace djp;
using point_i = point<int32_t>;
using segment_i = segment<point_i>;
using segment_set = std::vector<segment_i>;

static bool operator==(const segment_i &s0, const segment_i &s1) {
  return s0.a == s1.a && s0.b == s1.b;
}

static bool test_simple_polygon(const segment_set &set) {
  return simple_polygon(set.begin(), set.end());
}

static void test_intersection(const segment_set &set, const segment_i &s0,
                              const segment_i &s1) {
  auto intersection = find_intersection(set.begin(), set.end());
  EXPECT_TRUE(*intersection.first == s0);
  EXPECT_TRUE(*intersection.second == s1);
}

static segment_i seg(int32_t x0, int32_t y0, int32_t x1, int32_t y1) {
  return {{x0, y0}, {x1, y1}};
}

TEST(SegmentIntersection, SequentialSegments) {
  // vertical lines
  segment_set test_set = {seg(0, 0, 0, 1),    seg(0, 1, 0, 2),
                          seg(1, 0, 1, 1),    seg(2, 2, 3, 3),
                          seg(-1, -1, 1, -1), seg(-1, 0, -1, 3),
                          seg(-2, 0, -2, 1),  seg(-3, 2, -2, 2)};
  EXPECT_TRUE(test_simple_polygon(test_set));
  test_intersection(test_set, test_set[1], test_set[0]);
  // horizontal line
  test_set = {seg(0, 0, 1, 0),   seg(1, 0, 2, 0),  seg(0, 1, 1, 1),
              seg(1, -1, 2, -1), seg(3, -1, 3, 2), seg(-2, -1, -2, 2)};
  EXPECT_TRUE(test_simple_polygon(test_set));
  test_intersection(test_set, test_set[1], test_set[0]);
  // diagonal line
  test_set = {seg(0, 0, 1, 1), seg(1, 1, 2, 2), seg(0, 1, 1, 2),
              seg(1, -1, 2, 0), seg(3, -3, 3, 4)};
  EXPECT_TRUE(test_simple_polygon(test_set));
  test_intersection(test_set, test_set[1], test_set[0]);
  // perpendicular lines
  test_set = {seg(0, 0, 0, 1), seg(0, 1, 1, 1), seg(-1, -1, 2, -1),
              seg(-1, 1, -1, 0)};
  EXPECT_TRUE(test_simple_polygon(test_set));
  test_intersection(test_set, test_set[1], test_set[0]);
}

TEST(SegmentIntersection, OverlapedSegments) {
  // vertical lines
  segment_set test_set = {seg(0, 0, 0, 2),    seg(0, 1, 0, 3),
                          seg(1, 0, 1, 1),    seg(2, 2, 3, 3),
                          seg(-1, -1, 1, -1), seg(-1, 0, -1, 3),
                          seg(-2, 0, -2, 1),  seg(-3, 2, -2, 2)};
  EXPECT_FALSE(test_simple_polygon(test_set));
  test_intersection(test_set, test_set[1], test_set[0]);
  // horizontal line
  test_set = {seg(0, 0, 2, 0),   seg(1, 0, 5, 0),  seg(0, 1, 1, 1),
              seg(1, -1, 2, -1), seg(3, -1, 3, 2), seg(-2, -1, -2, 2)};
  test_intersection(test_set, test_set[1], test_set[0]);
  // diagonal line
  test_set = {seg(0, 0, 2, 2), seg(1, 1, 3, 3), seg(0, 1, 1, 2),
              seg(1, -1, 2, 0), seg(3, -3, 3, 4)};
  EXPECT_FALSE(test_simple_polygon(test_set));
  test_intersection(test_set, test_set[1], test_set[0]);
  // perpendicular lines
  test_set = {seg(0, 0, 0, 2), seg(0, 1, 1, 1), seg(-1, -1, 2, -1),
              seg(-1, 1, -1, 0)};
  EXPECT_FALSE(test_simple_polygon(test_set));
  test_intersection(test_set, test_set[1], test_set[0]);
}

TEST(SegmentIntersection, PolygonIntersection) {
  // a square
  segment_set test_set = {seg(0, 0, 0, 2), seg(0, 2, 2, 2), seg(2, 2, 2, 0),
                          seg(2, 0, 0, 0)};
  EXPECT_TRUE(test_simple_polygon(test_set));

  // two overlaping squares
  test_set = {seg(0, 0, 0, 2),  seg(0, 2, 2, 2),  seg(2, 2, 2, 0),
              seg(2, 0, 0, 0),  seg(0, -1, 0, 1), seg(0, 1, 1, 1),
              seg(1, 1, 1, -1), seg(1, -1, 0, -1)};
  EXPECT_FALSE(test_simple_polygon(test_set));
}
