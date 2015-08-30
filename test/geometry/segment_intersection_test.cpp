//          Copyright Jorge Aguirre, Diego Ramirez August 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <djp/geometry/segment_intersection.hpp>
#include <gtest/gtest.h>

#include <djp/geometry/point_2d.hpp>

#include <algorithm> // For std::count
#include <utility>   // For std::make_pair
#include <vector>    // For std::vector

using namespace djp;

// ==========================================
// FindIntersectionTest
// ==========================================

namespace {
class FindIntersectionTest : public ::testing::Test {
protected:
  using scalar_t = int;
  using point_t = point<scalar_t>;
  using segment_t = segment<point_t>;

  std::vector<segment_t> set;

protected:
  void add(scalar_t p0, scalar_t p1, scalar_t q0, scalar_t q1) {
    set.emplace_back(point_t(p0, p1), point_t(q0, q1));
  }

  bool finds(const size_t s0, const size_t s1) {
    const auto it0 = set.begin() + s0;
    const auto it1 = set.begin() + s1;
    const auto pair = find_intersection(set.begin(), set.end());
    return (pair == std::make_pair(it0, it1)) ||
           (pair == std::make_pair(it1, it0));
  }

  bool finds_nothing() {
    const auto pair = find_intersection(set.begin(), set.end());
    return pair.first == set.end() && pair.second == set.end();
  }
};

} // End anonymous namespace

TEST_F(FindIntersectionTest, CommonCrossingTest) {
  add(2, 0, 4, 0), add(5, 0, 4, 3), add(4, 4, 0, 4);
  add(3, 1, 2, 3), add(1, 0, 4, 3), add(0, 1, 1, 3);
  ASSERT_EQ(6, set.size());
  EXPECT_TRUE(finds(3, 4));

  set.clear();
  add(-2, -1, 0, 1), add(-1, -1, 1, 1), add(0, -1, 2, 1), add(1, -1, 3, 1);
  add(0, -2, 2, 2);
  ASSERT_EQ(5, set.size());
  EXPECT_TRUE(finds(2, 4));

  // Several crosses
  set.clear();
  add(-2, -1, 0, 1), add(-1, -1, 1, 1), add(0, -1, 2, 1), add(-2, 0, 2, 0);
  ASSERT_EQ(4, set.size());
  EXPECT_TRUE(finds(0, 3));
}

TEST_F(FindIntersectionTest, NoIntersectionTest) {
  // Empty
  ASSERT_EQ(0, set.size());
  EXPECT_TRUE(finds_nothing());

  // Unique segment
  set.clear();
  add(-50, -50, 10, 40);
  ASSERT_EQ(1, set.size());
  EXPECT_TRUE(finds_nothing());

  // Small test
  set.clear();
  add(0, 0, 1, 2), add(1, -2, 1, -1), add(0, -1, 2, 0);
  add(1, 3, -1, -1), add(-1, 0, 0, 2);
  ASSERT_EQ(5, set.size());
  EXPECT_TRUE(finds_nothing());

  // Big test
  set.clear();
  add(0, 2, 0, 3), add(1, -2, 3, 1), add(1, -3, -1, -1), add(0, 0, 0, -1);
  add(3, 2, 2, 5), add(2, -3, 4, 2), add(0, 1, 1, -1), add(-1, 0, -2, -2);
  add(-2, 0, -1, 3), add(2, 2, 1, 6), add(1, 2, 1, 3), add(1, 4, 1, 5);
  add(-2, 2, -1, 5), add(0, 4, 0, 5);
  ASSERT_EQ(14, set.size());
  EXPECT_TRUE(finds_nothing());
}

