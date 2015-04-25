#include <djp/string/suffix_array.hpp>
#include <gtest/gtest.h>
#include <iostream>

TEST(make_suffix_array, WorksOnSmallStrings) {
  std::string str = "banana";
  const auto actual_sa = djp::make_suffix_array(str);
  std::vector<size_t> expected_sa = {5, 3, 1, 0, 4, 2};
  EXPECT_EQ(expected_sa, actual_sa);
}

TEST(make_lcp_array, WorksOnSmallStrings) {
  std::string str = "banana";
  const auto sa = djp::make_suffix_array(str);
  const auto actual_lcp = djp::make_lcp_array(str, sa);
  std::vector<size_t> expected_lcp = {1, 3, 0, 0, 2};
  EXPECT_EQ(expected_lcp, actual_lcp);
}
