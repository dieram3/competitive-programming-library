//          Copyright Diego Ramirez 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cpl/bits/gray_code.hpp>
#include <gtest/gtest.h>

using cpl::nth_gray_code;
using cpl::rank_gray_code;

TEST(GrayCodeTest, NthGrayCodeTest) {
  EXPECT_EQ(0, nth_gray_code(0));
  EXPECT_EQ(1, nth_gray_code(1));
  EXPECT_EQ(3, nth_gray_code(2));
  EXPECT_EQ(2, nth_gray_code(3));
  EXPECT_EQ(6, nth_gray_code(4));
  EXPECT_EQ(7, nth_gray_code(5));
  EXPECT_EQ(5, nth_gray_code(6));
  EXPECT_EQ(4, nth_gray_code(7));
  EXPECT_EQ(12, nth_gray_code(8));
  EXPECT_EQ(13, nth_gray_code(9));
  EXPECT_EQ(15, nth_gray_code(10));
  EXPECT_EQ(14, nth_gray_code(11));
  EXPECT_EQ(10, nth_gray_code(12));
  EXPECT_EQ(11, nth_gray_code(13));
  EXPECT_EQ(9, nth_gray_code(14));
  EXPECT_EQ(8, nth_gray_code(15));
}

TEST(GrayCodeTest, RankGrayCodeTest) {
  EXPECT_EQ(0, rank_gray_code(0));
  EXPECT_EQ(1, rank_gray_code(1));
  EXPECT_EQ(2, rank_gray_code(3));
  EXPECT_EQ(3, rank_gray_code(2));
  EXPECT_EQ(4, rank_gray_code(6));
  EXPECT_EQ(5, rank_gray_code(7));
  EXPECT_EQ(6, rank_gray_code(5));
  EXPECT_EQ(7, rank_gray_code(4));
  EXPECT_EQ(8, rank_gray_code(12));
  EXPECT_EQ(9, rank_gray_code(13));
  EXPECT_EQ(10, rank_gray_code(15));
  EXPECT_EQ(11, rank_gray_code(14));
  EXPECT_EQ(12, rank_gray_code(10));
  EXPECT_EQ(13, rank_gray_code(11));
  EXPECT_EQ(14, rank_gray_code(9));
  EXPECT_EQ(15, rank_gray_code(8));
}
