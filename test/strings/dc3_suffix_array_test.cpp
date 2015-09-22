//          Copyright Patricio Beckmann August 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <djp/strings/dc3_suffix_array.hpp>
#include <gtest/gtest.h>

#include <algorithm>
#include <functional>
#include <iterator>
#include <numeric>
#include <random>
#include <string>
#include <vector>
#include <cstddef>

using namespace djp;

TEST(DC3SuffixArrayTest, WorksOnEmptyStrings) {
  EXPECT_TRUE(djp::dc3_suffix_array("").empty());
}

TEST(DC3SuffixArrayTest, WorksOnSmallStrings) {
  std::string str = "banana";
  std::vector<size_t> sa = {5, 3, 1, 0, 4, 2};
  EXPECT_EQ(sa, djp::dc3_suffix_array(str));
}

TEST(DC3SuffixArrayTest, Mississippi) {
  std::string str = "mississippi";
  std::vector<size_t> sa = {10, 7, 4, 1, 0, 9, 8, 6, 3, 5, 2};
  EXPECT_EQ(sa, djp::dc3_suffix_array(str));
}
