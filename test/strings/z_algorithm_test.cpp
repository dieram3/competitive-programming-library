//          Copyright Jorge Aguirre, Diego Ramírez June 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <djp/strings/z_algorithm.hpp>
#include <gtest/gtest.h>

#include <algorithm>  // for std::is_sorted
#include <functional> // for std::greater
#include <iterator>   // for std::begin, std::end, std::next
#include <numeric>    // for std::accumulate
#include <string>     // for std::string
#include <cstddef>    // for std::size_t

using namespace djp;
using std::size_t;

TEST(ZAlgorithmTest, AllCharactersEqual) {
  std::string str = "aaaaaaa";
  auto z = z_algorithm(str);
  EXPECT_TRUE(std::is_sorted(begin(z), end(z), std::greater<size_t>()));
  auto n = str.size();
  EXPECT_EQ(std::accumulate(begin(z), end(z), 0ull), n * (n + 1) / 2);
}

TEST(ZAlgorithmTest, AllCharactersDifferent) {
  std::string str = "abcdefghg";
  auto z = z_algorithm(str);
  EXPECT_EQ(std::accumulate(next(begin(z)), end(z), 0ull), 0ull);
}

TEST(ZAlgorithmTest, AllCharactersEqualBigInputs) {
  std::string str(100000, 'a');
  auto z = z_algorithm(str);
  EXPECT_TRUE(
      std::is_sorted(std::next(begin(z)), end(z), std::greater<size_t>()));
  auto n = str.size();
  EXPECT_EQ(std::accumulate(begin(z), end(z), 0ull), n * (n + 1) / 2);
}
