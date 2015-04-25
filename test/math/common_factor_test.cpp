//          Copyright Diego Ramírez July 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <djp/math/common_factor.hpp>
#include <gtest/gtest.h>

using djp::gcd;
using djp::lcm;

TEST(gcd_test, HandlesPositiveInput) {
  EXPECT_EQ(5, gcd(10, 5));
  EXPECT_EQ(1, gcd(10, 13));
  EXPECT_EQ(11, gcd(11, 22));
  EXPECT_EQ(15, gcd(15, 15));
}

TEST(gcd_test, HandlesZeroInput) {
  EXPECT_EQ(5, gcd(0, 5));
  EXPECT_EQ(10, gcd(10, 0));
}

TEST(lcm_test, HandlesPositiveInput) {
  EXPECT_EQ(10, lcm(2, 5));
  EXPECT_EQ(6, lcm(3, 6));
  EXPECT_EQ(21, lcm(7, 3));
  EXPECT_EQ(24, lcm(24, 24));
}

TEST(lcm_test, HandlesZeroInput) {
  EXPECT_EQ(0, lcm(0, 5));
  EXPECT_EQ(0, lcm(5, 0));
  EXPECT_EQ(0, lcm(0, 0));
}
