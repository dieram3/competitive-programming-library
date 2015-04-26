#include <djp/string/suffix_array.hpp>
#include <gtest/gtest.h>
#include <iostream>

TEST(make_suffix_array, WorksOnSmallStrings) {
  std::string str = "banana";
  std::vector<size_t> sa = {5, 3, 1, 0, 4, 2};
  EXPECT_EQ(sa, djp::make_suffix_array(str));
}

TEST(make_suffix_array, WorksOnLongStrings) {
  std::string str(10000, 'a');
  std::vector<size_t> sa(str.size());
  std::iota(sa.rbegin(), sa.rend(), 0);
  EXPECT_EQ(sa, djp::make_suffix_array(str));
}

TEST(make_suffix_array, WorksWithTheAlphabet) {
  std::string str(52, 0);
  auto middle = str.begin() + 26;
  std::iota(str.begin(), middle, 'A');
  std::iota(middle, str.end(), 'a');

  std::vector<size_t> sa(str.size());
  std::iota(sa.begin(), sa.end(), size_t{});
  EXPECT_EQ(djp::make_suffix_array(str), sa);
}

TEST(make_lcp_array, WorksOnSmallStrings) {
  std::string str = "banana";
  const auto sa = djp::make_suffix_array(str);
  const auto actual_lcp = djp::make_lcp_array(str, sa);
  std::vector<size_t> expected_lcp = {1, 3, 0, 0, 2};
  EXPECT_EQ(expected_lcp, actual_lcp);
}
