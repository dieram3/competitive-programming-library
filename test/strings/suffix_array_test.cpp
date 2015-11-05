//          Copyright Diego Ramírez April 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <djp/strings/suffix_array.hpp>
#include <gtest/gtest.h>

#include <algorithm>
#include <functional>
#include <iterator>
#include <numeric>
#include <random>
#include <string>
#include <vector>
#include <cstddef>

namespace {

/// \brief Generates a random string with characters uniformly distributed
///        between [char_min, char_max]
static inline std::string generate_random_string(std::size_t size,
                                                 unsigned char char_min = 0,
                                                 unsigned char char_max = 255) {
  std::string str(size, '\0');
  std::mt19937 gen;
  std::uniform_int_distribution<unsigned char> dist(char_min, char_max);
  std::generate(begin(str), end(str), std::bind(dist, std::ref(gen)));
  return str;
}

} // Anonymous namespace.

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
  const std::string str = generate_random_string(10000);

  auto suffix_less = [&str](size_t s1, size_t s2) {
    constexpr auto npos = std::string::npos;
    return str.compare(s1, npos, str, s2, npos) < 0;
  };

  const auto sa = djp::make_suffix_array(str);

  // It relies on uniform distribution, so different suffixes mismatch quickly.
  ASSERT_TRUE(std::is_sorted(sa.begin(), sa.end(), suffix_less));
}

//#include <iostream>
//#include <chrono>
// TEST(make_suffix_array, Benchmark) {
//  using namespace std::chrono;
//  std::string str(1000000, 'a');
//  auto start = steady_clock::now();
//  const auto sa = djp::make_suffix_array(str);
//  auto end = steady_clock::now();
//  EXPECT_EQ(0, sa.back());
//  duration<double, std::milli> elapsed = end - start;
//  std::clog << "Elapsed: " << elapsed.count() << " ms\n";
//}
