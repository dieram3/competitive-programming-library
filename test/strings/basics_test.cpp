//          Copyright Diego Ramirez 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cpl/strings/basics.hpp>
#include <gtest/gtest.h>

#include <string>

TEST(SplitTest, SplitsWell) {
  {
    std::string str("abc,abc,abcd");
    const auto tokens = cpl::split(str, ",");
    ASSERT_EQ(3u, tokens.size());
    EXPECT_EQ("abc", tokens[0]);
    EXPECT_EQ("abc", tokens[1]);
    EXPECT_EQ("abcd", tokens[2]);
  }
  {
    std::string str(",abc,bac, cba; gaga, blah, da;;");
    const auto tokens = cpl::split(str, " ,;");
    ASSERT_EQ(6u, tokens.size());
    EXPECT_EQ("abc", tokens[0]);
    EXPECT_EQ("bac", tokens[1]);
    EXPECT_EQ("cba", tokens[2]);
    EXPECT_EQ("gaga", tokens[3]);
    EXPECT_EQ("blah", tokens[4]);
    EXPECT_EQ("da", tokens[5]);
  }
}
