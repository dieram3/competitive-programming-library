//          Copyright Diego Ramírez 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cpl/strings/lcp_array.hpp>
#include <gtest/gtest.h>

#include <cpl/strings/suffix_array.hpp>

#include <algorithm> // mismatch
#include <cassert>   // assert
#include <cstddef>   // size_t
#include <iterator>  // begin, distance
#include <string>    // string
#include <utility>   // swap
#include <vector>    // vector

using namespace cpl;
using std::size_t;

// ==========================================
// MakeLCPArrayTest
// ==========================================

namespace {
class MakeLCPArrayTest : public ::testing::Test {
protected:
  using lcp_array = std::vector<size_t>;
  using suffix_array = std::vector<size_t>;

protected:
  void check_lcp(const std::string &str, const lcp_array &expected_lcp) {
    ASSERT_EQ(str.size(), expected_lcp.size());
    const suffix_array sa = make_suffix_array(str);
    const lcp_array actual_lcp = make_lcp_array(str, sa);
    EXPECT_EQ(expected_lcp, actual_lcp) << " bad LCP for string: " << str;
  }
};
} // end anonymous namespace

TEST_F(MakeLCPArrayTest, EmptyStringTest) { check_lcp("", {}); }

TEST_F(MakeLCPArrayTest, OneCharacterTest) {
  check_lcp("b", {0});
  check_lcp("x", {0});
  check_lcp("1", {0});
  check_lcp(" ", {0});
  check_lcp(std::string(1, '\0'), {0});
}

TEST_F(MakeLCPArrayTest, MultipleCharactersTest) {
  check_lcp("banana", {1, 3, 0, 0, 2, 0});
  check_lcp("mississippi", {1, 1, 4, 0, 0, 1, 0, 2, 1, 3, 0});
  check_lcp("alphabetically", {1, 2, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0});
  check_lcp("loop,loop,loop", {5, 0, 4, 9, 0, 3, 8, 1, 2, 7, 0, 1, 6, 0});

  check_lcp(std::string(5, 'a'), {1, 2, 3, 4, 0});
  check_lcp(std::string(5, 'z'), {1, 2, 3, 4, 0});
  check_lcp(std::string(5, ' '), {1, 2, 3, 4, 0});
  check_lcp(std::string(5, '\0'), {1, 2, 3, 4, 0});
}

// ==========================================
// LCPQuerierTest
// ==========================================

static size_t naive_lcp(const std::string &str, size_t s1, size_t s2) {
  using std::begin;
  using std::end;
  using std::distance;

  if (s1 == s2)
    return str.size() - s1;
  if (s1 < s2)
    std::swap(s1, s2);
  const auto begin1 = begin(str) + s1;
  const auto begin2 = begin(str) + s2;
  assert(distance(begin1, end(str)) < distance(begin2, end(str)));

  const auto mm_pair = std::mismatch(begin1, end(str), begin2);
  return distance(begin1, mm_pair.first);
}

namespace {
class LCPQuerierTest : public ::testing::Test {
protected:
  using suffix_array = std::vector<size_t>;

protected:
  /// \brief Builds and \c lcp_querier an check that every valid pair of
  /// suffixes (i,j) gives the correct LCP, as computed by \c naive_lcp
  /// function.
  ///
  void check_all_pairs(const std::string &str) {
    const suffix_array sa = make_suffix_array(str);
    const lcp_querier get_lcp(str, sa);

    for (size_t i = 0; i < str.size(); ++i)
      for (size_t j = 0; j < str.size(); ++j)
        EXPECT_EQ(naive_lcp(str, i, j), get_lcp(i, j))
            << "string=" << str << ", "
            << "(i,j)=(" << i << "," << j << ")";
  }
};
} // end anonymous namespace

TEST_F(LCPQuerierTest, EmptyStringTest) { check_all_pairs(""); }

TEST_F(LCPQuerierTest, OneCharacterTest) {
  check_all_pairs("a");
  check_all_pairs("b");
  check_all_pairs(" ");
  check_all_pairs(std::string(1, '\0'));
}

TEST_F(LCPQuerierTest, WorksOnSmallStrings) {
  check_all_pairs("banana");
  check_all_pairs("mississippi");
  check_all_pairs("loop,loop,loop");
  check_all_pairs("abbaabbaab");

  check_all_pairs(std::string(5, 's'));
  check_all_pairs(std::string(5, ' '));
  check_all_pairs(std::string(5, '\0'));
}
