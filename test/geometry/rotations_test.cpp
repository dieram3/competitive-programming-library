//          Copyright Diego Ramirez 2016
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cpl/geometry/rotations.hpp>
#include <gtest/gtest.h>

#include <cpl/geometry/point_2d.hpp> // point

namespace {
class PointRotationTest : public ::testing::Test {};
} // end anonymous namespace

TEST_F(PointRotationTest, Rotate90CCWTest) {
  using pt = cpl::point<int>;
  using cpl::rotate_90_ccw;

  EXPECT_EQ(pt(0, 0), rotate_90_ccw(pt(0, 0)));

  EXPECT_EQ(pt(0, 1), rotate_90_ccw(pt(1, 0)));
  EXPECT_EQ(pt(-1, 0), rotate_90_ccw(pt(0, 1)));
  EXPECT_EQ(pt(0, -1), rotate_90_ccw(pt(-1, 0)));
  EXPECT_EQ(pt(1, 0), rotate_90_ccw(pt(0, -1)));

  EXPECT_EQ(pt(-3, 2), rotate_90_ccw(pt(2, 3)));
  EXPECT_EQ(pt(4, 3), rotate_90_ccw(pt(3, -4)));
  EXPECT_EQ(pt(-5, -1), rotate_90_ccw(pt(-1, 5)));
  EXPECT_EQ(pt(7, -4), rotate_90_ccw(pt(-4, -7)));
}

TEST_F(PointRotationTest, Rotate180CCWTest) {
  using pt = cpl::point<int>;
  using cpl::rotate_180_ccw;

  EXPECT_EQ(pt(0, 0), rotate_180_ccw(pt(0, 0)));

  EXPECT_EQ(pt(-1, 0), rotate_180_ccw(pt(1, 0)));
  EXPECT_EQ(pt(0, -1), rotate_180_ccw(pt(0, 1)));
  EXPECT_EQ(pt(1, 0), rotate_180_ccw(pt(-1, 0)));
  EXPECT_EQ(pt(0, 1), rotate_180_ccw(pt(0, -1)));

  EXPECT_EQ(pt(-2, -3), rotate_180_ccw(pt(2, 3)));
  EXPECT_EQ(pt(-3, 4), rotate_180_ccw(pt(3, -4)));
  EXPECT_EQ(pt(1, -5), rotate_180_ccw(pt(-1, 5)));
  EXPECT_EQ(pt(4, 7), rotate_180_ccw(pt(-4, -7)));
}

TEST_F(PointRotationTest, Rotate270CCWTest) {
  using pt = cpl::point<int>;
  using cpl::rotate_270_ccw;

  EXPECT_EQ(pt(0, 0), rotate_270_ccw(pt(0, 0)));

  EXPECT_EQ(pt(0, -1), rotate_270_ccw(pt(1, 0)));
  EXPECT_EQ(pt(1, 0), rotate_270_ccw(pt(0, 1)));
  EXPECT_EQ(pt(0, 1), rotate_270_ccw(pt(-1, 0)));
  EXPECT_EQ(pt(-1, 0), rotate_270_ccw(pt(0, -1)));

  EXPECT_EQ(pt(3, -2), rotate_270_ccw(pt(2, 3)));
  EXPECT_EQ(pt(-4, -3), rotate_270_ccw(pt(3, -4)));
  EXPECT_EQ(pt(5, 1), rotate_270_ccw(pt(-1, 5)));
  EXPECT_EQ(pt(-7, 4), rotate_270_ccw(pt(-4, -7)));
}

namespace cpl {
static std::ostream& operator<<(std::ostream& os, const point<double>& p) {
  return os << '(' << p.x << ", " << p.y << ')';
}
}

TEST_F(PointRotationTest, RotateCCWTest) {
  using pt = cpl::point<double>;
  using cpl::rotate_ccw;

  auto expect_equal = [&](const pt& expected, const pt& actual) {
    EXPECT_NEAR(expected.x, actual.x, 1e-6) << "Expected: " << expected
                                            << ", Actual: " << actual;
    EXPECT_NEAR(expected.y, actual.y, 1e-6) << "Expected: " << expected
                                            << ", Actual: " << actual;
  };

  const double pi = std::acos(-1);

  expect_equal(pt{-5, 0}, rotate_ccw(pt{5, 0}, pi));
  expect_equal(pt{0, -4}, rotate_ccw(pt{0, 4}, pi));
  expect_equal(pt{5, -6}, rotate_ccw(pt{-5, 6}, pi));

  expect_equal(pt{-5, 0}, rotate_ccw(pt{5, 0}, -pi));
  expect_equal(pt{0, -4}, rotate_ccw(pt{0, 4}, -pi));
  expect_equal(pt{5, -6}, rotate_ccw(pt{-5, 6}, -pi));

  expect_equal(pt{0, 5}, rotate_ccw(pt{5, 0}, pi / 2));
  expect_equal(pt{-4, 0}, rotate_ccw(pt{0, 4}, pi / 2));
  expect_equal(pt{-6, -5}, rotate_ccw(pt{-5, 6}, pi / 2));

  expect_equal(pt{0, -5}, rotate_ccw(pt{5, 0}, -pi / 2));
  expect_equal(pt{4, 0}, rotate_ccw(pt{0, 4}, -pi / 2));
  expect_equal(pt{6, 5}, rotate_ccw(pt{-5, 6}, -pi / 2));

  const pt p0{5, 0};
  const pt p30{5 * std::sqrt(0.75), 2.5};
  const pt p45{5 * std::sqrt(0.5), 5 * std::sqrt(0.5)};
  const pt p60{2.5, 5 * std::sqrt(0.75)};

  expect_equal(p30, rotate_ccw(p0, pi / 6));
  expect_equal(p45, rotate_ccw(p0, pi / 4));
  expect_equal(p60, rotate_ccw(p0, pi / 3));

  expect_equal(p0, rotate_ccw(p30, -pi / 6));
  expect_equal(p45, rotate_ccw(p30, pi / 12));
  expect_equal(p60, rotate_ccw(p30, pi / 6));

  expect_equal(p0, rotate_ccw(p45, -pi / 4));
  expect_equal(p30, rotate_ccw(p45, -pi / 12));
  expect_equal(p60, rotate_ccw(p45, pi / 12));

  expect_equal(p0, rotate_ccw(p60, -pi / 3));
  expect_equal(p30, rotate_ccw(p60, -pi / 6));
  expect_equal(p45, rotate_ccw(p60, -pi / 12));
}
