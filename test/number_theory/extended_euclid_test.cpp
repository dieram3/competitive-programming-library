//          Copyright Diego Ramírez September 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <djp/number_theory/extended_euclid.hpp>
#include <gtest/gtest.h>

#include <tuple> // For std::tuple

using namespace djp;
using triple = std::tuple<long, long, long>;

TEST(ExtendedEuclidTest, InputWithZeroTest) {
  // Non-zero argument is positive
  EXPECT_EQ(triple(1, 0, 1), extended_euclid(0, 1));
  EXPECT_EQ(triple(5, 0, 1), extended_euclid(0, 5));
  EXPECT_EQ(triple(1, 1, 0), extended_euclid(1, 0));
  EXPECT_EQ(triple(8, 1, 0), extended_euclid(8, 0));

  // Non-zero argument is negative
  EXPECT_EQ(triple(1, 0, -1), extended_euclid(0, -1));
  EXPECT_EQ(triple(6, 0, -1), extended_euclid(0, -6));
  EXPECT_EQ(triple(1, -1, 0), extended_euclid(-1, 0));
  EXPECT_EQ(triple(11, -1, 0), extended_euclid(-11, 0));
}

TEST(ExtendedEuclidTest, PositiveInputTest) {
  EXPECT_EQ(triple(1, 0, 1), extended_euclid(1, 1));
  EXPECT_EQ(triple(1, 29, -41), extended_euclid(140, 99));
  EXPECT_EQ(triple(1, 21718, -4237), extended_euclid(9102, 46655));

  EXPECT_EQ(triple(5, -3, 5), extended_euclid(65, 40));
  EXPECT_EQ(triple(2, -9, 47), extended_euclid(240, 46));
  EXPECT_EQ(triple(4, 8, -107), extended_euclid(1124, 84));
  EXPECT_EQ(triple(21, -16, 27), extended_euclid(1239, 735));
  EXPECT_EQ(triple(30, -7, 3), extended_euclid(420, 990));
  EXPECT_EQ(triple(33, -9, 1), extended_euclid(693, 6270));
  EXPECT_EQ(triple(2431, -5, 72), extended_euclid(454597, 31603));
  EXPECT_EQ(triple(1961, -419, 130), extended_euclid(3296441, 10624698));
}

TEST(ExtendedEuclidTest, NegativeInputTest) {
  EXPECT_EQ(triple(1, 0, -1), extended_euclid(-1, -1));
  EXPECT_EQ(triple(1, 1, -2), extended_euclid(-7, -4));
  EXPECT_EQ(triple(1, -5041, 14441), extended_euclid(-52788, -18427));

  EXPECT_EQ(triple(9, -1, 0), extended_euclid(-9, -27));
  EXPECT_EQ(triple(4, 0, -1), extended_euclid(-8, -4));
  EXPECT_EQ(triple(3, 1, -1), extended_euclid(-12, -15));
  EXPECT_EQ(triple(6, -22, 5), extended_euclid(-78, -342));
  EXPECT_EQ(triple(2, -563, 67), extended_euclid(-412, -3462));
  EXPECT_EQ(triple(2, -179, 708), extended_euclid(-5134, -1298));
  EXPECT_EQ(triple(1010, 1, -41), extended_euclid(-206040, -5050));
  EXPECT_EQ(triple(287, 487, -200), extended_euclid(-222999, -543004));
}

TEST(ExtendedEuclidTest, PositiveAndNegativeInputTest) {
  EXPECT_EQ(triple(1, 0, -1), extended_euclid(1, -1));
  EXPECT_EQ(triple(1, -10, -7), extended_euclid(44, -63));
  EXPECT_EQ(triple(1, 23794, 13073), extended_euclid(32110, -58443));

  EXPECT_EQ(triple(12, 2, 1), extended_euclid(36, -60));
  EXPECT_EQ(triple(7, -1, -3), extended_euclid(98, -35));
  EXPECT_EQ(triple(13, 1, 1), extended_euclid(130, -117));
  EXPECT_EQ(triple(125, 0, -1), extended_euclid(875, -125));
  EXPECT_EQ(triple(216, 1, 0), extended_euclid(216, -2160));
  EXPECT_EQ(triple(987, 17, 12), extended_euclid(99687, -141141));
  EXPECT_EQ(triple(11, 13381, 296), extended_euclid(109263, -4939352));
}

TEST(ExtendedEuclidTest, NegativeAndPositiveInputTest) {
  EXPECT_EQ(triple(1, 0, 1), extended_euclid(-1, 1));
  EXPECT_EQ(triple(1, 3, 4), extended_euclid(-121, 91));
  EXPECT_EQ(triple(1, -291, -365), extended_euclid(-1386, 1105));

  EXPECT_EQ(triple(10, -16, -9), extended_euclid(-220, 390));
  EXPECT_EQ(triple(10, 0, 1), extended_euclid(-30, 10));
  EXPECT_EQ(triple(15, -1, 0), extended_euclid(-15, 105));
  EXPECT_EQ(triple(33, 1, 1), extended_euclid(-693, 726));
  EXPECT_EQ(triple(221, 16, 1), extended_euclid(-2873, 46189));
  EXPECT_EQ(triple(76, 4, 5), extended_euclid(-1976, 1596));
}
