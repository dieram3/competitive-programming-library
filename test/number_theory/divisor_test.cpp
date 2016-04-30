//          Copyright Diego Ramirez 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cpl/number_theory/divisor.hpp>
#include <gtest/gtest.h>

#include <cpl/number_theory/eratosthenes.hpp>
#include <cpl/number_theory/trial_division.hpp>
#include <algorithm> // is_sorted, adjacent_find
#include <cstdint>   // uint32_t
#include <vector>    // vector

using cpl::count_divisors;
using cpl::sum_divisors;
using cpl::generate_divisors;
using cpl::sieve_of_eratosthenes;
using cpl::trial_division;

namespace {
class DivisorTest : public ::testing::Test {
protected:
  using int_t = std::uint32_t;
  using vec_t = std::vector<int_t>;

protected:
  void set_sieve_limit(const int_t limit) {
    sieve = sieve_of_eratosthenes(limit);
  }

  int_t count_divs(const int_t n) {
    const vec_t prime_factors = trial_division(n, sieve);
    return count_divisors(prime_factors);
  }

  vec_t gen_divs(const int_t n) {
    const vec_t prime_factors = trial_division(n, sieve);
    return generate_divisors(prime_factors);
  }

  int_t sum_divs(const int_t n) {
    const vec_t prime_factors = trial_division(n, sieve);
    return sum_divisors(prime_factors);
  }

  void check_divisors(const int_t n) {
    const auto prime_factors = trial_division(n, sieve);
    const vec_t divs = generate_divisors(prime_factors);
    EXPECT_EQ(count_divisors(prime_factors), divs.size());
    EXPECT_TRUE(std::is_sorted(divs.begin(), divs.end()));
    EXPECT_TRUE(divs.end() == std::adjacent_find(divs.begin(), divs.end()))
        << "There are repeated divisors";
    for (const int_t d : divs)
      EXPECT_EQ(0, n % d) << d << " is not a divisor of " << n;
  }

private:
  vec_t sieve;
};
} // end anonymous namespace

TEST_F(DivisorTest, CountDivisorsTest) {
  set_sieve_limit(1000); // Allows factoring integers upto 1,000,000 approx.
  EXPECT_EQ(1, count_divs(1));
  EXPECT_EQ(2, count_divs(2));
  EXPECT_EQ(2, count_divs(3));
  EXPECT_EQ(3, count_divs(4));
  EXPECT_EQ(2, count_divs(5));
  EXPECT_EQ(4, count_divs(6));
  EXPECT_EQ(2, count_divs(7));
  EXPECT_EQ(4, count_divs(8));

  // Some highly composite number (HCN)
  EXPECT_EQ(12, count_divs(60));
  EXPECT_EQ(90, count_divs(25200));
  EXPECT_EQ(108, count_divs(50400));
  EXPECT_EQ(216, count_divs(554400));
  EXPECT_EQ(240, count_divs(720720));

  // Some primes
  EXPECT_EQ(2, count_divs(43));
  EXPECT_EQ(2, count_divs(101));
  EXPECT_EQ(2, count_divs(6719));
}

TEST_F(DivisorTest, SumDivisorsTest) {
  set_sieve_limit(66000); // Allows factoring 32-bit integers.

  EXPECT_EQ(1, sum_divs(1));
  EXPECT_EQ(3, sum_divs(2));
  EXPECT_EQ(4, sum_divs(3));
  EXPECT_EQ(7, sum_divs(4));
  EXPECT_EQ(6, sum_divs(5));
  EXPECT_EQ(12, sum_divs(6));
  EXPECT_EQ(15, sum_divs(8));
  EXPECT_EQ(28, sum_divs(12));

  EXPECT_EQ(168, sum_divs(60));
  EXPECT_EQ(99944, sum_divs(25200));
  EXPECT_EQ(203112, sum_divs(50400));
  EXPECT_EQ(2437344, sum_divs(554400));
  EXPECT_EQ(3249792, sum_divs(720720));

  EXPECT_EQ(44, sum_divs(43));
  EXPECT_EQ(102, sum_divs(101));
  EXPECT_EQ(6720, sum_divs(6719));

  EXPECT_EQ(1619766720, sum_divs(1073741822)); // Overflow test 1
  EXPECT_EQ(1074106440, sum_divs(1073741819)); // Overflow test 2 (2969*361651)
  EXPECT_EQ(3221224800, sum_divs(2952789389)); // Overflow test 3 (11*268435399)
}

TEST_F(DivisorTest, GenerateDivisorsTest) {
  set_sieve_limit(11000); // Allows factoring integers upto 121,000,000 approx.

  EXPECT_EQ(vec_t({1}), gen_divs(1));
  EXPECT_EQ(vec_t({1, 2}), gen_divs(2));
  EXPECT_EQ(vec_t({1, 2, 3, 4, 6, 12}), gen_divs(12));
  EXPECT_EQ(vec_t({1, 2, 3, 4, 6, 8, 12, 24}), gen_divs(24));
  EXPECT_EQ(vec_t({1, 4729, 21379, 101101291}), gen_divs(101101291));
  EXPECT_EQ(vec_t({1, 2, 4, 13, 26, 52}), gen_divs(52));

  check_divisors(60);
  check_divisors(25200);
  check_divisors(50400);
  check_divisors(554400);
  check_divisors(720720);

  EXPECT_EQ(vec_t({1, 43}), gen_divs(43));
  EXPECT_EQ(vec_t({1, 101}), gen_divs(101));
  EXPECT_EQ(vec_t({1, 6719}), gen_divs(6719));
}
