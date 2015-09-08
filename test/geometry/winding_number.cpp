//          Copyright Jorge Aguirre August 2015
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
  // A trinagle
  std::vector<point_t> poly = {{1, 1}, {4, 3}, {2, 4}};
  EXPECT_FALSE(point_in_polygon({6, 6}, poly));
  EXPECT_TRUE(point_in_polygon({3, 3}, poly));

  // A square
  poly = {{0, 0}, {0, 4}, {4, 4}, {4, 0}};
  EXPECT_FALSE(point_in_polygon({6, 6}, poly));
  EXPECT_TRUE(point_in_polygon({2, 3}, poly));
}

TEST(PointInPolygonTest, ColinearPoint) {
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
  std::vector<point_t> poly = {{1, 1}, {3, 0}, {4, 2}, {6, 1},
                               {3, 6}, {4, 3}, {2, 4}};
  EXPECT_TRUE(point_in_polygon({2, 3}, poly));
  EXPECT_TRUE(point_in_polygon({5, 2}, poly));
  EXPECT_TRUE(point_in_polygon({4, 4}, poly));

  EXPECT_FALSE(point_in_polygon({-1, -1}, poly));
  EXPECT_FALSE(point_in_polygon({3, 4}, poly));
  EXPECT_FALSE(point_in_polygon({4, 1}, poly));
}

TEST(PointInPolygonTest, ConvolutedTest) {
  std::vector<point_t> poly = {{0, 0}, {2, 4}, {4, 0}, {0, 3}, {5, 3}};

  EXPECT_FALSE(point_in_polygon({-1, -1}, poly));
  EXPECT_FALSE(point_in_polygon({1, 0}, poly));
  EXPECT_FALSE(point_in_polygon({2, 0}, poly));
  EXPECT_FALSE(point_in_polygon({3, 0}, poly));

  EXPECT_TRUE(point_in_polygon({2, 3}, poly));
  EXPECT_TRUE(point_in_polygon({2, 2}, poly));
  EXPECT_TRUE(point_in_polygon({1, 1}, poly));
}
