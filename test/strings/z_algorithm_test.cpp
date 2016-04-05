//          Copyright Jorge Aguirre, Diego Ramírez 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cpl/strings/z_algorithm.hpp>
#include <gtest/gtest.h>

#include <algorithm> // mismatch
#include <cstddef>   // size_t
#include <iterator>  // distance
#include <string>    // for string
#include <vector>    // vector

using namespace cpl;
using std::size_t;

namespace {
class ZAlgorithmTest : public ::testing::Test {
protected:
  using z_array = std::vector<size_t>;

private:
  static z_array naive_z_algorithm(const std::string &str) {
    using std::mismatch;
    using std::distance;

    z_array z(str.size());
    for (size_t i = 0; i < str.size(); ++i) {
      const auto mm_pair = mismatch(str.begin() + i, str.end(), str.begin());
      const size_t lcp = distance(str.begin(), mm_pair.second);
      z[i] = lcp;
    }
    return z;
  }

public:
  void check(const std::string &str) {
    EXPECT_EQ(naive_z_algorithm(str), z_algorithm(str));
  }
};
} // end anononymous namespace

TEST_F(ZAlgorithmTest, EmptyStringTest) { check(""); }

TEST_F(ZAlgorithmTest, ExplicitTest) {
  EXPECT_EQ(z_array(), z_algorithm(""));
  EXPECT_EQ(z_array({1}), z_algorithm("x"));
  EXPECT_EQ(z_array({1}), z_algorithm(std::string(1, '\0')));

  EXPECT_EQ(z_array({9, 0, 0, 0, 0, 0, 3, 0, 0}), z_algorithm("abc123abc"));
  EXPECT_EQ(z_array({12, 0, 3, 0, 1, 2, 0, 0, 3, 0, 1, 1}),
            z_algorithm("ababaabbabaa"));
  EXPECT_EQ(z_array({6, 5, 4, 3, 2, 1}), z_algorithm("aaaaaa"));
}

TEST_F(ZAlgorithmTest, EqualCharactersTest) {
  check("aaaaa");
  check("xxxx");
  check("   ");
  check(std::string(8, '\0'));
}

TEST_F(ZAlgorithmTest, DifferentCharactersTest) {
  check("abcdefghg");
  check("loop,loop,loop");
  check("asiasd asdi asiasd");
  check("abaabababaababab");
  check("00000000011010000");
  check("10110011101100101010011010");
  check("    .    .  .     .");
}
