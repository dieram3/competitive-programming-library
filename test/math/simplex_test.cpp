//          Copyright Diego Ramírez August 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <djp/math/simplex.hpp>
#include <gtest/gtest.h>

#include <djp/utility/matrix.hpp>

#include <limits>    // for std::numeric_limits
#include <numeric>   // for std::inner_product
#include <stdexcept> // for std::domain_error
#include <vector>    // for std::vector, std::initializer_list
#include <cassert>   // for assert
#include <cmath>     // for std::isfinite, std::isnan, std::fabs, NAN, INFINITY

using namespace djp;
using real_t = double;
using vec_t = std::vector<real_t>;

static bool float_eq(const real_t x, const real_t y) {
  return std::fabs(x - y) < 1e-7;
}

static bool float_le(real_t x, real_t y) { return x <= y || float_eq(x, y); }

static bool float_ge(real_t x, real_t y) { return x >= y || float_eq(x, y); }

namespace {

// Results have been verified in:
// http://www.zweigmedia.com/RealWorld/simplex.html
// Note that the implementation of this page does not detect unfeasible
// programs.
//
class SimplexTest : public ::testing::Test {
  matrix<real_t> A = matrix<real_t>({0, 0});
  vec_t b;
  vec_t c;
  vec_t x;

protected:
  void set_A(const size_t m, const size_t n, const vec_t &vec) {
    assert(vec.size() == m * n);
    A.assign({m, n});
    for (size_t i = 0; i != m; ++i)
      for (size_t j = 0; j != n; ++j)
        A[{i, j}] = vec[i * n + j];
  }

  void set_b(std::initializer_list<real_t> il) { b = il; }
  void set_c(std::initializer_list<real_t> il) { c = il; }

  void test_current_program(const real_t expected_opt_value,
                            const char *test_name, bool may_throw = false) {

    SCOPED_TRACE(test_name);

    real_t opt_value;
    try {
      opt_value = simplex_maximize(A, b, c, x);
    } catch (std::domain_error &) {
      EXPECT_TRUE(may_throw) << " Unexpected thrown exception\n";
      return;
    }

    if (std::isnan(expected_opt_value))
      EXPECT_TRUE(std::isnan(opt_value));
    else
      EXPECT_FLOAT_EQ(expected_opt_value, opt_value);
    if (!std::isfinite(opt_value))
      return;

    ASSERT_EQ(A.rows(), b.size());
    ASSERT_EQ(A.cols(), x.size());
    ASSERT_EQ(x.size(), c.size());
    const auto recomputed_opt_value =
        std::inner_product(c.begin(), c.end(), x.begin(), real_t{0});
    EXPECT_FLOAT_EQ(expected_opt_value, recomputed_opt_value);

    // Check A*x <= b
    for (size_t i = 0; i != A.rows(); ++i) {
      real_t sum = 0;
      for (size_t j = 0; j != A.cols(); ++j)
        sum += A[{i, j}] * x.at(j);
      EXPECT_TRUE(float_le(sum, b[i])) << "Constraint " << i
                                       << " is not satisfied\n" << sum
                                       << " !<= " << b[i];
    }

    // Check x >= 0
    for (size_t i = 0; i != x.size(); ++i)
      EXPECT_TRUE(float_ge(x[i], 0)) << "Variable " << i << " is negative";
  }

  //  void print_current_program() {
  //    std::cout << "Maximize p = ";
  //    std::cout << c[0] << "x0 ";
  //    std::cout << std::showpos;
  //    for (size_t j = 1; j != c.size(); ++j)
  //      std::cout << " " << c[j] << "x" << j;
  //    std::cout << " subject to\n";
  //    for (size_t i = 0; i != A.rows(); ++i) {
  //      std::cout << std::noshowpos << A[{i, 0}] << "x0 ";
  //      std::cout << std::showpos;
  //      for (size_t j = 1; j != A.cols(); ++j)
  //        std::cout << A[{i, j}] << "x" << j << " ";
  //      std::cout << std::noshowpos;
  //      std::cout << "<= " << b[i] << '\n';
  //    }
  //  }
};

} // end anonymous namespace

