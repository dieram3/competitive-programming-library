//          Copyright Diego Ramirez 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cpl/strings/knuth_morris_pratt.hpp>
#include <gtest/gtest.h>

#include <algorithm> // search
#include <iterator>  // distance
#include <string>    // string

using cpl::kmp_searcher;
using cpl::make_kmp_searcher;

namespace {
class KMPSearcherTest : public ::testing::Test {
public:
  using iterator_t = std::string::const_iterator;

protected:
  KMPSearcherTest() : pattern("."), search_on(pattern.begin(), pattern.end()) {}

  void set_pattern(const std::string& new_pattern) {
    ASSERT_FALSE(new_pattern.empty());
    pattern = new_pattern;
    search_on = make_kmp_searcher(pattern.cbegin(), pattern.cend());
  }

  size_t check_search(const std::string& text, const size_t pos = 0) const {
    using std::search;
    using std::distance;

    const auto first = text.begin() + pos;
    const auto last = text.end();
    const auto brute_it = search(first, last, pattern.begin(), pattern.end());
    const auto kmp_it = search_on(first, last);
    EXPECT_TRUE(brute_it == kmp_it) << "text=" << text.substr(pos)
                                    << "\npattern=" << pattern
                                    << "\nbrute-search finds occurrence at: "
                                    << distance(text.begin(), brute_it)
                                    << "\nkmp-search finds occurrence at: "
                                    << distance(text.begin(), kmp_it);

    return distance(text.begin(), brute_it);
  }

private:
  std::string pattern;
  kmp_searcher<iterator_t> search_on;
};
} // end anonymous namespace

TEST_F(KMPSearcherTest, SingleCharacterPatternTest) {
  set_pattern("a");
  check_search("abc abc");
  check_search("bad bad");
  check_search("  jk  a x");
  check_search("jiiuej");
  check_search(std::string(5, '\0'));
}

TEST_F(KMPSearcherTest, SmallTextTest) {
  set_pattern("aabbbaa");
  check_search("aab");
  check_search("aabbba");
  check_search("a");
}

TEST_F(KMPSearcherTest, FullTextPatternTest) {
  std::string str = "abcabcabc";
  set_pattern(str);
  check_search(str);

  str = "123090123";
  set_pattern(str);
  check_search(str);
}

TEST_F(KMPSearcherTest, EmptyTextSearch) {
  set_pattern("abc");
  check_search("");
  set_pattern("aab");
  check_search("");
}

TEST_F(KMPSearcherTest, SeveralSearchesTest) {
  set_pattern("abc");
  const std::string text("abc, abc,   abc,   abc,  dxa wd abc");
  size_t pos{};
  size_t cnt = 0;
  while (pos < text.size()) {
    pos = check_search(text, pos);
    if (pos == text.size())
      break;
    ++cnt;
    ++pos;
  }
  EXPECT_EQ(5, cnt)
      << "The number of occurrences found differs from the expected one.";
}
