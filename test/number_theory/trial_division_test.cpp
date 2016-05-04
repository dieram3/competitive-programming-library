//          Copyright Diego Ramirez 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cpl/number_theory/trial_division.hpp>
#include <gtest/gtest.h>

#include <cpl/number_theory/eratosthenes.hpp>
#include <cstdint>   // uint32_t
#include <stdexcept> // logic_error
#include <vector>    // vector

using cpl::trial_division;
using cpl::sieve_of_eratosthenes;

namespace {
class TrialDivisionTest : public ::testing::Test {
protected:
  using int_t = std::uint32_t;
  using vec_t = std::vector<int_t>;

protected:
  vec_t factor(const int_t n) {
    return trial_division(n, sieve);
  }

  void set_sieve_limit(const int_t limit) {
    sieve = sieve_of_eratosthenes(limit);
  }

private:
  vec_t sieve;
};
} // end anonymous namespace

TEST_F(TrialDivisionTest, SmallIntegersTest) {
  set_sieve_limit(100);
  EXPECT_EQ(vec_t(), factor(1));
  EXPECT_EQ(vec_t({2}), factor(2));
  EXPECT_EQ(vec_t({3}), factor(3));
  EXPECT_EQ(vec_t({2, 2}), factor(4));
  EXPECT_EQ(vec_t({5}), factor(5));
  EXPECT_EQ(vec_t({2, 3}), factor(6));
  EXPECT_EQ(vec_t({7}), factor(7));
  EXPECT_EQ(vec_t({2, 2, 2}), factor(8));
  EXPECT_EQ(vec_t({2, 2, 3}), factor(12));
  EXPECT_EQ(vec_t({2, 2, 3, 5}), factor(60));
  EXPECT_EQ(vec_t({2, 2, 2, 2, 3, 5, 7}), factor(1680));
  EXPECT_EQ(vec_t({2, 2, 2, 3, 3, 3, 5, 7}), factor(7560));
}

TEST_F(TrialDivisionTest, LargeIntegersTest) {
  set_sieve_limit(66000);
  EXPECT_EQ(vec_t({104729}), factor(104729));
  EXPECT_EQ(vec_t({1284287}), factor(1284287));
  EXPECT_EQ(vec_t({43, 1284287}), factor(55224341));
  EXPECT_EQ(vec_t({3, 5, 17, 257, 65537}), factor(4294967295));
  EXPECT_EQ(vec_t({2147483647}), factor(2147483647));
  EXPECT_EQ(vec_t({3, 3, 7, 11, 31, 151, 331}), factor(1073741823));
  EXPECT_EQ(vec_t({47, 178481}), factor(8388607));
  EXPECT_EQ(vec_t({4294967291}), factor(4294967291)); // overflow test 1.
  EXPECT_EQ(vec_t({4294967279}), factor(4294967279)); // overflow test 2.
}

TEST_F(TrialDivisionTest, SquaresTest) {
  set_sieve_limit(100);
  EXPECT_EQ(vec_t({7, 7}), factor(49));
  EXPECT_EQ(vec_t({3, 3, 3, 3}), factor(81));
  EXPECT_EQ(vec_t({13, 13}), factor(169));
  EXPECT_EQ(vec_t({41, 41}), factor(1681));
  EXPECT_EQ(vec_t({97, 97}), factor(9409));
}

TEST_F(TrialDivisionTest, ExceptionsTest) {
  set_sieve_limit(6); // Sieve contains 2,3,5
  EXPECT_EQ(vec_t({5, 5}), factor(25));
  EXPECT_THROW(factor(29), std::logic_error);
}
