//          Copyright Diego Ramírez June 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <djp/strings/lcp_array.hpp>
#include <gtest/gtest.h>

#include <djp/strings/suffix_array.hpp>

#include <algorithm>
#include <functional>
#include <iterator>
#include <random>
#include <string>
#include <vector>
#include <cstddef>

namespace {

/// \brief Computes the longest common prefix of the suffixes s1 and s2
/// Complexity: Linear in the size of str.
static inline size_t naive_lcp(const std::string &str, size_t s1, size_t s2) {
  if (s1 == s2)
    return str.size() - s1;
  const auto begin1 = begin(str) + s1;
  const auto begin2 = begin(str) + s2;
  return std::mismatch(begin1, end(str), begin2).first - begin1;
}

/// \brief Generates a random string with characters uniformly distributed
///        between [char_min, char_max]
static inline std::string generate_random_string(std::size_t size,
                                                 unsigned char char_min = 0,
                                                 unsigned char char_max = 255) {
  std::string str(size, '\0');
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<unsigned char> dist(char_min, char_max);
  std::generate(begin(str), end(str), std::bind(dist, std::ref(gen)));
  return str;
}

} // anonymous namespace

TEST(make_lcp_array, WorksOnSmallStrings) {
  std::string str = "banana";
  const auto sa = djp::make_suffix_array(str);
  const auto actual_lcp = djp::make_lcp_array(str, sa);
  std::vector<size_t> expected_lcp = {1, 3, 0, 0, 2};
  EXPECT_EQ(expected_lcp, actual_lcp);
}

TEST(lcp_querier, WorksOnSmallStrings) {
  std::string str = "abbaabbaab";
  const auto sa = djp::make_suffix_array(str);
  const djp::lcp_querier get_lcp(str, sa);

  EXPECT_EQ(6, get_lcp(0, 4));
  EXPECT_EQ(6, naive_lcp(str, 0, 4));

  // It proves every combination in two ways (a, b) and (b, a)
  for (size_t i = 0; i < str.size(); ++i)
    for (size_t j = 0; j < str.size(); ++j)
      EXPECT_EQ(naive_lcp(str, i, j), get_lcp(i, j)) << i << ' ' << j;
}

TEST(lcp_querier, WorksOnBigStrings) {
  const std::string str = generate_random_string(10000, 'A', 'Z');
  const auto sa = djp::make_suffix_array(str);
  const djp::lcp_querier get_lcp(str, sa);

  std::random_device rd;
  std::default_random_engine gen(rd());
  std::uniform_int_distribution<size_t> dist(0, str.size() - 1);

  for (size_t i = 0; i < str.size(); ++i) {
    const size_t s1 = dist(gen);
    const size_t s2 = dist(gen);
    get_lcp(s1, s2);
    // This relies on the uniform distribution of str so no prefix is so long.
    EXPECT_EQ(naive_lcp(str, s1, s2), get_lcp(s1, s2));
  }
}
