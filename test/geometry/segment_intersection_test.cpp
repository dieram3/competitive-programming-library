//          Copyright Jorge Aguirre, Diego Ramirez 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cpl/geometry/segment_intersection.hpp>
#include <gtest/gtest.h>

#include <cpl/geometry/point_2d.hpp> // point
#include <algorithm>                 // count
#include <cassert>                   // assert
#include <initializer_list>          // initializer_list
#include <utility>                   // make_pair
#include <vector>                    // vector

using cpl::segment_intersect;
using cpl::segment;
using cpl::find_intersection;
using cpl::simple_polygon;
using cpl::point;

// ==========================================
// SegmentIntersectTest
// ==========================================

namespace {
class SegmentIntersectTest : public ::testing::Test {
protected:
  const point<int> a{0, 1};
  const point<int> b{2, 0};
  const point<int> c{3, 2};
  const point<int> d{1, 3};
};
} // end anonymous namespace

TEST_F(SegmentIntersectTest, BoundingBoxTest) {
  // x case
  EXPECT_FALSE(segment_intersect(a, d, b, c));
  EXPECT_FALSE(segment_intersect(b, c, a, d));

  // y case
  EXPECT_FALSE(segment_intersect(a, b, c, d));
  EXPECT_FALSE(segment_intersect(c, d, a, b));
}

TEST_F(SegmentIntersectTest, BasicTest) {
  EXPECT_TRUE(segment_intersect(a, c, d, b));
  EXPECT_TRUE(segment_intersect(c, a, d, b));
  EXPECT_TRUE(segment_intersect(b, d, a, c));
  EXPECT_TRUE(segment_intersect(b, d, c, a));
}

TEST_F(SegmentIntersectTest, TouchingSegmentsTest) {
  EXPECT_TRUE(segment_intersect(a, d, d, c));
  EXPECT_TRUE(segment_intersect(a, b, c, b));
  EXPECT_TRUE(segment_intersect(d, b, d, c));
}

TEST_F(SegmentIntersectTest, ParallelSegmentsTest) {
  const point<int> e{2, 5}, f{3, 7};
  EXPECT_TRUE(segment_intersect(a, d, a, e));
  EXPECT_TRUE(segment_intersect(d, a, e, a));
  EXPECT_TRUE(segment_intersect(a, d, d, e));
  EXPECT_TRUE(segment_intersect(a, e, f, d));
  EXPECT_TRUE(segment_intersect(e, a, d, f));

  EXPECT_FALSE(segment_intersect(a, d, e, f));
  EXPECT_FALSE(segment_intersect(d, a, f, e));
}

TEST_F(SegmentIntersectTest, SegmentInterfaceTest) {
  using segment_t = segment<point<int>>;
  const segment_t ab(a, b);
  const segment_t cd(c, d);
  const segment_t ac(a, c);
  const segment_t bd(b, d);

  EXPECT_TRUE(segment_intersect(ac, bd));
  EXPECT_TRUE(segment_intersect(bd, ac));

  EXPECT_FALSE(segment_intersect(ab, cd));
  EXPECT_FALSE(segment_intersect(cd, ab));
}

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
  static segment_t seg(point_t p, point_t q) {
    return segment_t{p, q};
  }

  void add(const segment_t& segment) {
    set.emplace_back(segment);
  }

  void add(std::initializer_list<segment_t> segments) {
    for (const auto& elem : segments) {
      add(elem);
    }
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
  add({
      seg({2, 0}, {4, 0}), seg({5, 0}, {4, 3}), seg({4, 4}, {0, 4}),
      seg({3, 1}, {2, 3}), seg({1, 0}, {4, 3}), seg({0, 1}, {1, 3}),
  });
  ASSERT_EQ(6, set.size());
  EXPECT_TRUE(finds(3, 4));

  set.clear();
  add({
      seg({-2, -1}, {0, 1}), seg({-1, -1}, {1, 1}), seg({0, -1}, {2, 1}),
      seg({1, -1}, {3, 1}), seg({0, -2}, {2, 2}),
  });
  ASSERT_EQ(5, set.size());
  EXPECT_TRUE(finds(2, 4));

  // One intersection.
  set.clear();
  add({
      seg({1, 1}, {2, 0}), seg({1, 4}, {3, 4}), seg({3, 3}, {4, 5}),
      seg({2, 5}, {5, 4}),
  });
  ASSERT_EQ(4, set.size());
  EXPECT_TRUE(finds(2, 3));

  // Two intersections.
  set.clear();
  add({
      seg({0, 0}, {2, 3}), seg({0, 1}, {1, 2}), seg({0, 5}, {3, 4}),
      seg({1, 0}, {3, 0}), seg({0, 3}, {4, 0}), seg({2, 4}, {3, 5}),
  });
  ASSERT_EQ(6, set.size());
  EXPECT_TRUE(finds(0, 4));

  // Three intersections.
  set.clear();
  add({
      seg({-2, -1}, {0, 1}), seg({-1, -1}, {1, 1}), seg({0, -1}, {2, 1}),
      seg({-2, 0}, {2, 0}),
  });
  ASSERT_EQ(4, set.size());
  EXPECT_TRUE(finds(0, 3));
}

