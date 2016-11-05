//          Copyright Diego Ramirez 2016
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cpl/math/ternary_search.hpp>
#include <gtest/gtest.h>

#include <cassert> // assert
#include <cmath>   // log, ceil

using cpl::ternary_search;

namespace {
class TernarySearchTest : public ::testing::Test {
protected:
  template <typename F, typename T>
  T find_maximum(F f, T a, T b) {
    const auto tol = get_tol();
    const auto max_iter = compute_number_of_iterations(a, b, tol) + 10;

    return ternary_search(f, a, b, tol, max_iter);
  }

  template <typename F, typename T>
  T find_minimum(F f, T a, T b) {
    auto nf = [f](double x) { return -f(x); };
    return find_maximum(nf, a, b);
  }

  void set_tol(double value) {
    assert(value > 0);
    m_tol = value;
  }

  double get_tol() const {
    return m_tol;
  }

private:
  static int compute_number_of_iterations(double a, double b, double tol) {

    if ((b - a) <= 2 * tol) {
      return 0;
    }

    const auto ans = std::ceil(std::log((b - a) / (2 * tol)) / std::log(1.5));

    assert(ans >= 1);
    assert(ans < 1000); // Otherwise, the test is too demanding
    return static_cast<int>(ans);
  }

  double m_tol = 1e-08;
};
} // end anonymous namespace

TEST_F(TernarySearchTest, SquaredFunctionTest) {
  auto f = [](double x) { return -(x * x); };
  const double x_max = 0.0;

  // This function is very precise for any x as there is no sum.
  const double tol = 1e-50;
  set_tol(tol);

  EXPECT_NEAR(x_max, find_maximum(f, -10.0, 10.0), tol);
  EXPECT_NEAR(x_max, find_maximum(f, -0.4, 1400.0), tol);
  EXPECT_NEAR(x_max, find_maximum(f, -1242.0, 42.0), tol);
  EXPECT_NEAR(x_max, find_maximum(f, -1e37, 0.00005), tol);
}

TEST_F(TernarySearchTest, QuadraticFunctionWithSmallConstantTermTest) {
  auto f = [](double x) { return -(2 * x * x) + 7 * x - 3; };
  const double x_max = 1.75;

  // This function is imprecise in some points.
  const double tol = 1e-08;
  set_tol(tol);

  EXPECT_NEAR(x_max, find_maximum(f, 1.5, 2.0), tol);
  EXPECT_NEAR(x_max, find_maximum(f, 1.7, 3.6), tol);
  EXPECT_NEAR(x_max, find_maximum(f, -5000.0, 20000.0), tol);
  EXPECT_NEAR(x_max, find_maximum(f, -400.0, 1.76), tol);
  EXPECT_NEAR(x_max, find_maximum(f, -1e15, 1e15), tol);
  EXPECT_NEAR(x_max, find_maximum(f, 1.74, 1e15), tol);
  EXPECT_NEAR(x_max, find_maximum(f, 1.75, 1e50), tol);
  EXPECT_NEAR(x_max, find_maximum(f, 1.75, 1.75), 0.0);
}

TEST_F(TernarySearchTest, QuadraticFunctionWithLargeConstantTermTest) {
  auto f = [](double x) { return 9 * x * x + 7 * x + 42; };
  const double x_min = -7.0 / 18.0; // -0.3888...

  // The +42 makes f imprecise when x is small.
  const double tol = 1e-07;
  set_tol(tol);

  EXPECT_NEAR(x_min, find_minimum(f, -0.4, -0.38), tol);
  EXPECT_NEAR(x_min, find_minimum(f, -5.0, 10.0), tol);
  EXPECT_NEAR(x_min, find_minimum(f, -1e08, 12345.67), tol);
  EXPECT_NEAR(x_min, find_minimum(f, -1e42, x_min + 0.001), tol);
  EXPECT_NEAR(x_min, find_minimum(f, x_min, 5000.0), tol);
  EXPECT_NEAR(x_min, find_minimum(f, x_min, 1e30), tol);
  EXPECT_NEAR(x_min, find_minimum(f, -1.0, x_min), tol);
  EXPECT_NEAR(x_min, find_minimum(f, -1.0, 0.0), tol);
}
