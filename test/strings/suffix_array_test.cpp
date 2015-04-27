//          Copyright Diego Ramírez April 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <djp/strings/suffix_array.hpp>
#include <gtest/gtest.h>
#include <random>
#include <algorithm>
#include <functional>

TEST(make_suffix_array, WorksOnEmptyStrings) {
  EXPECT_TRUE(djp::make_suffix_array("").empty());
}

TEST(make_suffix_array, WorksOnSmallStrings) {
  std::string str = "banana";
  std::vector<size_t> sa = {5, 3, 1, 0, 4, 2};
  EXPECT_EQ(sa, djp::make_suffix_array(str));
}

TEST(make_suffix_array, WorksOnRepeatedStrings) {
  std::string str(10000, 'a');
  std::vector<size_t> sa(str.size());
  std::iota(sa.rbegin(), sa.rend(), 0);
  EXPECT_EQ(sa, djp::make_suffix_array(str));
}

TEST(make_suffix_array, WorksWith256chars) {
  std::string str(256, 0);
  std::iota(str.begin(), str.end(), 0);
  std::vector<size_t> sa(str.size());
  std::iota(sa.begin(), sa.end(), size_t{});
  EXPECT_EQ(sa, djp::make_suffix_array(str));
}

TEST(make_suffix_array, WorksWithStringFilledWithNull) {
  std::string str(100, '\0');
  std::vector<size_t> sa(str.size());
  std::iota(sa.rbegin(), sa.rend(), size_t{});
  EXPECT_EQ(sa, djp::make_suffix_array(str));
}

TEST(make_suffix_array, WorksWithRandomBigStrings) {
  std::string str(10000, '\0');
  std::mt19937 gen;
  std::uniform_int_distribution<unsigned char> dist(0, 255);
  std::generate(str.begin(), str.end(), std::bind(dist, gen));

  auto suffix_less = [&str](size_t s1, size_t s2) {
    constexpr auto npos = std::string::npos;
    return str.compare(s1, npos, str, s2, npos) < 0;
  };

  const auto sa = djp::make_suffix_array(str);

  // It relies on uniform distribution, so different suffixes mismatch quickly.
  ASSERT_TRUE(std::is_sorted(sa.begin(), sa.end(), suffix_less));
}

TEST(make_lcp_array, WorksOnSmallStrings) {
  std::string str = "banana";
  const auto sa = djp::make_suffix_array(str);
  const auto actual_lcp = djp::make_lcp_array(str, sa);
  std::vector<size_t> expected_lcp = {1, 3, 0, 0, 2};
  EXPECT_EQ(expected_lcp, actual_lcp);
}
