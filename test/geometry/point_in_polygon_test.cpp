//          Copyright Jorge Aguirre, Diego Ramirez 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cpl/geometry/point_in_polygon.hpp>
#include <gtest/gtest.h>

#include <cpl/geometry/point_2d.hpp> // point
#include <cassert>                   // assert
#include <initializer_list>          // initializer_list
#include <vector>                    // vector

namespace {

class PointInPolygonTest : public ::testing::Test {
protected:
  using scalar_t = long;
  using point_t = cpl::point<scalar_t>;

protected:
  void add(point_t point) {
    poly.emplace_back(point);
  }
  void add(std::initializer_list<point_t> points) {
    for (const auto point : points) {
      add(point);
    }
  }

  void rel_add(point_t delta) {
    assert(!poly.empty()); // There must be a point of reference.
    add(poly.back() + delta);
  }
  void rel_add(std::initializer_list<point_t> deltas) {
    for (const auto delta : deltas) {
      rel_add(delta);
    }
  }

  bool in_polygon(scalar_t x, scalar_t y) const {
    assert(poly.size() >= 3);
    return cpl::point_in_polygon(point_t{x, y}, poly);
  }

protected:
  std::vector<point_t> poly;
};

} // end anonymous namespace

TEST_F(PointInPolygonTest, TriangleTest) {
  add({{1, 1}, {4, 3}, {2, 4}});
  EXPECT_FALSE(in_polygon(6, 6));
  EXPECT_TRUE(in_polygon(3, 3));
}

TEST_F(PointInPolygonTest, SquareTest) {
  add({{0, 0}, {0, 4}, {4, 4}, {4, 0}});
  EXPECT_FALSE(in_polygon(6, 6));
  EXPECT_TRUE(in_polygon(2, 3));
}

TEST_F(PointInPolygonTest, CollinearPointsTest) {
  add({{0, 0}, {0, 4}, {4, 4}, {4, 0}});
  EXPECT_FALSE(in_polygon(-1, 0));
  EXPECT_FALSE(in_polygon(5, 0));
  EXPECT_FALSE(in_polygon(0, 6));
  EXPECT_FALSE(in_polygon(4, 6));

  poly.clear();
  add({{0, 0}, {1, 1}, {2, 0}});

  EXPECT_FALSE(in_polygon(2, 2));
  EXPECT_FALSE(in_polygon(0, 2));
  EXPECT_FALSE(in_polygon(-1, 0));
  EXPECT_FALSE(in_polygon(3, 0));
  EXPECT_FALSE(in_polygon(3, -1));
  EXPECT_FALSE(in_polygon(-1, -1));
}

TEST_F(PointInPolygonTest, NonConvexPolygonTest) {
  add({{1, 1}, {3, 0}, {4, 2}, {6, 1}, {3, 6}, {4, 3}, {2, 4}});

  EXPECT_TRUE(in_polygon(2, 3));
  EXPECT_TRUE(in_polygon(5, 2));
  EXPECT_TRUE(in_polygon(4, 4));

  EXPECT_FALSE(in_polygon(-1, -1));
  EXPECT_FALSE(in_polygon(3, 4));
  EXPECT_FALSE(in_polygon(4, 1));
}

TEST_F(PointInPolygonTest, RingLikePolygonTest) {
  add({2, 3});
  rel_add({
      {-5, 0}, {0, -2}, {0, -2}, {0, -2}, {3, 0},  {0, -1}, {2, 0},  {0, 1},
      {2, 0},  {0, 3},  {1, 1},  {0, 5},  {-2, 0}, {-4, 0}, {-2, 0}, {0, -2},
      {2, 0},  {0, 1},  {4, 0},  {0, -4}, {0, -3}, {-4, 0}, {0, 4},  {3, 0},
  });

  ASSERT_EQ(25, poly.size());
  ASSERT_EQ(point_t(2, 2), poly.back());

  // Points outside the ring.
  EXPECT_FALSE(in_polygon(-4, 3));
  EXPECT_FALSE(in_polygon(-4, 4));
  EXPECT_FALSE(in_polygon(5, 7));
  EXPECT_FALSE(in_polygon(6, 7));
  EXPECT_FALSE(in_polygon(5, 0));
  EXPECT_FALSE(in_polygon(6, -5));
  EXPECT_FALSE(in_polygon(3, -4));
  EXPECT_FALSE(in_polygon(-1, -4));

  // Points in the hole of the ring.
  EXPECT_FALSE(in_polygon(2, -1));
  EXPECT_FALSE(in_polygon(1, 0));
  EXPECT_FALSE(in_polygon(2, 1));
  EXPECT_FALSE(in_polygon(0, 1));
  EXPECT_FALSE(in_polygon(2, 4));
  EXPECT_FALSE(in_polygon(0, 4));

  // Points on the ring.
  EXPECT_TRUE(in_polygon(-2, 1));
  EXPECT_TRUE(in_polygon(-2, 0));
  EXPECT_TRUE(in_polygon(-2, -2));
  EXPECT_TRUE(in_polygon(1, -3));
  EXPECT_TRUE(in_polygon(4, 1));
  EXPECT_TRUE(in_polygon(4, 3));
  EXPECT_TRUE(in_polygon(4, 5));
  EXPECT_TRUE(in_polygon(-2, 5));
}

TEST_F(PointInPolygonTest, DistortedPolygonTest) {
  add({-3, 2});
  rel_add({
      {2, 0},   {2, 0},   {1, 0},   {-1, -1}, {1, -1}, {1, 1},
      {-1, 2},  {2, -1},  {1, -1},  {-1, -1}, {-1, 0}, {-1, -2},
      {-1, 1},  {1, -2},  {2, 1},   {-1, 1},  {3, 0},  {-1, -1},
      {0, -1},  {-1, -1}, {-1, 0},  {-1, 0},  {-1, 1}, {-1, 1},
      {-1, -2}, {-1, 2},  {-1, -1}, {-1, 2},  {1, 2},  {1, -2},
      {-1, -1}, {2, 1},   {0, -1},  {2, 2},   {-1, 1}, {-1, -1},
  });

  ASSERT_EQ(37, poly.size());
  ASSERT_EQ(point_t(-1, 0), poly.back());

  // Points outside polygon, trivial cases.
  EXPECT_FALSE(in_polygon(0, 3));
  EXPECT_FALSE(in_polygon(5, 3));
  EXPECT_FALSE(in_polygon(5, 0));
  EXPECT_FALSE(in_polygon(3, -5));
  EXPECT_FALSE(in_polygon(0, -3));
  EXPECT_FALSE(in_polygon(-2, -3));
  EXPECT_FALSE(in_polygon(-5, -1));
  EXPECT_FALSE(in_polygon(-5, 3));

  // Points outside polygon, non-trivial cases.
  EXPECT_FALSE(in_polygon(0, 0));
  EXPECT_FALSE(in_polygon(-2, 0));
  EXPECT_FALSE(in_polygon(2, 1));
  EXPECT_FALSE(in_polygon(3, -2));

  // Points inside the polygon.
  EXPECT_TRUE(in_polygon(-1, 1));
  EXPECT_TRUE(in_polygon(-3, 0));
  EXPECT_TRUE(in_polygon(-3, -1));
  EXPECT_TRUE(in_polygon(-1, -3));
  EXPECT_TRUE(in_polygon(1, -2));
  EXPECT_TRUE(in_polygon(2, -1));
  EXPECT_TRUE(in_polygon(3, -3));
  EXPECT_TRUE(in_polygon(4, -3));
  EXPECT_TRUE(in_polygon(4, 1));
  EXPECT_TRUE(in_polygon(3, 2));
}
