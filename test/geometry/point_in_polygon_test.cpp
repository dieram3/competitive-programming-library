//          Copyright Jorge Aguirre, Diego Ramírez September 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <djp/geometry/point_in_polygon.hpp>
#include <gtest/gtest.h>

#include <djp/geometry/point_2d.hpp>

#include <vector>  // For std::vector
#include <cassert> // For assert

using namespace djp;

namespace {

class PointInPolygonTest : public ::testing::Test {
protected:
  using scalar_t = long;
  using point_t = point<scalar_t>;

protected:
  void add(scalar_t x, scalar_t y) { poly.emplace_back(x, y); }
  void rel_add(scalar_t dx, scalar_t dy) {
    add(poly.back().x + dx, poly.back().y + dy);
  }
  bool in_polygon(scalar_t x, scalar_t y) const {
    assert(poly.size() >= 3);
    return point_in_polygon(point_t(x, y), poly);
  }

protected:
  std::vector<point_t> poly;
};

} // end namespace djp

TEST_F(PointInPolygonTest, TriangleTest) {
  add(1, 1), add(4, 3), add(2, 4);
  EXPECT_FALSE(in_polygon(6, 6));
  EXPECT_TRUE(in_polygon(3, 3));
}

TEST_F(PointInPolygonTest, SquareTest) {
  add(0, 0), add(0, 4), add(4, 4), add(4, 0);
  EXPECT_FALSE(in_polygon(6, 6));
  EXPECT_TRUE(in_polygon(2, 3));
}

TEST_F(PointInPolygonTest, CollinearPointsTest) {
  add(0, 0), add(0, 4), add(4, 4), add(4, 0);
  EXPECT_FALSE(in_polygon(-1, 0));
  EXPECT_FALSE(in_polygon(5, 0));
  EXPECT_FALSE(in_polygon(0, 6));
  EXPECT_FALSE(in_polygon(4, 6));

  poly.clear();
  add(0, 0), add(1, 1), add(2, 0);

  EXPECT_FALSE(in_polygon(2, 2));
  EXPECT_FALSE(in_polygon(0, 2));
  EXPECT_FALSE(in_polygon(-1, 0));
  EXPECT_FALSE(in_polygon(3, 0));
  EXPECT_FALSE(in_polygon(3, -1));
  EXPECT_FALSE(in_polygon(-1, -1));
}

TEST_F(PointInPolygonTest, NonConvexPolygonTest) {
  add(1, 1), add(3, 0), add(4, 2), add(6, 1);
  add(3, 6), add(4, 3), add(2, 4);

  EXPECT_TRUE(in_polygon(2, 3));
  EXPECT_TRUE(in_polygon(5, 2));
  EXPECT_TRUE(in_polygon(4, 4));

  EXPECT_FALSE(in_polygon(-1, -1));
  EXPECT_FALSE(in_polygon(3, 4));
  EXPECT_FALSE(in_polygon(4, 1));
}

TEST_F(PointInPolygonTest, RingLikePolygonTest) {
  add(2, 3);
  rel_add(-5, 0), rel_add(0, -2), rel_add(0, -2), rel_add(0, -2);
  rel_add(3, 0), rel_add(0, -1), rel_add(2, 0), rel_add(0, 1);
  rel_add(2, 0), rel_add(0, 3), rel_add(1, 1), rel_add(0, 5);
  rel_add(-2, 0), rel_add(-4, 0), rel_add(-2, 0), rel_add(0, -2);
  rel_add(2, 0), rel_add(0, 1), rel_add(4, 0), rel_add(0, -4);
  rel_add(0, -3), rel_add(-4, 0), rel_add(0, 4), rel_add(3, 0);

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
  add(-3, 2);
  rel_add(2, 0), rel_add(2, 0), rel_add(1, 0), rel_add(-1, -1);
  rel_add(1, -1), rel_add(1, 1), rel_add(-1, 2), rel_add(2, -1);
  rel_add(1, -1), rel_add(-1, -1), rel_add(-1, 0), rel_add(-1, -2);
  rel_add(-1, 1), rel_add(1, -2), rel_add(2, 1), rel_add(-1, 1);
  rel_add(3, 0), rel_add(-1, -1), rel_add(0, -1), rel_add(-1, -1);
  rel_add(-1, 0), rel_add(-1, 0), rel_add(-1, 1), rel_add(-1, 1);
  rel_add(-1, -2), rel_add(-1, 2), rel_add(-1, -1), rel_add(-1, 2);
  rel_add(1, 2), rel_add(1, -2), rel_add(-1, -1), rel_add(2, 1);
  rel_add(0, -1), rel_add(2, 2), rel_add(-1, 1), rel_add(-1, -1);

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