TEST_F(FindIntersectionTest, SequentialSegmentsTest) {
  // vertical lines
  add(0, 0, 0, 1), add(0, 1, 0, 2), add(1, 0, 1, 1), add(2, 2, 3, 3);
  add(-1, -1, 1, -1), add(-1, 0, -1, 3), add(-2, 0, -2, 1), add(-3, 2, -2, 2);
  ASSERT_EQ(8, set.size());
  EXPECT_TRUE(finds(1, 0));

  // horizontal line
  set.clear();
  add(0, 0, 1, 0), add(1, 0, 2, 0), add(0, 1, 1, 1);
  add(1, -1, 2, -1), add(3, -1, 3, 2), add(-2, -1, -2, 2);
  ASSERT_EQ(6, set.size());
  EXPECT_TRUE(finds(1, 0));

  // diagonal line
  set.clear();
  add(0, 0, 1, 1), add(1, 1, 2, 2), add(0, 1, 1, 2);
  add(1, -1, 2, 0), add(3, -3, 3, 4);
  ASSERT_EQ(5, set.size());
  EXPECT_TRUE(finds(1, 0));

  // perpendicular lines
  set.clear();
  add(0, 0, 0, 1), add(0, 1, 1, 1), add(-1, -1, 2, -1), add(-1, 1, -1, 0);
  ASSERT_EQ(4, set.size());
  EXPECT_TRUE(finds(1, 0));
}

TEST_F(FindIntersectionTest, OverlappedSegments) {
  // vertical lines
  add(0, 0, 0, 2), add(0, 1, 0, 3), add(1, 0, 1, 1), add(2, 2, 3, 3);
  add(-1, -1, 1, -1), add(-1, 0, -1, 3), add(-2, 0, -2, 1), add(-3, 2, -2, 2);
  ASSERT_EQ(8, set.size());
  EXPECT_TRUE(finds(1, 0));

  // horizontal line
  set.clear();
  add(0, 0, 2, 0), add(1, 0, 5, 0), add(0, 1, 1, 1);
  add(1, -1, 2, -1), add(3, -1, 3, 2), add(-2, -1, -2, 2);
  ASSERT_EQ(6, set.size());
  EXPECT_TRUE(finds(1, 0));

  // diagonal line
  set.clear();
  add(0, 0, 2, 2), add(1, 1, 3, 3), add(0, 1, 1, 2);
  add(1, -1, 2, 0), add(3, -3, 3, 4);
  ASSERT_EQ(5, set.size());
  EXPECT_TRUE(finds(1, 0));

  // perpendicular lines
  set.clear();
  add(0, 0, 0, 2), add(0, 1, 1, 1), add(-1, -1, 2, -1), add(-1, 1, -1, 0);
  ASSERT_EQ(4, set.size());
  EXPECT_TRUE(finds(1, 0));
}

TEST_F(FindIntersectionTest, TangentSegmentTest) {
  // Perpendicular case
  add(0, 0, 0, 3), add(1, 0, 2, 2), add(0, 2, 1, 2);
  add(-1, -1, -1, 1), add(-2, 1, -1, 2), add(-2, 2, -1, 3);
  ASSERT_EQ(6, set.size());
  EXPECT_TRUE(finds(2, 0));

  // Another perpendicular case
  set.clear();
  add(-1, -1, 1, 1), add(1, -2, 2, 1), add(-3, 0, -2, 1), add(-1, 1, -2, 2);
  add(0, 0, 1, -1), add(-1, -2, -2, 0);
  ASSERT_EQ(6, set.size());
  EXPECT_TRUE(finds(4, 0));

  set.clear();
  add(0, 2, 1, 0), add(0, 4, 1, 3), add(2, 1, 3, 4);
  add(1, 1, 2, 4), add(1, 2, 3, 6);
  ASSERT_EQ(5, set.size());
  EXPECT_TRUE(finds(4, 3));
}

TEST_F(FindIntersectionTest, CollinearSegmentsWithSameStartTest) {
  add(0, 2, 1, 2), add(-2, 0, -1, 2), add(-1, 1, 1, 1);
  add(0, 0, 1, -1), add(3, -1, 3, 1);
  EXPECT_TRUE(finds_nothing());
  add(-1, 1, 1, 1);
  EXPECT_TRUE(finds(2, 5));

  set.clear();
  add(1, 1, 0, 2), add(2, 1, 4, 4), add(1, 2, 2, 3), add(1, 3, 2, 4);
  EXPECT_TRUE(finds_nothing());
  add(1, 2, 3, 4);
  EXPECT_TRUE(finds(2, 4));
}

// ==========================================
// SimplePolygonTest
// ==========================================

namespace {
class SimplePolygonTest : public ::testing::Test {

protected:
  using scalar_t = int;
  using point_t = point<scalar_t>;
  using segment_t = segment<point_t>;

  std::vector<point_t> points;

