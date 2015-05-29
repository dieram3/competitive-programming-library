#include <djp/number_theory/primes.hpp>
#include <gtest/gtest.h>
#include <algorithm>
#include <iterator>

TEST(sieve_of_eratosthenes, FindPrimes) {
  const auto primes = djp::sieve_of_eratosthenes(7919 + 1);
  auto is_prime = [&primes](size_t num) {
    return std::binary_search(begin(primes), end(primes), num);
  };

  EXPECT_FALSE(is_prime(0));
  EXPECT_FALSE(is_prime(1));
  EXPECT_TRUE(is_prime(2));
  EXPECT_TRUE(is_prime(3));
  EXPECT_FALSE(is_prime(4));
  EXPECT_TRUE(is_prime(5));
  EXPECT_TRUE(is_prime(7919));
  EXPECT_FALSE(is_prime(7917));

  EXPECT_EQ(1000, primes.size());
}
