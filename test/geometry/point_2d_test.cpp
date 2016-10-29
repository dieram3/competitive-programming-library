//          Copyright Jorge Aguirre 2015, Diego Ramirez 2016
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cpl/geometry/point_2d.hpp>
#include <gtest/gtest.h>

#include <cmath>   // sqrt
#include <cstdint> // int32_t
#include <ostream> // ostream
#include <sstream> // ostringstream

using cpl::point;
using std::int32_t;
using point_i = point<int32_t>;

namespace cpl {
template <typename T>
static std::ostream& operator<<(std::ostream& os, const point<T>& p) {
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

// ===--------------------------------------------------===
//                    Point 2D Version 2
// ===--------------------------------------------------===

namespace {
class Point2DV2Test : public ::testing::Test {
protected:
  using point = cpl::point2d<int>;
  using vec = cpl::vector2d<int>;
};
} // end anonymous namespace

TEST_F(Point2DV2Test, ValueConstructorTest) {
  point p{};
  EXPECT_EQ(0, p.x);
  EXPECT_EQ(0, p.y);
}

TEST_F(Point2DV2Test, PairConstructorTest) {
  {
    point p{0, 0};
    EXPECT_EQ(0, p.x);
    EXPECT_EQ(0, p.y);
  }
  {
    point p{0, 3};
    EXPECT_EQ(0, p.x);
    EXPECT_EQ(3, p.y);
  }
  {
    point p{2, 0};
    EXPECT_EQ(2, p.x);
    EXPECT_EQ(0, p.y);
  }
  {
    point p{4, 7};
    EXPECT_EQ(4, p.x);
    EXPECT_EQ(7, p.y);
  }
}

TEST_F(Point2DV2Test, VectorAdditionTest) {
  EXPECT_EQ(point(0, 0), point(0, 0) + vec(0, 0));
  EXPECT_EQ(point(6, 8), point(2, 3) + vec(4, 5));
  EXPECT_EQ(point(2, -2), point(-2, 3) + vec(4, -5));
  EXPECT_EQ(point(7, 27), point(10, 23) + vec(-3, 4));

  EXPECT_EQ(point(0, 0), vec(0, 0) + point(0, 0));
  EXPECT_EQ(point(6, 8), vec(4, 5) + point(2, 3));
  EXPECT_EQ(point(2, -2), vec(4, -5) + point(-2, 3));
  EXPECT_EQ(point(7, 27), vec(-3, 4) + point(10, 23));
}

TEST_F(Point2DV2Test, VectorSubtractionTest) {
  EXPECT_EQ(point(0, 0), point(0, 0) - vec(0, 0));
  EXPECT_EQ(point(2, 1), point(2, 1) - vec(0, 0));
  EXPECT_EQ(point(-3, -5), point(0, 0) - vec(3, 5));

  EXPECT_EQ(point(-6, 8), point(-2, 3) - vec(4, -5));
  EXPECT_EQ(point(13, 19), point(10, 23) - vec(-3, 4));
}

TEST_F(Point2DV2Test, PointsDifferenceTest) {
  EXPECT_EQ(vec(0, 0), point(0, 0) - point(0, 0));
  EXPECT_EQ(vec(-1, -3), point(0, 0) - point(1, 3));
  EXPECT_EQ(vec(2, 5), point(2, 5) - point(0, 0));

  EXPECT_EQ(vec(-4, 1), point(3, 4) - point(7, 3));
  EXPECT_EQ(vec(10, 3), point(3, 6) - point(-7, 3));
  EXPECT_EQ(vec(-5, -2), point(3, -5) - point(8, -3));
  EXPECT_EQ(vec(-5, -10), point(-2, 1) - point(3, 11));
}

TEST_F(Point2DV2Test, EqualityOperatorTest) {
  EXPECT_TRUE(point(0, 0) == point(0, 0));
  EXPECT_TRUE(point(1, 2) == point(1, 2));
  EXPECT_TRUE(point(4, 7) == point(4, 7));

  EXPECT_FALSE(point(0, 0) == point(0, 1));
  EXPECT_FALSE(point(0, 0) == point(1, 0));
  EXPECT_FALSE(point(1, 2) == point(2, 1));
  EXPECT_FALSE(point(4, 7) == point(7, 4));
}

TEST_F(Point2DV2Test, InequalityOperatorTest) {
  EXPECT_FALSE(point(0, 0) != point(0, 0));
  EXPECT_FALSE(point(1, 2) != point(1, 2));
  EXPECT_FALSE(point(4, 7) != point(4, 7));

  EXPECT_TRUE(point(0, 0) != point(0, 1));
  EXPECT_TRUE(point(0, 0) != point(1, 0));
  EXPECT_TRUE(point(1, 2) != point(2, 1));
  EXPECT_TRUE(point(4, 7) != point(7, 4));
}

TEST_F(Point2DV2Test, SerializationTest) {
  auto serialize = [](const point& p) {
    std::ostringstream oss;
    oss << p;
    return oss.str();
  };

  EXPECT_EQ("(0,0)", serialize(point{0, 0}));
  EXPECT_EQ("(0,-1)", serialize(point{0, -1}));
  EXPECT_EQ("(4,0)", serialize(point{4, 0}));

  EXPECT_EQ("(2,3)", serialize(point{2, 3}));
  EXPECT_EQ("(-2,3)", serialize(point{-2, 3}));
  EXPECT_EQ("(2,-3)", serialize(point{2, -3}));
  EXPECT_EQ("(-2,-3)", serialize(point{-2, -3}));
}

TEST_F(Point2DV2Test, SquaredDistanceTest) {
  using cpl::squared_distance;

  EXPECT_EQ(0, squared_distance(point{0, 0}, point{0, 0}));
  EXPECT_EQ(0, squared_distance(point{0, 1}, point{0, 1}));
  EXPECT_EQ(0, squared_distance(point{1, 0}, point{1, 0}));
  EXPECT_EQ(0, squared_distance(point{2, -3}, point{2, -3}));

  EXPECT_EQ(1, squared_distance(point{1, 0}, point{2, 0}));
  EXPECT_EQ(2, squared_distance(point{3, 3}, point{4, 4}));
  EXPECT_EQ(73, squared_distance(point{-3, 4}, point{5, 7}));
  EXPECT_EQ(25, squared_distance(point{-3, 2}, point{1, -1}));
}

TEST_F(Point2DV2Test, DistanceTest) {
  using cpl::distance;
  using std::sqrt;
  using pt = cpl::point2d<float>;

  EXPECT_FLOAT_EQ(0, distance(pt{0, 0}, pt{0, 0}));
  EXPECT_FLOAT_EQ(0, distance(pt{0, 1}, pt{0, 1}));
  EXPECT_FLOAT_EQ(0, distance(pt{1, 0}, pt{1, 0}));
  EXPECT_FLOAT_EQ(0, distance(pt{2, -3}, pt{2, -3}));

  EXPECT_FLOAT_EQ(1.0f, distance(pt{1, 0}, pt{2, 0}));
  EXPECT_FLOAT_EQ(sqrt(2.0f), distance(pt{3, 3}, pt{4, 4}));
  EXPECT_FLOAT_EQ(sqrt(73.0f), distance(pt{-3, 4}, pt{5, 7}));
  EXPECT_FLOAT_EQ(5.0f, distance(pt{-3, 2}, pt{1, -1}));
}
