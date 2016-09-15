//          Copyright Diego Ramirez 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cpl/number_theory/chinese_remainder_theorem.hpp>
#include <gtest/gtest.h>

#include <cpl/number_theory/euclid.hpp> // gcd
#include <cassert>                      // assert
#include <cstddef>                      // size_t
#include <vector>                       // vector

using cpl::chinese_remainder_theorem;
using cpl::gcd;
using std::size_t;

namespace {
class ChineseRemainderTheoremTest : public ::testing::Test {
  using int_t = long long;

  static bool pairwise_coprime(const std::vector<int_t>& m) {
    const size_t n = m.size();
    for (size_t i = 0; i < n; ++i) {
      for (size_t j = i + 1; j < n; ++j) {
        if (gcd(m[i], m[j]) != 1)
          return false;
      }
    }
    return true;
  }

protected:
  void check(const std::vector<int_t>& a, const std::vector<int_t>& m) {
    assert(!a.empty() && a.size() == m.size());
    ASSERT_TRUE(pairwise_coprime(m));
    const int_t x = chinese_remainder_theorem(a, m);
    EXPECT_LE(0, x);
    int_t prod = 1;
    for (size_t i = 0; i < a.size(); ++i) {
      prod *= m[i];
      EXPECT_EQ(a[i], x % m[i]) << "i=" << i << " a[i]=" << a[i]
                                << " m[i]=" << m[i];
    }
    EXPECT_LT(x, prod);
  }
};
} // end anonymous namespace

TEST_F(ChineseRemainderTheoremTest, PairwiseCoprimeTest) {
  check({0}, {1});
  check({0}, {2});
  check({4}, {10});
  check({0, 0}, {1, 1});
  check({1, 3}, {3, 10});
  check({0, 0, 0}, {1, 1, 1});

  check({2, 3, 1}, {3, 4, 5});
  check({3, 3, 0}, {4, 7, 9});
  check({1, 2, 4, 6, 10, 12}, {2, 3, 5, 7, 11, 13});
  check({4, 13, 10, 5}, {9, 14, 13, 11});
  check({16, 20, 15, 1, 0, 38}, {17, 29, 23, 2, 43, 41});

  check({2, 2, 4, 10, 17, 28, 20, 59, 79}, {3, 5, 7, 13, 19, 31, 43, 101, 193});

  check({1, 2, 4, 6, 10, 12, 16, 18, 22, 28, 30, 36, 40},
        {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41});
}
