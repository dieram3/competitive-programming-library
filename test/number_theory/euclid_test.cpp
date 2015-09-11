//          Copyright Diego Ramírez September 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <djp/number_theory/euclid.hpp>
#include <gtest/gtest.h>

using namespace djp;

namespace {

class EuclidTest : public ::testing::Test {

  using int_t = long long;

protected:
  void check_gcd(int_t x, int_t y, int_t expected) const {
    ASSERT_LE(0, expected); // The expected value cannot be negative.

    EXPECT_EQ(expected, gcd(x, y)) << x << ' ' << y;
    EXPECT_EQ(expected, gcd(x, -y)) << x << ' ' << y;
    EXPECT_EQ(expected, gcd(-x, y)) << x << ' ' << y;
    EXPECT_EQ(expected, gcd(-x, -y)) << x << ' ' << y;

    EXPECT_EQ(expected, gcd(y, x)) << x << ' ' << y;
    EXPECT_EQ(expected, gcd(y, -x)) << x << ' ' << y;
    EXPECT_EQ(expected, gcd(-y, x)) << x << ' ' << y;
    EXPECT_EQ(expected, gcd(-y, -x)) << x << ' ' << y;
  }

  void check_lcm(int_t x, int_t y, int_t expected) const {
    ASSERT_LE(0, expected); // The expected value cannot be negative.

    EXPECT_EQ(expected, lcm(x, y)) << x << ' ' << y;
    EXPECT_EQ(expected, lcm(x, -y)) << x << ' ' << y;
    EXPECT_EQ(expected, lcm(-x, y)) << x << ' ' << y;
    EXPECT_EQ(expected, lcm(-x, -y)) << x << ' ' << y;

    EXPECT_EQ(expected, lcm(y, x)) << x << ' ' << y;
    EXPECT_EQ(expected, lcm(y, -x)) << x << ' ' << y;
    EXPECT_EQ(expected, lcm(-y, x)) << x << ' ' << y;
    EXPECT_EQ(expected, lcm(-y, -x)) << x << ' ' << y;
  }
};

} // end anonymous namespace

TEST_F(EuclidTest, UndefinedGCDTest) { check_gcd(0, 0, 0); }

TEST_F(EuclidTest, DefinedGCDTest) {
  check_gcd(0, 1, 1);
  check_gcd(0, 2, 2);
  check_gcd(0, 5123, 5123);

  check_gcd(2, 30, 2);
  check_gcd(6, 30, 6);
  check_gcd(5, 10, 5);
  check_gcd(10, 13, 1);
  check_gcd(11, 22, 11);
  check_gcd(15, 15, 15);
  check_gcd(32340, 38220, 2940);
  check_gcd(165649, 557183, 15059);
  check_gcd(4257122337, 9338030548, 1); // Coprime composite numbers
  check_gcd(295076477, 314606869, 1);   // Coprime prime numbers

  // Check overflow is handled correctly.
  check_gcd(1908621660371581133, 1538741795868433129, 1038988383435809);
  check_gcd(1383000742879527101, 1634455423403077483, 125727340261775191);
}

TEST_F(EuclidTest, UndefinedLCMTest) {
  check_lcm(0, 0, 0);
  check_lcm(0, 1, 0);
  check_lcm(0, 5, 0);
  check_lcm(0, 4122, 0);
}

TEST_F(EuclidTest, DefinedLCMTest) {
  check_lcm(2, 5, 10);
  check_lcm(3, 6, 6);
  check_lcm(3, 7, 21);
  check_lcm(3, 11, 33);
  check_lcm(6, 15, 30);
  check_lcm(24, 24, 24);
  check_lcm(12, 60, 60);
  check_lcm(123, 323, 39729);
  check_lcm(4950, 7260, 108900);

  // Check overflow is handled correctly.
  check_lcm(1252956445974270543, 2088260743290450905, 6264782229871352715);
  check_lcm(1304021451848932435, 1825630032588505409, 9128150162942527045);
}
