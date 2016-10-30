//          Copyright Diego Ramirez 2016
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cpl/geometry/rotations.hpp>
#include <gtest/gtest.h>

#include <cpl/geometry/point_2d.hpp> // point

namespace {
class RotationsTest : public ::testing::Test {
protected:
  using vec = cpl::vector2d<int>;
};
} // end anonymous namespace

TEST_F(RotationsTest, Rotate90CCWTest) {
  using cpl::rotate_90_ccw;

  EXPECT_EQ(vec(0, 0), rotate_90_ccw(vec(0, 0)));

  EXPECT_EQ(vec(0, 1), rotate_90_ccw(vec(1, 0)));
  EXPECT_EQ(vec(-1, 0), rotate_90_ccw(vec(0, 1)));
  EXPECT_EQ(vec(0, -1), rotate_90_ccw(vec(-1, 0)));
  EXPECT_EQ(vec(1, 0), rotate_90_ccw(vec(0, -1)));

  EXPECT_EQ(vec(-3, 2), rotate_90_ccw(vec(2, 3)));
  EXPECT_EQ(vec(4, 3), rotate_90_ccw(vec(3, -4)));
  EXPECT_EQ(vec(-5, -1), rotate_90_ccw(vec(-1, 5)));
  EXPECT_EQ(vec(7, -4), rotate_90_ccw(vec(-4, -7)));
}

TEST_F(RotationsTest, Rotate180CCWTest) {
  using cpl::rotate_180_ccw;

  EXPECT_EQ(vec(0, 0), rotate_180_ccw(vec(0, 0)));

  EXPECT_EQ(vec(-1, 0), rotate_180_ccw(vec(1, 0)));
  EXPECT_EQ(vec(0, -1), rotate_180_ccw(vec(0, 1)));
  EXPECT_EQ(vec(1, 0), rotate_180_ccw(vec(-1, 0)));
  EXPECT_EQ(vec(0, 1), rotate_180_ccw(vec(0, -1)));

  EXPECT_EQ(vec(-2, -3), rotate_180_ccw(vec(2, 3)));
  EXPECT_EQ(vec(-3, 4), rotate_180_ccw(vec(3, -4)));
  EXPECT_EQ(vec(1, -5), rotate_180_ccw(vec(-1, 5)));
  EXPECT_EQ(vec(4, 7), rotate_180_ccw(vec(-4, -7)));
}

TEST_F(RotationsTest, Rotate270CCWTest) {
  using cpl::rotate_270_ccw;

  EXPECT_EQ(vec(0, 0), rotate_270_ccw(vec(0, 0)));

  EXPECT_EQ(vec(0, -1), rotate_270_ccw(vec(1, 0)));
  EXPECT_EQ(vec(1, 0), rotate_270_ccw(vec(0, 1)));
  EXPECT_EQ(vec(0, 1), rotate_270_ccw(vec(-1, 0)));
  EXPECT_EQ(vec(-1, 0), rotate_270_ccw(vec(0, -1)));

  EXPECT_EQ(vec(3, -2), rotate_270_ccw(vec(2, 3)));
  EXPECT_EQ(vec(-4, -3), rotate_270_ccw(vec(3, -4)));
  EXPECT_EQ(vec(5, 1), rotate_270_ccw(vec(-1, 5)));
  EXPECT_EQ(vec(-7, 4), rotate_270_ccw(vec(-4, -7)));
}

TEST_F(RotationsTest, RotateCCWTest) {
  using vecf = cpl::vector2d<double>;
  using cpl::rotate_ccw;

  auto expect_equal = [&](const vecf& expected, const vecf& actual) {
    ASSERT_NEAR(expected.x, actual.x, 1e-6) << "Expected: " << expected
                                            << ", Actual: " << actual;
    ASSERT_NEAR(expected.y, actual.y, 1e-6) << "Expected: " << expected
                                            << ", Actual: " << actual;
  };

  const double pi = std::acos(-1);

  expect_equal(vecf{-5, 0}, rotate_ccw(vecf{5, 0}, pi));
  expect_equal(vecf{0, -4}, rotate_ccw(vecf{0, 4}, pi));
  expect_equal(vecf{5, -6}, rotate_ccw(vecf{-5, 6}, pi));

  expect_equal(vecf{-5, 0}, rotate_ccw(vecf{5, 0}, -pi));
  expect_equal(vecf{0, -4}, rotate_ccw(vecf{0, 4}, -pi));
  expect_equal(vecf{5, -6}, rotate_ccw(vecf{-5, 6}, -pi));

  expect_equal(vecf{0, 5}, rotate_ccw(vecf{5, 0}, pi / 2));
  expect_equal(vecf{-4, 0}, rotate_ccw(vecf{0, 4}, pi / 2));
  expect_equal(vecf{-6, -5}, rotate_ccw(vecf{-5, 6}, pi / 2));

  expect_equal(vecf{0, -5}, rotate_ccw(vecf{5, 0}, -pi / 2));
  expect_equal(vecf{4, 0}, rotate_ccw(vecf{0, 4}, -pi / 2));
  expect_equal(vecf{6, 5}, rotate_ccw(vecf{-5, 6}, -pi / 2));

  const vecf v0{5, 0};
  const vecf v30{5 * std::sqrt(0.75), 2.5};
  const vecf v45{5 * std::sqrt(0.5), 5 * std::sqrt(0.5)};
  const vecf v60{2.5, 5 * std::sqrt(0.75)};

  expect_equal(v30, rotate_ccw(v0, pi / 6));
  expect_equal(v45, rotate_ccw(v0, pi / 4));
  expect_equal(v60, rotate_ccw(v0, pi / 3));

  expect_equal(v0, rotate_ccw(v30, -pi / 6));
  expect_equal(v45, rotate_ccw(v30, pi / 12));
  expect_equal(v60, rotate_ccw(v30, pi / 6));

  expect_equal(v0, rotate_ccw(v45, -pi / 4));
  expect_equal(v30, rotate_ccw(v45, -pi / 12));
  expect_equal(v60, rotate_ccw(v45, pi / 12));

  expect_equal(v0, rotate_ccw(v60, -pi / 3));
  expect_equal(v30, rotate_ccw(v60, -pi / 6));
  expect_equal(v45, rotate_ccw(v60, -pi / 12));
}
