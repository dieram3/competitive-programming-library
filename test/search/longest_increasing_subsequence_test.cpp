//          Copyright Diego Ramírez March 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <djp/search/longest_increasing_subsequence.hpp>
#include <gtest/gtest.h>
#include <vector>
#include <numeric>
#include <iterator>

template <class InputRange, class Compare>
std::size_t calc_lis_size(const InputRange &range, Compare comp) {
  return djp::calc_lis_size(std::begin(range), std::end(range), comp);
}

TEST(calc_lis_size, HandlesSmallRanges) {
  const int seq[] = {0, 8, 4, 12, 2, 10, 6, 14, 1, 9, 5, 13, 3, 11, 7, 15};
  EXPECT_EQ(6u, calc_lis_size(seq, std::less<int>{}));
}

TEST(calc_lis_size, HandlesLongRanges) {
  std::vector<int> seq(4096);
  std::iota(seq.begin(), seq.end(), 0);
  EXPECT_EQ(seq.size(), calc_lis_size(seq, std::less<int>{}));
  EXPECT_EQ(1u, calc_lis_size(seq, std::greater<int>{}));
}

TEST(calc_lis_size, HandlesNonStrict) {
  std::vector<int> seq(4096);
  std::iota(seq.begin() + 96, seq.end(), 0);
  EXPECT_EQ(4000u, calc_lis_size(seq, std::less<int>{}));
  EXPECT_EQ(4096u, calc_lis_size(seq, std::less_equal<int>{}));

  std::vector<int> seq2 = {130, 150, 150, 140, 120, 100, 30, 50, 50, 5};
  EXPECT_EQ(6u, calc_lis_size(seq2, std::greater<int>{}));
  EXPECT_EQ(8u, calc_lis_size(seq2, std::greater_equal<int>{}));
}
