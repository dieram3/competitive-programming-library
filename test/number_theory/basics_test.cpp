//          Copyright Diego Ramírez July 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <djp/number_theory/basics.hpp>
#include <gtest/gtest.h>

#include <djp/utility/basics.hpp> // for djp::repeat
#include <random>  // for std::mt19937, std::uniform_int_distribution
#include <cstdint> // for std::uint32_t, std::uint_fast64_t
#include <cstdio>  // for std::sprintf

using namespace djp;

TEST(CeilDivTest, WorksWell) {
  EXPECT_EQ(0, ceil_div(0, 1));
  EXPECT_EQ(0, ceil_div(0, 2));
  EXPECT_EQ(3, ceil_div(9, 3));
  EXPECT_EQ(4, ceil_div(10, 3));
  EXPECT_EQ(UINT64_MAX, ceil_div(UINT64_MAX, UINT64_C(1)));
  EXPECT_EQ(16, ceil_div(31, 2));
}

TEST(MultiplyLessTest, WorksWell) {
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

TEST(MultiplyGreaterTest, WorksWell) {
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

TEST(MultiplyComparatorsTest, WorksWell) {

  std::mt19937 gen;

  // Pathological cases are difficult to find thereby many repetitions
  // are needed to check that multiply_greater works well.
  repeat(12345, [&gen] {
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

TEST(IPowTest, WorksWell) {
  using int_t = std::uint_fast64_t;
  EXPECT_EQ(1, ipow<int_t>(1, 0));
  EXPECT_EQ(1, ipow<int_t>(2, 0));
  EXPECT_EQ(1, ipow<int_t>(29, 0));
  EXPECT_EQ(29, ipow<int_t>(29, 1));
  EXPECT_EQ(841, ipow<int_t>(29, 2));
  EXPECT_EQ(129140163, ipow<int_t>(3, 17));
  EXPECT_EQ(476837158203125, ipow<int_t>(5, 21));
  EXPECT_EQ(819628286980801, ipow<int_t>(31, 10));

  // Special cases
  EXPECT_EQ(1, ipow(0, 0));
  EXPECT_EQ(0, ipow(0, 1));
  EXPECT_EQ(0, ipow(0, 2));
}
