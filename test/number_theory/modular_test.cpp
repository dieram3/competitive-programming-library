//          Copyright Diego Ramírez November 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <djp/number_theory/modular.hpp>
#include <gtest/gtest.h>

TEST(modular, HandlesArithmeticOps) {
  using modular_t = djp::modular<unsigned, 7>;

  const modular_t a = 4;
  const modular_t b = 12;

  EXPECT_TRUE(congruent(a, 4));
  EXPECT_TRUE(congruent(b, 5));
  EXPECT_TRUE(congruent(a + b, 2));
  EXPECT_TRUE(congruent(a - b, 6));
  EXPECT_TRUE(congruent(b - a, 1));
  EXPECT_TRUE(congruent(a * b, 6));
  EXPECT_TRUE(congruent(pow(a, 7 - 2), 2));
  EXPECT_TRUE(congruent(pow(b, 7 - 2), 3));
  EXPECT_TRUE(congruent(inverse(a), 2));
  EXPECT_TRUE(congruent(inverse(b), 3));
  EXPECT_TRUE(congruent(a / b, 5));
  EXPECT_TRUE(congruent(b / a, 3));
  EXPECT_EQ(4, static_cast<int>(a));
  EXPECT_EQ(5, static_cast<int>(b));
}
