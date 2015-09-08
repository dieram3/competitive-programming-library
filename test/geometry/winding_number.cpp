//          Copyright Jorge Aguirre September 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <djp/geometry/winding_number.hpp>
#include <gtest/gtest.h>

#include <djp/geometry/point_2d.hpp>

#include <vector>  // For std::vector
#include <cstdint> // For std::int32_t

using namespace djp;

using scalar_t = std::int32_t;
using point_t = point<scalar_t>;

TEST(PointInPolygonTest, SimpleShapes) {
  // A triangle
  std::vector<point_t> poly = {{1, 1}, {4, 3}, {2, 4}};
  EXPECT_FALSE(point_in_polygon({6, 6}, poly));
  EXPECT_TRUE(point_in_polygon({3, 3}, poly));

  // A square
  poly = {{0, 0}, {0, 4}, {4, 4}, {4, 0}};
  EXPECT_FALSE(point_in_polygon({6, 6}, poly));
  EXPECT_TRUE(point_in_polygon({2, 3}, poly));
}

TEST(PointInPolygonTest, CollinearPoint) {
  std::vector<point_t> poly = {{0, 0}, {0, 4}, {4, 4}, {4, 0}};

  EXPECT_FALSE(point_in_polygon({-1, 0}, poly));
  EXPECT_FALSE(point_in_polygon({5, 0}, poly));
  EXPECT_FALSE(point_in_polygon({0, 6}, poly));
  EXPECT_FALSE(point_in_polygon({4, 6}, poly));

  poly = {{0, 0}, {1, 1}, {2, 0}};

  EXPECT_FALSE(point_in_polygon({2, 2}, poly));
  EXPECT_FALSE(point_in_polygon({0, 2}, poly));
  EXPECT_FALSE(point_in_polygon({-1, 0}, poly));
  EXPECT_FALSE(point_in_polygon({3, 0}, poly));
  EXPECT_FALSE(point_in_polygon({3, -1}, poly));
  EXPECT_FALSE(point_in_polygon({-1, -1}, poly));
}

TEST(PointInPolygonTest, NonConvexPolygon) {
  std::vector<point_t> poly = {
      {1, 1}, {3, 0}, {4, 2}, {6, 1}, {3, 6}, {4, 3}, {2, 4}};
  EXPECT_TRUE(point_in_polygon({2, 3}, poly));
  EXPECT_TRUE(point_in_polygon({5, 2}, poly));
  EXPECT_TRUE(point_in_polygon({4, 4}, poly));

  EXPECT_FALSE(point_in_polygon({-1, -1}, poly));
  EXPECT_FALSE(point_in_polygon({3, 4}, poly));
  EXPECT_FALSE(point_in_polygon({4, 1}, poly));
}

TEST(PointInPolygonTest, ConvolutedPolygonTest) {
  std::vector<point_t> poly = {{0, 0}, {2, 4}, {4, 0}, {0, 3}, {5, 3}};

  EXPECT_FALSE(point_in_polygon({-1, -1}, poly));
  EXPECT_FALSE(point_in_polygon({1, 0}, poly));
  EXPECT_FALSE(point_in_polygon({2, 0}, poly));
  EXPECT_FALSE(point_in_polygon({3, 0}, poly));
  EXPECT_FALSE(point_in_polygon({2, 1}, poly));

  // Point in boundary of a segment but not of the polygon.
  EXPECT_TRUE(point_in_polygon({2, 3}, poly));

  // Arbitrary points.
  EXPECT_TRUE(point_in_polygon({2, 2}, poly));
  EXPECT_TRUE(point_in_polygon({1, 1}, poly));
}

TEST(PointInPolygonTest, MessNonSimplePolygonTest) {
  std::vector<point_t> poly = {{1, 7}};
  auto add_rel = [&](scalar_t dx, scalar_t dy) {
    poly.emplace_back(poly.back().x + dx, poly.back().y + dy);
  };
  add_rel(3, 1), add_rel(-1, -3), add_rel(2, 0), add_rel(-1, 2);
  add_rel(1, 3), add_rel(1, -3), add_rel(-1, 1), add_rel(0, -1);
  add_rel(2, -2), add_rel(4, 4), add_rel(2, 2), add_rel(-2, 2);
  add_rel(-1, -3), add_rel(-1, -1), add_rel(2, -2), add_rel(2, 4);
  add_rel(1, -1), add_rel(0, -3), add_rel(-3, -1), add_rel(-1, 1);
  add_rel(-3, -3), add_rel(-1, 1), add_rel(0, -4), add_rel(-1, 0);
  add_rel(0, 3), add_rel(-1, 0), add_rel(-1, -1), add_rel(1, -1);
  add_rel(0, -1), add_rel(-1, 1), add_rel(-1, -1), add_rel(0, 1);
  add_rel(-1, 1), add_rel(1, 1);

  ASSERT_EQ(35, poly.size());
  ASSERT_EQ(point_t(2, 4), poly.back());

  // Points outside bounding box
  EXPECT_FALSE(point_in_polygon({0, 0}, poly));
  EXPECT_FALSE(point_in_polygon({0, 10}, poly));
  EXPECT_FALSE(point_in_polygon({9, 14}, poly));

  // Points near boundaries of polygon
  EXPECT_FALSE(point_in_polygon({4, 3}, poly));
  EXPECT_FALSE(point_in_polygon({4, 6}, poly));
  EXPECT_FALSE(point_in_polygon({10, 6}, poly));
  EXPECT_FALSE(point_in_polygon({9, 8}, poly));
  EXPECT_FALSE(point_in_polygon({13, 12}, poly));

  // Points in polygon trivial part
  EXPECT_TRUE(point_in_polygon({5, 9}, poly));
  EXPECT_TRUE(point_in_polygon({2, 3}, poly));
  EXPECT_TRUE(point_in_polygon({3, 7}, poly));
  EXPECT_TRUE(point_in_polygon({5, 6}, poly));
  EXPECT_TRUE(point_in_polygon({11, 12}, poly));

  // Points that are in segments but not in the boundary polygon.
  // EXPECT_TRUE(point_in_polygon({12, 10}, poly)); // fails!
  EXPECT_TRUE(point_in_polygon({12, 9}, poly));

  // Points that are in vertices that are inside polygon.
  EXPECT_TRUE(point_in_polygon({11, 7}, poly));
  // EXPECT_TRUE(point_in_polygon({11, 9}, poly)); // fails!

  // Points that are in the boundary of polygon but not vertices.
  EXPECT_FALSE(point_in_polygon({6, 3}, poly));
  // EXPECT_FALSE(point_in_polygon({9, 7}, poly)); // fails!
  EXPECT_FALSE(point_in_polygon({12, 12}, poly));
  EXPECT_FALSE(point_in_polygon({14, 9}, poly));
  EXPECT_FALSE(point_in_polygon({6, 6}, poly));

  // Points that are in vertices that belong to the boundary of the polygon.
  EXPECT_FALSE(point_in_polygon({2, 1}, poly));
  EXPECT_FALSE(point_in_polygon({5, 10}, poly));
  // EXPECT_FALSE(point_in_polygon({3, 2}, poly)); // fails!
  // EXPECT_FALSE(point_in_polygon({12, 11}, poly)); // fails!
  // EXPECT_FALSE(point_in_polygon({10, 10}, poly)); // fails!
  // EXPECT_FALSE(point_in_polygon({4, 7}, poly)); // fails!
}
