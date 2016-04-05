//          Copyright Jorge Aguirre July 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cpl/geometry/point_2d.hpp>
#include <gtest/gtest.h>

#include <ostream> // For std::ostream
#include <cmath>   // For std::sqrt
#include <cstdint> // For std::int32_t

using namespace cpl;
using std::int32_t;
using point_i = point<int32_t>;

namespace cpl {
template <typename T>
static std::ostream &operator<<(std::ostream &os, const point<T> &p) {
  os << '(' << p.x << ", " << p.y << ')';
  return os;
}
} // end namespace cpl

TEST(Point2DTest, BasicOperations) {
  point_i lhs = {1, 2};
  point_i rhs = {5, 4};
  point_i expected = {6, 6};
  EXPECT_EQ(lhs + rhs, expected);

  expected = {-4, -2};
  EXPECT_EQ(lhs - rhs, expected);

  int32_t k = 2;
  expected = {2, 4};
  EXPECT_EQ(k * lhs, expected);
  EXPECT_EQ(lhs * k, expected);

  expected = {0, 1};
  EXPECT_EQ(lhs / k, expected);
}

TEST(Point2DTest, DotProduct) {
  point_i lhs = {3, 4};
  point_i rhs = {2, 1};
  int32_t expected = 3 * 2 + 4 * 1;
  EXPECT_EQ(lhs * rhs, expected);
  EXPECT_EQ(rhs * lhs, expected);
}

TEST(Point2DTest, WedgeProduct) {
  point_i lhs = {3, 4};
  point_i rhs = {2, 1};
  int32_t expected = 3 * 1 - 4 * 2;
  EXPECT_EQ(lhs ^ rhs, expected);
  expected = 2 * 4 - 3 * 1;
  EXPECT_EQ(rhs ^ lhs, expected);
}

TEST(Point2DTest, LexicographicalComparison) {
  point_i lhs = {3, 4};
  point_i rhs = {2, 1};
  EXPECT_TRUE(lhs > rhs);
  EXPECT_TRUE(lhs != rhs);
  EXPECT_TRUE(lhs >= rhs);
  lhs = rhs;
  EXPECT_EQ(lhs, rhs);
}

TEST(Point2DTest, SquareNormAndSquaredDistance) {
  point_i q = {3, 4};
  point_i p = {2, 1};
  int32_t expected = 3 * 3 + 4 * 4;
  EXPECT_EQ(norm2(q), expected);
  expected = (3 - 2) * (3 - 2) + (4 - 1) * (4 - 1);
  EXPECT_EQ(distance2(p, q), expected);
}

TEST(Point2DTest, NormAndDistance) {
  point_i q = {3, 4};
  point_i p = {2, 1};
  auto expected = std::sqrt(3 * 3 + 4 * 4);
  EXPECT_EQ(norm(q), expected);
  expected = std::sqrt((3 - 2) * (3 - 2) + (4 - 1) * (4 - 1));
  EXPECT_EQ(distance(p, q), expected);
}