  // Adds a point to the end of the list.
  void add(scalar_t x, scalar_t y) { points.emplace_back(x, y); }

  // Adds a point to the end of the list relative to the last point.
  void rel_add(scalar_t x_delta, scalar_t y_delta) {
    const auto prev_x = points.back().x;
    const auto prev_y = points.back().y;
    add(prev_x + x_delta, prev_y + y_delta);
  }

  bool is_simple() {
    assert(points.size() >= 3 && "Invalid polygon");
    std::vector<segment_t> segments;
    segments.reserve(points.size());
    for (size_t i = 0; i + 1 != points.size(); ++i)
      segments.emplace_back(points[i], points[i + 1]);
    segments.emplace_back(points.back(), points.front());
    assert(segments.size() == points.size());
    return simple_polygon(segments.begin(), segments.end());
  }
};
} // End anonymous namespace

TEST_F(SimplePolygonTest, SquareTest) {
  add(0, 0);
  add(0, 2);
  add(2, 2);
  add(2, 0);
  EXPECT_TRUE(is_simple());
}

TEST_F(SimplePolygonTest, CrossingSquaresTest) {
  add(0, 1), add(1, 1), add(1, 0);
  EXPECT_TRUE(is_simple());
  add(0, 0);
  EXPECT_TRUE(is_simple());
  points.pop_back();
  add(-1, 0);
  EXPECT_TRUE(is_simple());
  add(-1, -1);
  EXPECT_FALSE(is_simple());
  add(0, -1);
  EXPECT_FALSE(is_simple());
  points.pop_back();
  add(1, -1);
  EXPECT_FALSE(is_simple());
  points.pop_back();
  add(2, -1);
  EXPECT_FALSE(is_simple());
  add(2, 0);
  EXPECT_FALSE(is_simple());
  points.pop_back();
  add(2, 2);
  EXPECT_TRUE(is_simple());
  add(-1, 2);
  EXPECT_TRUE(is_simple());
}

TEST_F(SimplePolygonTest, OneIntersectionTest) {
  add(0, 0);
  rel_add(3, -2), rel_add(1, 3), rel_add(-2, 1);
  rel_add(2, -5), rel_add(-2, -1), rel_add(1, 1);
  ASSERT_EQ(7, points.size());
  ASSERT_EQ(point_t(3, -3), points.back());
  EXPECT_FALSE(is_simple());

  points.clear();
  add(0, 0);
  rel_add(1, 3), rel_add(3, 0), rel_add(1, -3);
  rel_add(-2, -3), rel_add(-2, 0), rel_add(0, 5);
  EXPECT_TRUE(is_simple());
  rel_add(3, 0);
  rel_add(0, -2);
  EXPECT_FALSE(is_simple());
  rel_add(-2, 0);
  rel_add(0, -1);
  EXPECT_FALSE(is_simple());

  EXPECT_EQ(point_t(2, -1), points.back());
  EXPECT_EQ(11, points.size());
}

TEST_F(SimplePolygonTest, MultipleIntersecionsTest) {
  add(0, 0);
  rel_add(0, 3), rel_add(2, -2), rel_add(0, 2), rel_add(1, -2);
  rel_add(2, 0), rel_add(-2, -2), rel_add(2, 1), rel_add(-2, -2);
  EXPECT_TRUE(is_simple());
  rel_add(-2, 5);
  EXPECT_FALSE(is_simple());
  rel_add(-2, -2);
  rel_add(2, -1);
  EXPECT_FALSE(is_simple());

  EXPECT_EQ(point_t(1, 0), points.back());
  EXPECT_EQ(12, points.size());

  points.clear();
  add(0, 0);
  rel_add(1, 1), rel_add(1, -1), rel_add(-2, -2), rel_add(2, -2);
  rel_add(-1, -1), rel_add(-1, 1), rel_add(2, 2);
  EXPECT_EQ(point_t(2, -2), points.back());
  EXPECT_EQ(8, points.size());
  EXPECT_FALSE(is_simple());
}

