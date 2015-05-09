#include <djp/number_theory/primes.hpp>
#include <gtest/gtest.h>
#include <algorithm> // for std::count

TEST(sieve_of_eratosthenes, FindPrimes) {
  const auto is_prime = djp::sieve_of_eratosthenes(7919 + 1);
  EXPECT_TRUE(is_prime[2]);
  EXPECT_TRUE(is_prime[3]);
  EXPECT_FALSE(is_prime[4]);
  EXPECT_TRUE(is_prime[5]);
  EXPECT_TRUE(is_prime[7919]);
  EXPECT_FALSE(is_prime[7917]);

  auto num_primes = std::count(is_prime.begin() + 2, is_prime.end(), true);
  EXPECT_EQ(1000, num_primes);
}