TEST_F(FindIntersectionTest, NoIntersectionTest) {
  // Empty
  ASSERT_EQ(0, set.size());
  EXPECT_TRUE(finds_nothing());

  // Unique segment
  set.clear();
  add({{-50, -50}, {10, 40}});
  ASSERT_EQ(1, set.size());
  EXPECT_TRUE(finds_nothing());

  // Small test
  set.clear();
  add({
      seg({0, 0}, {1, 2}), seg({1, -2}, {1, -1}), seg({0, -1}, {2, 0}),
      seg({1, 3}, {-1, -1}), seg({-1, 0}, {0, 2}),
  });
  ASSERT_EQ(5, set.size());
  EXPECT_TRUE(finds_nothing());

  // Big test
  set.clear();
  add({
      seg({0, 2}, {0, 3}), seg({1, -2}, {3, 1}), seg({1, -3}, {-1, -1}),
      seg({0, 0}, {0, -1}), seg({3, 2}, {2, 5}), seg({2, -3}, {4, 2}),
      seg({0, 1}, {1, -1}), seg({-1, 0}, {-2, -2}), seg({-2, 0}, {-1, 3}),
      seg({2, 2}, {1, 6}), seg({1, 2}, {1, 3}), seg({1, 4}, {1, 5}),
      seg({-2, 2}, {-1, 5}), seg({0, 4}, {0, 5}),
  });
  ASSERT_EQ(14, set.size());
  EXPECT_TRUE(finds_nothing());
}

TEST_F(FindIntersectionTest, SequentialSegmentsTest) {
  // vertical lines
  add({
      seg({0, 0}, {0, 1}), seg({0, 1}, {0, 2}), seg({1, 0}, {1, 1}),
      seg({2, 2}, {3, 3}), seg({-1, -1}, {1, -1}), seg({-1, 0}, {-1, 3}),
      seg({-2, 0}, {-2, 1}), seg({-3, 2}, {-2, 2}),
  });
  ASSERT_EQ(8, set.size());
  EXPECT_TRUE(finds(1, 0));

  // horizontal line
  set.clear();
  add({
      seg({0, 0}, {1, 0}), seg({1, 0}, {2, 0}), seg({0, 1}, {1, 1}),
      seg({1, -1}, {2, -1}), seg({3, -1}, {3, 2}), seg({-2, -1}, {-2, 2}),
  });
  ASSERT_EQ(6, set.size());
  EXPECT_TRUE(finds(1, 0));

  // diagonal line
  set.clear();
  add({
      seg({0, 0}, {1, 1}), seg({1, 1}, {2, 2}), seg({0, 1}, {1, 2}),
      seg({1, -1}, {2, 0}), seg({3, -3}, {3, 4}),
  });
  ASSERT_EQ(5, set.size());
  EXPECT_TRUE(finds(1, 0));

  // perpendicular lines
  set.clear();
  add({
      seg({0, 0}, {0, 1}), seg({0, 1}, {1, 1}), seg({-1, -1}, {2, -1}),
      seg({-1, 1}, {-1, 0}),
  });
  ASSERT_EQ(4, set.size());
  EXPECT_TRUE(finds(1, 0));
}