TEST_F(SimplePolygonTest, NoIntersectionTest) {
  add(0, 0);
  rel_add(0, 3), rel_add(1, -2), rel_add(1, 1), rel_add(0, -3);
  rel_add(-1, 1), rel_add(0, -1), rel_add(1, -1), rel_add(1, 1);
  rel_add(0, 3), rel_add(-1, 1), rel_add(-1, -1), rel_add(0, 1);
  rel_add(1, 1), rel_add(2, -2), rel_add(0, -3), rel_add(-2, -2);
  rel_add(-3, 3), rel_add(0, 3);

  ASSERT_EQ(19, points.size());
  ASSERT_EQ(point_t(-1, 3), points.back());
  EXPECT_TRUE(is_simple());

  points.clear();
  add(0, 0);
  rel_add(0, 3), rel_add(1, -2), rel_add(-1, -2), rel_add(-1, 5);
  ASSERT_EQ(5, points.size());
  ASSERT_EQ(point_t(-1, 4), points.back());
  EXPECT_TRUE(is_simple());
}

TEST_F(SimplePolygonTest, TangentPointTest) {
  add(0, 0);
  rel_add(2, 2), rel_add(0, -3), rel_add(2, 2), rel_add(-4, 2);
  ASSERT_EQ(point_t(0, 3), points.back());
  ASSERT_EQ(5, points.size());
  EXPECT_FALSE(is_simple());

  points.clear();
  add(0, 0);
  rel_add(2, 2), rel_add(0, -3), rel_add(2, 3), rel_add(0, -4);
  rel_add(2, 0), rel_add(0, -1);
  ASSERT_EQ(7, points.size());
  ASSERT_EQ(point_t(6, -3), points.back());
  EXPECT_FALSE(is_simple());
}

TEST_F(SimplePolygonTest, CollinearChainedTest) {
  add(0, 0);
  rel_add(0, 1), rel_add(0, 1), rel_add(1, 1);
  rel_add(0, -1), rel_add(0, -1), rel_add(0, -1), rel_add(0, -1);
  rel_add(-1, 0), rel_add(-1, 0);
  rel_add(0, 1), rel_add(0, 1), rel_add(0, 1);
  ASSERT_EQ(13, points.size());
  ASSERT_EQ(point_t(-1, 2), points.back());
  EXPECT_TRUE(is_simple());

  points.clear();
  add(0, 0);
  rel_add(1, 1), rel_add(1, 1), rel_add(1, 1), rel_add(1, 1), rel_add(1, 1);
  rel_add(-4, -5);
  ASSERT_EQ(7, points.size());
  ASSERT_EQ(point_t(1, 0), points.back());
  EXPECT_TRUE(is_simple());
}

TEST_F(SimplePolygonTest, OverlappedSegments) {
  add(0, 0);
  rel_add(1, 3), rel_add(2, 0), rel_add(1, -3), rel_add(1, 3);
  ASSERT_EQ(5, points.size());
  ASSERT_EQ(point_t(5, 3), points.back());
  EXPECT_FALSE(is_simple());

  points.clear();
  add(0, 0);
  rel_add(0, 2), rel_add(1, 0), rel_add(0, -2);
  rel_add(1, 0), rel_add(0, 3), rel_add(-2, 0);
  ASSERT_EQ(7, points.size());
  ASSERT_EQ(point_t(0, 3), points.back());
  EXPECT_FALSE(is_simple());
}

TEST_F(SimplePolygonTest, EightTest) {
  add(0, 0);
  rel_add(1, 1), rel_add(1, -1), rel_add(-1, -1), rel_add(-1, -1);
  rel_add(1, -1), rel_add(1, 1);
  rel_add(-1, 1);
  ASSERT_EQ(8, points.size());
  ASSERT_EQ(point_t(1, -1), points.back());
  EXPECT_TRUE(is_simple());
}

TEST_F(SimplePolygonTest, RareSimplePolygonTest) {
  add(0, 0);
  rel_add(3, -4), rel_add(2, 7), rel_add(5, 0), rel_add(-3, -3);
  rel_add(-3, -4), rel_add(3, -2), rel_add(3, -2), rel_add(1, 0);
  rel_add(2, -2), rel_add(-3, 2), rel_add(-1, 0), rel_add(-5, -2);
  rel_add(0, 4), rel_add(-1, -1);
  ASSERT_EQ(15, points.size());
  ASSERT_EQ(point_t(3, -7), points.back());
  ASSERT_EQ(2, std::count(points.begin(), points.end(), point_t(10, -8)));
  EXPECT_TRUE(is_simple());
}
