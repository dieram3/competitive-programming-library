//          Copyright Diego Ramírez September 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cpl/math/bisection.hpp>
#include <gtest/gtest.h>

#include <limits>      // For std::numeric_limits
#include <type_traits> // For std::is_floating_point
#include <cassert>     // For assert
#include <cstddef>     // For std::size_t
#include <cmath>       // For fabs, fmin, fmax, sin, acos

using namespace cpl;
using std::fmin;
using std::fmax;
using std::fabs;

TEST(BisectTest, SquareRootTest) {
  auto sqrt = [](double x) {
    assert(x >= 0);
    auto func = [x](double y) { return y * y - x; };
    const double tol = std::numeric_limits<double>::epsilon();
    const size_t max_iter = 150;
    return bisect(func, 0.0, fmax(1.0, x), tol, max_iter);
  };

  EXPECT_FLOAT_EQ(0.0, sqrt(0.0));
  EXPECT_FLOAT_EQ(1.0, sqrt(1.0));
  EXPECT_FLOAT_EQ(3.0, sqrt(9));
  EXPECT_FLOAT_EQ(4.0, sqrt(16));
  EXPECT_FLOAT_EQ(9.0, sqrt(81));
  EXPECT_FLOAT_EQ(11.0, sqrt(121));

  EXPECT_FLOAT_EQ(1.414213562, sqrt(2));
  EXPECT_FLOAT_EQ(1.73205080757, sqrt(3));
  EXPECT_FLOAT_EQ(1109.63867993, sqrt(1231298));
  EXPECT_FLOAT_EQ(94.4040422757, sqrt(8912.123198));
  EXPECT_FLOAT_EQ(2287.23042127, sqrt(5231423));

  EXPECT_FLOAT_EQ(0.316227766017, sqrt(0.1));
  EXPECT_FLOAT_EQ(0.1, sqrt(0.01));
  EXPECT_FLOAT_EQ(0.0350856095857, sqrt(0.001231));
}

TEST(BisectTest, CubeRootTest) {
  auto cbrt = [](double x) {
    auto func = [x](double y) { return y * y * y - x; };
    const double tol = std::numeric_limits<double>::epsilon();
    const size_t max_iter = 150;
    const double x_min = fmin(-1.0, -fabs(x));
    const double x_max = fmax(1.0, fabs(x));
    return bisect(func, x_min, x_max, tol, max_iter);
  };

  EXPECT_FLOAT_EQ(0.0, cbrt(0));
  EXPECT_FLOAT_EQ(+1.0, cbrt(+1));
  EXPECT_FLOAT_EQ(-1.0, cbrt(-1));
  EXPECT_FLOAT_EQ(+2.0, cbrt(+8));
  EXPECT_FLOAT_EQ(-2.0, cbrt(-8));

  EXPECT_FLOAT_EQ(933.078494101, cbrt(812371239));
  EXPECT_FLOAT_EQ(43.397045887, cbrt(81729.812389));

  EXPECT_FLOAT_EQ(-0.231446460756, cbrt(-0.012398));
  EXPECT_FLOAT_EQ(0.992652817783, cbrt(0.97812));
}

TEST(BisectTest, SineTest) {

  const auto pi = std::acos(-1);

  auto func = [](double x) { return std::sin(x); };
  const auto x_min = -1.4 * pi;
  const auto x_max = 1.8 * pi;
  ASSERT_LT(0.0, func(x_min));
  ASSERT_GT(0.0, func(x_max));

  const double tol = std::numeric_limits<double>::epsilon();
  const auto root = bisect(func, x_min, x_max, tol, 100);
  EXPECT_GT(root, x_min);
  EXPECT_LT(root, x_max);

  // There exists three roots but only one is found.
  EXPECT_LE(std::sin(root), std::numeric_limits<double>::epsilon());

  EXPECT_EQ(pi, bisect(func, 0.5 * pi, 1.5 * pi, tol, 100));
  EXPECT_EQ(-pi, bisect(func, -1.5 * pi, -0.5 * pi, tol, 100));
}

TEST(BisectTest, ThrowsIfMaxIterationsIsExceeded) {
  auto f = [](double x) { return x; };
  EXPECT_THROW(bisect(f, -1.0, 1.0, 1e-10, 0), std::runtime_error);
}
