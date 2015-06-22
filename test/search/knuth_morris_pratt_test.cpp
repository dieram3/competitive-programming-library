//          Copyright Diego Ramírez April 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <djp/search/knuth_morris_pratt.hpp>
#include <gtest/gtest.h>
#include <string>
#include <random>
#include <algorithm>
#include <iterator>
#include <functional>

TEST(kmp_searcher, SearchsQuickly) {
  std::string pattern(1000, 'a');
  pattern.back() = 'b';
  std::string text(1000000, 'a');
  text.back() = 'b';
  auto search_on = djp::make_kmp_searcher(pattern.begin(), pattern.end());
  auto it = search_on(text.begin(), text.end());
  EXPECT_EQ(pattern.size(), std::distance(it, text.end()));
}

TEST(kmp_searcher, SearchCorrectly) {
  std::string text(10000, '\0');
  std::default_random_engine gen;
  std::uniform_int_distribution<> dist(0, 255);
  std::generate(text.begin(), text.end(), std::bind(dist, std::ref(gen)));
  auto p_first = text.begin() + text.size() / 2;
  auto p_last = p_first + 1000;
  auto search_on = djp::make_kmp_searcher(p_first, p_last);

  // This search relies on uniform distribution, so mismatch occur quickly.
  auto expected = std::search(text.begin(), text.end(), p_first, p_last);

  auto actual = search_on(text.begin(), text.end());
  EXPECT_TRUE(expected == actual);
}

TEST(kmp_searcher, ReturnLastIfNoMatch) {
  std::string pattern(10, 'b');
  std::string text(1000, 'c');
  auto search_on = djp::make_kmp_searcher(pattern.begin(), pattern.end());
  EXPECT_TRUE(search_on(text.begin(), text.end()) == text.end());
  text.clear();
  EXPECT_TRUE(search_on(text.begin(), text.end()) == text.end());
}
