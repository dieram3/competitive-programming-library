//          Copyright Diego Ramírez July 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <djp/number_theory/basics.hpp>
#include <gtest/gtest.h>

#include <random>

#include <cstdint>
#include <cstdio>

namespace {

template <class Function> static inline void repeat(std::size_t n, Function f) {
  while (n--)
    f();
}

} // Anonymous namespace.

TEST(gcd, HandlesPositiveInput) {
  using djp::gcd;
  EXPECT_EQ(5, gcd(10, 5));
  EXPECT_EQ(1, gcd(10, 13));
  EXPECT_EQ(11, gcd(11, 22));
  EXPECT_EQ(15, gcd(15, 15));
}

TEST(gcd, HandlesZeroInput) {
  using djp::gcd;
  EXPECT_EQ(5, gcd(0, 5));
  EXPECT_EQ(10, gcd(10, 0));
}

TEST(lcm, HandlesPositiveInput) {
  using djp::lcm;
  EXPECT_EQ(10, lcm(2, 5));
  EXPECT_EQ(6, lcm(3, 6));
  EXPECT_EQ(21, lcm(7, 3));
  EXPECT_EQ(24, lcm(24, 24));
}

TEST(lcm, HandlesZeroInput) {
  using djp::lcm;
  EXPECT_EQ(0, lcm(0, 5));
  EXPECT_EQ(0, lcm(5, 0));
  EXPECT_EQ(0, lcm(0, 0));
}

TEST(ceil_div, WorksWell) {
  using djp::ceil_div;
  EXPECT_EQ(0, ceil_div(0, 1));
  EXPECT_EQ(0, ceil_div(0, 2));
  EXPECT_EQ(3, ceil_div(9, 3));
  EXPECT_EQ(4, ceil_div(10, 3));
  EXPECT_EQ(UINT64_MAX, ceil_div(UINT64_MAX, UINT64_C(1)));
  EXPECT_EQ(16, ceil_div(31, 2));
}

TEST(multiply_less, WorksWell) {
  using djp::multiply_less;
  EXPECT_FALSE(multiply_less(0, 0, 0));
  EXPECT_FALSE(multiply_less(0, 1, 0));
  EXPECT_FALSE(multiply_less(1, 0, 0));
  EXPECT_TRUE(multiply_less(0, 0, 1));
  EXPECT_TRUE(multiply_less(0, 1, 1));
  EXPECT_TRUE(multiply_less(1, 0, 1));
  EXPECT_FALSE(multiply_less(65536u, 65536u, 4294967295u));
  EXPECT_FALSE(multiply_less(3, 3, 9));
  EXPECT_TRUE(multiply_less(3, 3, 10));
  EXPECT_FALSE(multiply_less(4, 3, 10));
}

TEST(multiply_greater, WorksWell) {
  using djp::multiply_greater;
  EXPECT_FALSE(multiply_greater(0, 0, 0));
  EXPECT_FALSE(multiply_greater(0, 1, 0));
  EXPECT_FALSE(multiply_greater(1, 0, 0));
  EXPECT_FALSE(multiply_greater(0, 0, 1));
  EXPECT_FALSE(multiply_greater(0, 1, 1));
  EXPECT_FALSE(multiply_greater(1, 0, 1));
  EXPECT_TRUE(multiply_greater(65536u, 65536u, 4294967295u));
  EXPECT_FALSE(multiply_greater(3, 3, 9));
  EXPECT_FALSE(multiply_greater(3, 3, 10));
  EXPECT_TRUE(multiply_greater(4, 3, 10));
}

TEST(multiply_comparators, WorksWell) {
  using djp::multiply_greater;
  using djp::multiply_less;

  std::random_device rd;
  std::mt19937 gen(rd());

  // Pathological cases are difficult to find thereby many repetitions
  // are needed to check that multiply_greater works well.
  repeat(250000, [&gen] {
    std::uniform_int_distribution<uint32_t> dist(0, UINT16_MAX);
    const auto a = dist(gen);
    const auto b = dist(gen);
    const auto c = dist(gen);
    char info[128];
    std::sprintf(info, "a=%zu,b=%zu,c=%zu", size_t(a), size_t(b), size_t(c));
    EXPECT_EQ(a * b > c, multiply_greater(a, b, c)) << info;
    EXPECT_EQ(a * b < c, multiply_less(a, b, c)) << info;
  });
}
