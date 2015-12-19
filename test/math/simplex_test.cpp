//          Copyright Diego Ramírez August 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <djp/math/simplex.hpp>
#include <gtest/gtest.h>

#include <djp/utility/matrix.hpp>

#include <cassert>   // for assert
#include <cmath>     // for std::isfinite, std::isnan, std::fabs, NAN, INFINITY
#include <limits>    // for std::numeric_limits
#include <numeric>   // for std::inner_product
#include <stdexcept> // for std::domain_error
#include <vector>    // for std::vector, std::initializer_list

using namespace djp;
using real_t = double;
using vec_t = std::vector<real_t>;

static bool float_eq(const real_t x, const real_t y) {
  return std::fabs(x - y) < 1e-7;
}

static bool float_le(real_t x, real_t y) { return x <= y || float_eq(x, y); }

static bool float_ge(real_t x, real_t y) { return x >= y || float_eq(x, y); }

namespace {

class SimplexTest : public ::testing::Test {
  simplex_solver<real_t> solver;
  matrix2<real_t> A;
  vec_t b;
  vec_t c;
  vec_t x;

protected:
  void set_A(const size_t m, const size_t n, const vec_t &vec) {
    assert(vec.size() == m * n);
    A.resize(m, n);
    for (size_t i = 0; i != m; ++i)
      for (size_t j = 0; j != n; ++j)
        A[i][j] = vec[i * n + j];
  }

  void set_b(std::initializer_list<real_t> il) { b = il; }
  void set_c(std::initializer_list<real_t> il) { c = il; }

  void test_current_program(const real_t expected_opt_value,
                            const char *test_name) {

    SCOPED_TRACE(test_name);

    solver.set_eps(1e-11);
    const real_t opt_value = solver.maximize(A, b, c, x);

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
        sum += A[i][j] * x.at(j);
      EXPECT_TRUE(float_le(sum, b[i])) << "Constraint " << i
                                       << " is not satisfied\n" << sum
                                       << " !<= " << b[i];
    }

    // Check x >= 0
    for (size_t i = 0; i != x.size(); ++i)
      EXPECT_TRUE(float_ge(x[i], 0)) << "Variable " << i << " is negative: x["
                                     << i << "] = " << x[i];
  }
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

TEST_F(SimplexTest, EqualityRestrictionTest) {
  // Test taken from: http://web.stanford.edu/~liszt90/acm/notebook.html#file17
  set_A(4, 3, {
                  6, -1, 0,  // row 1
                  -1, -5, 0, // row 2
                  1, 5, 1,   // row 3
                  -1, -5, -1 // row 4
              });

  set_b({10, -4, 5, -5});
  set_c({1, -1, 0});

  // In this program: x + 5y + z = 5
  test_current_program(1.2903225806, "First test");
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

TEST_F(SimplexTest, AntiCyclingTest) {
  set_A(3, 4, {
                  0.5, -5.5, -2.5, 9.0, // row 1
                  0.5, -1.5, -0.5, 1.0, // row 2
                  1.0, 0.0, 0.0, 0.0    // row 3
              });
  set_b({0, 0, 1});
  set_c({10, -57, -9, -24});
  test_current_program(1, "Test 1");

  set_A(3, 4, {
                  1.0, 1.0, 1.0, 1.0,   // row 1
                  0.5, -5.5, -2.5, 9.0, // row 2
                  0.5, -1.5, -0.5, 1.0  // row 3
              });
  set_b({1, 0, 0});
  set_c({-1, 7, 1, 2});
  test_current_program(7, "Test 2");

  set_A(2, 4, {
                  0.4, 0.2, -1.4, -0.2, // row 1
                  7.8, 1.4, 7.8, 0.4    // row 2
              });
  set_b({0, 0});
  set_c({2.3, 2.15, -13.55, -0.4});
  test_current_program(0, "Test 3");

  set_A(3, 4, {
                  0.5, -5.5, -2.5, 9.0, // row 0
                  0.5, -1.5, -0.5, 1.0, // row 1
                  1.0, 0.0, 0.0, 0.0    // row 2
              });
  set_b({0, 0, 1});
  set_c({10, -57, -9, -24});
  test_current_program(1, "Test 4");
}

TEST_F(SimplexTest, GreaterThanTest) {
  set_A(3, 3, {
                  -1, -1, 0, // row 1
                  -1, 0, -1, // row2
                  0, -1, -1  // row 3
              });
  set_b({-5, -4, -3});
  set_c({-2, -2, -2});
  test_current_program(-12, "Test 1");

  set_A(3, 3, {
                  3, 2, 1,  // row 1
                  2, 5, 3,  // row 2
                  -1, -9, 1 // row 3
              });
  set_b({10, 15, -4});
  set_c({2, 3, 4});
  test_current_program(16.90625, "Test 2");

  set_A(5, 3, {
                  3, -1, -1, // row 1
                  1, 2, -1,  // row 2
                  2, 1, 0,   // row 3
                  1, 1, 0,   // row 4
                  -1, -1, 0  // row 5
              });
  set_b({-1, -2, 2, 1, -1});
  set_c({5, -1, -1});
  test_current_program(1, "Test 3");
}

TEST_F(SimplexTest, MinimizationTest) {
  set_A(3, 2, {
                  -3, -4, // row 1
                  -1, 0,  // row 2
                  0, -1,  // row 3
              });
  set_b({-6, -1, -2});
  set_c({-200, -150});
  test_current_program(-500, "Test 1");
}

TEST_F(SimplexTest, ArtificialVariableRemainsBasicTest) {
  set_A(2, 2, {
                  -1, -1, // row 1
                  1, 0    // row 2
              });
  set_b({-1, 0});
  set_c({-1, -1});

  test_current_program(-1, "Test 1");

  set_A(2, 2, {
                  2, 1,  // row 1
                  -1, -1 // row 2
              });
  set_b({1, -1});
  set_c({2, 2});
  test_current_program(2, "Test 3");

  set_A(3, 3, {
                  -2, -2, -2, // row 1
                  -2, -2, 1,  // row 2
                  2, 2, 1     // row 3
              });
  set_b({-2, 2, 1});
  set_c({2, 2, -1});
  test_current_program(-1, "Test 4");
}

//#include <chrono>
//#include <random>
// TEST_F(SimplexTest, Benchmark) {
//  simplex_solver<real_t> solver;
//  solver.set_eps(1e-10);
//  std::mt19937 engine;
//  std::uniform_real_distribution<real_t> gen_value;
//  std::bernoulli_distribution gen_bool(0.4);
//
//  const size_t m = 100;
//  const size_t n = 100;
//  real_t checksum = 0;
//  matrix2<real_t> A(m, n);
//  vec_t b(m), c(n), x;
//
//  auto run_test = [&] {
//    for (size_t i = 0; i < m; ++i)
//      for (size_t j = 0; j < n; ++j)
//        if (gen_bool(engine))
//          A[i][j] = gen_value(engine);
//
//    for (size_t i = 0; i < m; ++i)
//      b[i] = gen_value(engine) * n;
//
//    for (size_t j = 0; j < n; ++j)
//      c[j] = gen_value(engine) * 5;
//
//    checksum += solver.maximize(A, b, c, x);
//  };
//
//  using namespace std::chrono;
//  const auto begin = steady_clock::now();
//  for (size_t rep = 0; rep < 200; ++rep)
//    run_test();
//  const auto end = steady_clock::now();
//  const auto elapsed = duration_cast<milliseconds>(end - begin);
//  std::cout << "Elapsed time: " << elapsed.count() << " ms\n";
//  std::cout << "Checksum: " << checksum << '\n';
//}