TEST_F(FindIntersectionTest, OverlappedSegments) {
  // vertical lines
  add({
      seg({0, 0}, {0, 2}), seg({0, 1}, {0, 3}), seg({1, 0}, {1, 1}),
      seg({2, 2}, {3, 3}), seg({-1, -1}, {1, -1}), seg({-1, 0}, {-1, 3}),
      seg({-2, 0}, {-2, 1}), seg({-3, 2}, {-2, 2}),
  });
  ASSERT_EQ(8, set.size());
  EXPECT_TRUE(finds(1, 0));

  // horizontal line
  set.clear();
  add({
      seg({0, 0}, {2, 0}), seg({1, 0}, {5, 0}), seg({0, 1}, {1, 1}),
      seg({1, -1}, {2, -1}), seg({3, -1}, {3, 2}), seg({-2, -1}, {-2, 2}),
  });
  ASSERT_EQ(6, set.size());
  EXPECT_TRUE(finds(1, 0));

  // diagonal line
  set.clear();
  add({
      seg({0, 0}, {2, 2}), seg({1, 1}, {3, 3}), seg({0, 1}, {1, 2}),
      seg({1, -1}, {2, 0}), seg({3, -3}, {3, 4}),
  });
  ASSERT_EQ(5, set.size());
  EXPECT_TRUE(finds(1, 0));

  // perpendicular lines
  set.clear();
  add({
      seg({0, 0}, {0, 2}), seg({0, 1}, {1, 1}), seg({-1, -1}, {2, -1}),
      seg({-1, 1}, {-1, 0}),
  });
  ASSERT_EQ(4, set.size());
  EXPECT_TRUE(finds(1, 0));
}

TEST_F(FindIntersectionTest, TangentSegmentTest) {
  // Perpendicular case
  add({
      seg({0, 0}, {0, 3}), seg({1, 0}, {2, 2}), seg({0, 2}, {1, 2}),
      seg({-1, -1}, {-1, 1}), seg({-2, 1}, {-1, 2}), seg({-2, 2}, {-1, 3}),
  });
  ASSERT_EQ(6, set.size());
  EXPECT_TRUE(finds(2, 0));

  // Another perpendicular case
  set.clear();
  add({
      seg({-1, -1}, {1, 1}), seg({1, -2}, {2, 1}), seg({-3, 0}, {-2, 1}),
      seg({-1, 1}, {-2, 2}), seg({0, 0}, {1, -1}), seg({-1, -2}, {-2, 0}),
  });
  ASSERT_EQ(6, set.size());
  EXPECT_TRUE(finds(4, 0));

  set.clear();
  add({
      seg({0, 2}, {1, 0}), seg({0, 4}, {1, 3}), seg({2, 1}, {3, 4}),
      seg({1, 1}, {2, 4}), seg({1, 2}, {3, 6}),
  });
  ASSERT_EQ(5, set.size());
  EXPECT_TRUE(finds(4, 3));
}

