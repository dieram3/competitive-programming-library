//          Copyright Diego Ramírez April 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <djp/combinatorics/nth_permutation.hpp>
#include <gtest/gtest.h>

#include <algorithm>  // for std::is_sorted
#include <functional> // for std::greater
#include <vector>     // for std::vector
#include <cstddef>    // for std::size_t
#include <cstdint>    // for std::uint64_t, UINT64_MAX

using namespace djp;
using std::size_t;
using std::uint64_t;

TEST(NthPermutationTest, WorksOnAllPermutations) {
  std::vector<size_t> perm[] = {
      {0, 1, 2}, {0, 2, 1}, {1, 0, 2}, {1, 2, 0}, {2, 0, 1}, {2, 1, 0}};
  constexpr size_t len = 3;
  constexpr uint64_t perms = 6;
  for (size_t i = 0; i < perms; ++i) {
    EXPECT_EQ(perm[i], nth_permutation(len, i));
  }
}

TEST(NthPermutationTest, WorksOnExtremeCase) {
  uint64_t factorial_20 = 2432902008176640000;
  auto perm = nth_permutation(20, factorial_20 - 1);
  EXPECT_TRUE(std::is_sorted(begin(perm), end(perm), std::greater<uint64_t>{}));

  std::vector<size_t> perm2 = {0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10,
                               11, 12, 13, 14, 15, 16, 17, 20, 19, 18};
  EXPECT_EQ(perm2, nth_permutation(21, 5));

  std::vector<size_t> perm3 = {7,  12, 14, 4, 3, 20, 5,  9,  6,  11, 0,
                               18, 10, 16, 1, 2, 8,  17, 15, 19, 13};

  EXPECT_EQ(perm3, nth_permutation(21, UINT64_MAX));
}
