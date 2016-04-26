//          Copyright Diego Ramirez 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cpl/combinatorics/binomial_coefficient.hpp>
#include <gtest/gtest.h>

#include <cpl/utility/matrix.hpp> // matrix
#include <cstddef>                // size_t
#include <cstdint>                // uint32_t, uint64_t

using cpl::gen_binomial_coefficient_matrix;
using std::size_t;
using std::uint32_t;
using std::uint64_t;

TEST(BinomialCoefficientTest, SimpleTest) {
  const auto dp = gen_binomial_coefficient_matrix(10);
  auto C = [&](size_t n, size_t k) { return dp[{n, k}]; };
  EXPECT_EQ(1, C(0, 0));
  EXPECT_EQ(1, C(1, 0));
  EXPECT_EQ(1, C(1, 1));
  EXPECT_EQ(1, C(2, 0));
  EXPECT_EQ(2, C(2, 1));
  EXPECT_EQ(1, C(2, 2));
  EXPECT_EQ(1, C(3, 0));
  EXPECT_EQ(3, C(3, 1));
  EXPECT_EQ(3, C(3, 2));
  EXPECT_EQ(1, C(3, 3));
  EXPECT_EQ(4, C(4, 1));
  EXPECT_EQ(6, C(4, 2));
  EXPECT_EQ(4, C(4, 3));
  EXPECT_EQ(10, C(5, 2));
  EXPECT_EQ(10, C(5, 3));
  EXPECT_EQ(5, C(5, 4));
  EXPECT_EQ(210, C(10, 4));
  EXPECT_EQ(252, C(10, 5));
  EXPECT_EQ(210, C(10, 6));
}

TEST(BinomialCoefficientTest, Uint32Test) {
  const auto dp = gen_binomial_coefficient_matrix<uint32_t>(34);
  auto C = [&](size_t n, size_t k) { return dp[{n, k}]; };
  EXPECT_EQ(278256u, C(34, 5));
  EXPECT_EQ(131128140u, C(34, 10));
  EXPECT_EQ(2203961430u, C(34, 16));
  EXPECT_EQ(2333606220u, C(34, 17));
  EXPECT_EQ(2203961430u, C(34, 18));
  EXPECT_EQ(5379616u, C(34, 27));
  EXPECT_EQ(561u, C(34, 32));
}

TEST(BinomialCoefficientTest, Uint64Test) {
  const auto dp = gen_binomial_coefficient_matrix<uint64_t>(67);
  auto C = [&](size_t n, size_t k) { return dp[{n, k}]; };
  EXPECT_EQ(9364899127970100u, C(67, 18));
  EXPECT_EQ(9989690752182277136u, C(67, 30));
  EXPECT_EQ(14226520737620288370u, C(67, 33));
  EXPECT_EQ(14226520737620288370u, C(67, 34));
  EXPECT_EQ(57963796707857040u, C(67, 47));
  EXPECT_EQ(6522361560, C(67, 59));
}