TEST_F(FindIntersectionTest, CollinearSegmentsWithSameStartTest) {
  add({
      seg({0, 2}, {1, 2}), seg({-2, 0}, {-1, 2}), seg({-1, 1}, {1, 1}),
      seg({0, 0}, {1, -1}), seg({3, -1}, {3, 1}),
  });
  EXPECT_TRUE(finds_nothing());
  add({{-1, 1}, {1, 1}});
  EXPECT_TRUE(finds(2, 5));

  set.clear();
  add({
      seg({1, 1}, {0, 2}), seg({2, 1}, {4, 4}), seg({1, 2}, {2, 3}),
      seg({1, 3}, {2, 4}),
  });
  EXPECT_TRUE(finds_nothing());
  add({{1, 2}, {3, 4}});
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
  void add(point_t point) {
    points.emplace_back(point);
  }
  void add(std::initializer_list<point_t> list) {
    for (const auto& point : list) {
      add(point);
    }
  }

  // Adds a point to the end of the list relative to the last point.
  void rel_add(point_t delta) {
    assert(!points.empty()); // There must be a point of reference.
    add(points.back() + delta);
  }
  void rel_add(std::initializer_list<point_t> deltas) {
    for (const auto& delta : deltas) {
      rel_add(delta);
    }
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
  add({
      {0, 0}, {0, 2}, {2, 2}, {2, 0},
  });
  EXPECT_TRUE(is_simple());
}

TEST_F(SimplePolygonTest, CrossingSquaresTest) {
  add({
      {0, 1}, {1, 1}, {1, 0},
  });
  EXPECT_TRUE(is_simple());

  add({0, 0});
  EXPECT_TRUE(is_simple());

  points.pop_back();
  add({-1, 0});
  EXPECT_TRUE(is_simple());

  add({-1, -1});
  EXPECT_FALSE(is_simple());

  add({0, -1});
  EXPECT_FALSE(is_simple());

  points.pop_back();
  add({1, -1});
  EXPECT_FALSE(is_simple());

  points.pop_back();
  add({2, -1});
  EXPECT_FALSE(is_simple());

  add({2, 0});
  EXPECT_FALSE(is_simple());

  points.pop_back();
  add({2, 2});
  EXPECT_TRUE(is_simple());

  add({-1, 2});
  EXPECT_TRUE(is_simple());
}

TEST_F(SimplePolygonTest, OneIntersectionTest) {
  add({0, 0});
  rel_add({
      {3, -2}, {1, 3}, {-2, 1}, {2, -5}, {-2, -1}, {1, 1},
  });
  ASSERT_EQ(7, points.size());
  ASSERT_EQ(point_t(3, -3), points.back());
  EXPECT_FALSE(is_simple());

  points.clear();
  add({0, 0});
  rel_add({
      {1, 3}, {3, 0}, {1, -3}, {-2, -3}, {-2, 0}, {0, 5},
  });
  EXPECT_TRUE(is_simple());

  rel_add({3, 0});
  rel_add({0, -2});
  EXPECT_FALSE(is_simple());

  rel_add({-2, 0});
  rel_add({0, -1});
  EXPECT_FALSE(is_simple());

  EXPECT_EQ(point_t(2, -1), points.back());
  EXPECT_EQ(11, points.size());
}

TEST_F(SimplePolygonTest, MultipleIntersecionsTest) {
  add({0, 0});
  rel_add({
      {0, 3}, {2, -2}, {0, 2}, {1, -2}, {2, 0}, {-2, -2}, {2, 1}, {-2, -2},
  });
  EXPECT_TRUE(is_simple());
  rel_add({-2, 5});
  EXPECT_FALSE(is_simple());
  rel_add({-2, -2});
  rel_add({2, -1});
  EXPECT_FALSE(is_simple());

  EXPECT_EQ(point_t(1, 0), points.back());
  EXPECT_EQ(12, points.size());

  points.clear();
  add({0, 0});
  rel_add({
      {1, 1}, {1, -1}, {-2, -2}, {2, -2}, {-1, -1}, {-1, 1}, {2, 2},
  });
  EXPECT_EQ(point_t(2, -2), points.back());
  EXPECT_EQ(8, points.size());
  EXPECT_FALSE(is_simple());
}

TEST_F(SimplePolygonTest, NoIntersectionTest) {
  add({0, 0});
  rel_add({
      {0, 3},
      {1, -2},
      {1, 1},
      {0, -3},
      {-1, 1},
      {0, -1},
      {1, -1},
      {1, 1},
      {0, 3},
      {-1, 1},
      {-1, -1},
      {0, 1},
      {1, 1},
      {2, -2},
      {0, -3},
      {-2, -2},
      {-3, 3},
      {0, 3},
  });
  ASSERT_EQ(19, points.size());
  ASSERT_EQ(point_t(-1, 3), points.back());
  EXPECT_TRUE(is_simple());

  points.clear();
  add({0, 0});
  rel_add({
      {0, 3}, {1, -2}, {-1, -2}, {-1, 5},
  });
  ASSERT_EQ(5, points.size());
  ASSERT_EQ(point_t(-1, 4), points.back());
  EXPECT_TRUE(is_simple());
}

TEST_F(SimplePolygonTest, TangentPointTest) {
  add({0, 0});
  rel_add({
      {2, 2}, {0, -3}, {2, 2}, {-4, 2},
  });
  ASSERT_EQ(point_t(0, 3), points.back());
  ASSERT_EQ(5, points.size());
  EXPECT_FALSE(is_simple());

  points.clear();
  add({0, 0});
  rel_add({
      {2, 2}, {0, -3}, {2, 3}, {0, -4}, {2, 0}, {0, -1},
  });
  ASSERT_EQ(7, points.size());
  ASSERT_EQ(point_t(6, -3), points.back());
  EXPECT_FALSE(is_simple());
}

TEST_F(SimplePolygonTest, CollinearChainedTest) {
  add({0, 0});
  rel_add({
      {0, 1},
      {0, 1},
      {1, 1},
      {0, -1},
      {0, -1},
      {0, -1},
      {0, -1},
      {-1, 0},
      {-1, 0},
      {0, 1},
      {0, 1},
      {0, 1},
  });
  ASSERT_EQ(13, points.size());
  ASSERT_EQ(point_t(-1, 2), points.back());
  EXPECT_TRUE(is_simple());

  points.clear();
  add({0, 0});
  rel_add({
      {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {-4, -5},
  });
  ASSERT_EQ(7, points.size());
  ASSERT_EQ(point_t(1, 0), points.back());
  EXPECT_TRUE(is_simple());
}

TEST_F(SimplePolygonTest, OverlappedSegments) {
  add({0, 0});
  rel_add({
      {1, 3}, {2, 0}, {1, -3}, {1, 3},
  });
  ASSERT_EQ(5, points.size());
  ASSERT_EQ(point_t(5, 3), points.back());
  EXPECT_FALSE(is_simple());

  points.clear();
  add({0, 0});
  rel_add({
      {0, 2}, {1, 0}, {0, -2}, {1, 0}, {0, 3}, {-2, 0},
  });
  ASSERT_EQ(7, points.size());
  ASSERT_EQ(point_t(0, 3), points.back());
  EXPECT_FALSE(is_simple());
}

TEST_F(SimplePolygonTest, EightTest) {
  add({0, 0});
  rel_add({
      {1, 1}, {1, -1}, {-1, -1}, {-1, -1}, {1, -1}, {1, 1}, {-1, 1},
  });
  ASSERT_EQ(8, points.size());
  ASSERT_EQ(point_t(1, -1), points.back());
  EXPECT_TRUE(is_simple());
}

TEST_F(SimplePolygonTest, RareSimplePolygonTest) {
  add({0, 0});
  rel_add({
      {3, -4},
      {2, 7},
      {5, 0},
      {-3, -3},
      {-3, -4},
      {3, -2},
      {3, -2},
      {1, 0},
      {2, -2},
      {-3, 2},
      {-1, 0},
      {-5, -2},
      {0, 4},
      {-1, -1},
  });
  ASSERT_EQ(15, points.size());
  ASSERT_EQ(point_t(3, -7), points.back());
  ASSERT_EQ(2, std::count(points.begin(), points.end(), point_t(10, -8)));
  EXPECT_TRUE(is_simple());
}