TEST_F(SimplexTest, OneVariableTest) {
  set_A(1, 1, {2});
  set_b({5});
  set_c({2});
  test_current_program(5, "First test");
}

TEST_F(SimplexTest, UnboundedSolutionTest) {
  set_A(1, 2, {1, -1});
  set_b({-5});
  set_c({1, 1});
  test_current_program(INFINITY, "First test");

  set_A(1, 2, {2, 0});
  set_b({6});
  set_c({-1, 1});
  test_current_program(INFINITY, "Second test");
}

TEST_F(SimplexTest, SimpleTest) {
  set_A(3, 3, {
                  1, 2, 3, // row 1
                  0, 1, 4, // row 2
                  3, 2, 0, // row 3
              });
  set_b({8, 5, 12});

  set_c({2, 3, 4});
  test_current_program(13.28571428571, "First test");

  set_c({1, 2, 3});
  test_current_program(8, "Second test");
}

TEST_F(SimplexTest, ComplexTest) {
  set_A(10, 7, {
                   19, 33, 81, 1, 23, 13, -12,        // row 1
                   32, 23, 12, 99, -2, 42, 3,         // row 2
                   91, 12, -23, 23, 18, 18, 4,        // row 3
                   18, 14, 41, 82, 23, 82, 19,        // row 4
                   34, 98, -5, 18, 82, 92, 23,        // row 5
                   123, 40, 71, 23, 89, 38, 123,      // row 6
                   198, 28, -42, 28, 238, 82, 812,    // row 7
                   98, 239, -123, 89, 23, 28, 192,    // row 8
                   -12, -124, -23, -48, -12, -89, 12, // row 9
                   18, 124, 88, 89, 218, 32, 223      // row 10
               });
  set_b({1412, 12319, 9812, 8912, 8723, 1928, 8912, 8322, -1298, 47818});

  set_c({139, 89, 182, 138, 123, -123, 0});

  test_current_program(11568, "Complex test");
}

TEST_F(SimplexTest, SparseMatrixTest) {
  set_A(3, 5, {
                  92, 0, 0, 0, 29, // row 1
                  0, 0, 0, 23, 0,  // row 2
                  32, 12, 23, 0, 0 // row 3
              });

  set_b({1122, 9128, 1239});
  set_c({19, 92, 89, 12, 98});

  test_current_program(18053.02098951, "SparseMatrixTest");
}

TEST_F(SimplexTest, NoFeasibleSolutionTest) {
  set_A(3, 2, {
                  2, 2,  // row 1
                  -1, 3, // row 2
                  1, 0   // row 3
              });

  set_b({-5, 4, 36});
  set_c({3, 2});
  test_current_program(NAN, "Trivial test case");

  set_A(4, 2, {
                  -2, -4, // row 1
                  1, 0,   // row 2
                  0, 1,   // row 3
                  3, 3    // row 4
              });
  set_b({-20, 10, 6, 12});
  set_c({3, 3});
  test_current_program(NAN, "Non-trivial test case");
}

TEST_F(SimplexTest, PossibleInfiniteLoopTest) {
  // From http://www.math.toronto.edu/mpugh/Teaching/APM236_04/bland
  set_A(3, 4, {
                  0.5, -5.5, -2.5, 9.0, // row 1
                  0.5, -1.5, -0.5, 1.0, // row 2
                  1.0, 0.0, 0.0, 0.0    // row 3
              });
  set_b({0, 0, 1});
  set_c({10, -57, -9, -24});
  test_current_program(1, "Test 1", true);

  set_A(3, 4, {
                  1.0, 1.0, 1.0, 1.0,   // row 1
                  0.5, -5.5, -2.5, 9.0, // row 2
                  0.5, -1.5, -0.5, 1.0  // row 3
              });
  set_b({1, 0, 0});
  set_c({-1, 7, 1, 2});
  test_current_program(7, "Test 2", true);
}
