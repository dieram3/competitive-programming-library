//          Copyright Diego Ramírez August 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <djp/numerical/simplex.hpp>
#include <gtest/gtest.h>

#include <djp/utility/matrix.hpp>

#include <numeric> // for std::inner_product
#include <vector>  // for std::vector
#include <cassert> // for assert
#include <cmath>   // fors std::is_finite

using namespace djp;
using real_t = double;
using vec_t = std::vector<real_t>;
static constexpr auto inf = std::numeric_limits<real_t>::infinity();

namespace {

// Results have been verified in:
// www.easycalculation.com/operations-research/simplex-method-calculator.php
class SimplexTest : public ::testing::Test {
protected:
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

  void check_solution(const real_t expected) {
    const auto opt_value = simplex_maximize(A, b, c, x);

    EXPECT_FLOAT_EQ(expected, opt_value);
    if (!std::isfinite(opt_value))
      return;
    ASSERT_EQ(A.rows(), x.size());
    ASSERT_EQ(x.size(), c.size());
    const auto recomputed_opt_value =
        std::inner_product(c.begin(), c.end(), x.begin(), real_t{0});
    EXPECT_FLOAT_EQ(opt_value, recomputed_opt_value);

    for (size_t i = 0; i != A.rows(); ++i) {
      real_t sum = 0;
      for (size_t j = 0; j != A.cols(); ++j)
        sum += A[{i, j}] * x.at(j);
      EXPECT_LE(sum, b[i]) << "Constraint " << i << " is violated.\n";
    }
  }
};

} // end anonymous namespace

TEST_F(SimplexTest, OneVariableTest) {
  set_A(1, 1, {2});
  set_b({5});
  set_c({2});
  check_solution(5);
}

TEST_F(SimplexTest, UnboundedSolutionTest) {
  {
    SCOPED_TRACE("First test");
    set_A(1, 2, {1, -1});
    set_b({-5});
    set_c({1, 1});
    check_solution(inf);
  }
  {
    SCOPED_TRACE("Second test");
    set_A(1, 2, {2, 0});
    set_b({6});
    set_c({-1, 1});
    check_solution(inf);
  }
}

TEST_F(SimplexTest, SimpleTest) {
  set_A(3, 3, {
                  1, 2, 3, // row 1
                  0, 1, 4, // row 2
                  3, 2, 0, // row 3
              });
  set_b({8, 5, 12});
  {
    SCOPED_TRACE("First test");
    set_c({2, 3, 4});
    check_solution(13.28571428571);
  }
  {
    SCOPED_TRACE("Second test");
    set_c({1, 2, 3});
    check_solution(8);
  }
}
