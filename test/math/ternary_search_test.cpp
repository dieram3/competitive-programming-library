//          Copyright Diego Ramirez 2016
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cpl/math/ternary_search.hpp>
#include <gtest/gtest.h>

#include <limits> // numeric_limits

namespace {
class TernarySearchTest : public ::testing::Test {
protected:
  static constexpr double tol = 1e-07;

  template <typename F, typename T>
  static T find_maximum(F f, T a, T b, int max_iter = 200) {
    return cpl::ternary_search(f, a, b, tol, max_iter);
  }

  template <typename F, typename T>
  static T find_minimum(F f, T a, T b, int max_iter = 200) {
    auto nf = [f](double x) { return -f(x); };
    return find_maximum(nf, a, b, max_iter);
  }
};
} // end anonymous namespace

TEST_F(TernarySearchTest, QuadraticFunctionsTest) {
  {
    auto f = [](double x) { return -(x * x); };
    EXPECT_NEAR(0.0, find_maximum(f, -10.0, 10.0), tol);
    EXPECT_NEAR(0.0, find_maximum(f, -0.4, 1400.0), tol);
    EXPECT_NEAR(0.0, find_maximum(f, -1242.0, 42.0), tol);
    EXPECT_NEAR(0.0, find_maximum(f, -1e37, 0.00005, 500), tol);
  }
  {
    auto f = [](double x) { return -(2 * x * x) + 7 * x - 3; };
    const double x_max = 1.75;
    EXPECT_NEAR(x_max, find_maximum(f, 1.5, 2.0), tol);
    EXPECT_NEAR(x_max, find_maximum(f, 1.7, 3.6), tol);
    EXPECT_NEAR(x_max, find_maximum(f, -5000.0, 20000.0), tol);
    EXPECT_NEAR(x_max, find_maximum(f, -400.0, 1.76), tol);
    EXPECT_NEAR(x_max, find_maximum(f, -1e15, 1e15), tol);
    EXPECT_NEAR(x_max, find_maximum(f, 1.74, 1e15), tol);
    EXPECT_NEAR(x_max, find_maximum(f, 1.75, 1e50, 500), tol);
    EXPECT_NEAR(x_max, find_maximum(f, 1.75, 1.75, 1), 0.0);
  }
  {
    auto f = [](double x) { return 9 * x * x + 7 * x + 42; };
    const double x_min = -7.0 / 18.0; // -0.3888...
    EXPECT_NEAR(x_min, find_minimum(f, -0.4, -0.38), tol);
    EXPECT_NEAR(x_min, find_minimum(f, -5.0, 10.0), tol);
    EXPECT_NEAR(x_min, find_minimum(f, -1e08, 12345.67), tol);
    EXPECT_NEAR(x_min, find_minimum(f, -1e42, x_min + 0.001, 500), tol);
    EXPECT_NEAR(x_min, find_minimum(f, x_min, 5000.0), tol);
    EXPECT_NEAR(x_min, find_minimum(f, x_min, 1e30, 500), tol);
    EXPECT_NEAR(x_min, find_minimum(f, -1.0, x_min), tol);
    EXPECT_NEAR(x_min, find_minimum(f, -1.0, 0.0), tol);
  }
}
