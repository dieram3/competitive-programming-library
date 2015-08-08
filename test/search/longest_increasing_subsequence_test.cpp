//          Copyright Diego Ramírez March 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <djp/search/longest_increasing_subsequence.hpp>
#include <gtest/gtest.h>
#include <vector>
#include <numeric>
#include <iterator>

using djp::longest_increasing_subsequence;
using std::vector;
using std::less;
using std::greater;
using std::less_equal;
using std::greater_equal;

template <typename InputRange, typename Compare>
static std::size_t calc_lis_size(const InputRange &range, Compare comp) {
  return djp::calc_lis_size(std::begin(range), std::end(range), comp);
}

TEST(CalcLisSizeTest, HandlesSmallRanges) {
  const int seq[] = {0, 8, 4, 12, 2, 10, 6, 14, 1, 9, 5, 13, 3, 11, 7, 15};
  EXPECT_EQ(6u, calc_lis_size(seq, std::less<int>{}));
}

TEST(CalcLisSizeTest, HandlesLongRanges) {
  vector<int> seq(4096);
  std::iota(seq.begin(), seq.end(), 0);
  EXPECT_EQ(seq.size(), calc_lis_size(seq, std::less<int>{}));
  EXPECT_EQ(1u, calc_lis_size(seq, greater<int>{}));
}

TEST(CalcLisSizeTest, HandlesNonStrict) {
  vector<int> seq(4096);
  std::iota(seq.begin() + 96, seq.end(), 0);
  EXPECT_EQ(4000u, calc_lis_size(seq, less<int>{}));
  EXPECT_EQ(4096u, calc_lis_size(seq, less_equal<int>{}));

  std::vector<int> seq2 = {130, 150, 150, 140, 120, 100, 30, 50, 50, 5};
  EXPECT_EQ(6u, calc_lis_size(seq2, greater<int>{}));
  EXPECT_EQ(8u, calc_lis_size(seq2, greater_equal<int>{}));
}

TEST(LongestIncreasingSubsequenceTest, UniqueNumbersTest) {
  const vector<int> seq = {0, 8, 4, 12, 2, 10, 6, 14,
                           1, 9, 5, 13, 3, 11, 7, 15};

  vector<size_t> lis;

  lis = longest_increasing_subsequence(seq, less<int>{});
  // The lis it should find is : {0, 2, 6, 9, 11, 15}
  // whose indices are: {0, 4, 6, 9, 13, 15}

  EXPECT_EQ(6, lis.size());
  EXPECT_EQ(vector<size_t>({0, 4, 6, 9, 13, 15}), lis);
  EXPECT_EQ(lis, longest_increasing_subsequence(seq, less_equal<int>{}));

  lis = longest_increasing_subsequence(seq, greater<int>{});
  // The lis should find is : {12, 10, 9, 5, 3}
  // whose indices are: {3, 5, 9, 10, 12}
  EXPECT_EQ(5, lis.size());
  EXPECT_EQ(vector<size_t>({3, 5, 9, 10, 12}), lis);
  EXPECT_EQ(lis, longest_increasing_subsequence(seq, greater_equal<int>{}));
}

TEST(LongestIncreasingSubsequenceTest, NonStrictTest) {
  const vector<int> seq = {1, 4, 1, 2, 10, 7, 10, 5, 8, 9, 9, 10, 13, 4, 7, 2};

  vector<size_t> lis;

  // The LIS it should find below is : {1, 2, 5, 8, 9, 10, 13}
  // Whose indices are: {2, 3, 7, 8, 10, 11, 12}
  lis = longest_increasing_subsequence(seq, less<int>{});
  EXPECT_EQ(7, lis.size());
  EXPECT_EQ(vector<size_t>({2, 3, 7, 8, 10, 11, 12}), lis);

  // The LIS it should find below is : {1, 1, 2, 5, 8, 9, 9, 10, 13}
  // Whose indices are: {0, 2, 3, 7, 8, 9, 10, 11, 12}
  lis = longest_increasing_subsequence(seq, less_equal<int>{});
  EXPECT_EQ(9, lis.size());
  EXPECT_EQ(vector<size_t>({0, 2, 3, 7, 8, 9, 10, 11, 12}), lis);

  // The LIS it should find below is : {10, 7, 5, 4, 2}
  // Whose indices are: {4, 5, 7, 13, 15}
  lis = longest_increasing_subsequence(seq, greater<int>{});
  EXPECT_EQ(5, lis.size());
  EXPECT_EQ(vector<size_t>({4, 5, 7, 13, 15}), lis);

  // The LIS it should find below is : {10, 10, 9, 9, 7, 2}
  // Whose indices are: {4, 6, 9, 10, 14, 15}
  lis = longest_increasing_subsequence(seq, greater_equal<int>{});
  EXPECT_EQ(6, lis.size());
  EXPECT_EQ(vector<size_t>({4, 6, 9, 10, 14, 15}), lis);
}
