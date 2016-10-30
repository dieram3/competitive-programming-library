//          Copyright Jorge Aguirre 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//

#include <cpl/geometry/shoelace.hpp>
#include <gtest/gtest.h>

#include <cpl/geometry/point_2d.hpp> // point2d
#include <vector>                    // vector

using cpl::shoelace_area;
using cpl::compute_polygon_area;
using point_i = cpl::point2d<int>;

TEST(ShoelaceTest, SimpleShapes) {
  // A square
  std::vector<point_i> poly = {{0, 0}, {0, 2}, {2, 2}, {2, 0}};

  EXPECT_EQ(8, shoelace_area(poly));
  EXPECT_EQ(4, compute_polygon_area(poly));

  // A square with colinear lines
  poly = {{0, 0}, {0, 1}, {0, 2}, {1, 2}, {2, 2}, {2, 1}, {2, 0}, {1, 0}};
  EXPECT_EQ(8, shoelace_area(poly));
  EXPECT_EQ(4, compute_polygon_area(poly));

  // A triangle
  poly = {{2, 6}, {6, 4}, {4, 1}};
  EXPECT_EQ(16, shoelace_area(poly));
  EXPECT_EQ(8, compute_polygon_area(poly));
}

TEST(ShoelaceTest, NonConvexPolygon) {
  std::vector<point_i> poly = {{1, 6}, {1, 3}, {1, 2}, {1, 1},  {2, 4},
                               {3, 2}, {5, 1}, {6, 3}, {10, 3}, {8, 6},
                               {9, 4}, {3, 5}, {4, 6}};
  EXPECT_EQ(42, shoelace_area(poly));
  EXPECT_EQ(21, compute_polygon_area(poly));
}
