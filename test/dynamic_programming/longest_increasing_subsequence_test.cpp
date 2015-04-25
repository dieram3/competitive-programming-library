//          Copyright Diego Ramírez March 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <djp/dynamic_programming/longest_increasing_subsequence.hpp>
#include <gtest/gtest.h>
#include <vector>
#include <numeric>
#include <iterator>

using djp::calc_lis_size;

TEST(lis_test, HandlesSmallRanges) {
  const int seq[] = {0, 8, 4, 12, 2, 10, 6, 14, 1, 9, 5, 13, 3, 11, 7, 15};
  EXPECT_EQ(6, djp::calc_lis_size(std::begin(seq), std::end(seq)));
}

TEST(lis_test, HandlesLongRanges) {
  std::vector<int> vec(4096);
  std::iota(vec.begin(), vec.end(), 0);
  EXPECT_EQ(vec.size(), djp::calc_lis_size(vec.begin(), vec.end()));
  EXPECT_EQ(1, djp::calc_lis_size(vec.rbegin(), vec.rend()));
}
