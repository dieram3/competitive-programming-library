//          Copyright Diego Ramírez July 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <djp/number_theory/basics.hpp>
#include <gtest/gtest.h>

#include <djp/utility/basics.hpp> // for djp::repeat
#include <random>
#include <cstdint>
#include <cstdio>

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

  std::mt19937 gen;

  // Pathological cases are difficult to find thereby many repetitions
  // are needed to check that multiply_greater works well.
  djp::repeat(12345, [&gen] {
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
