//          Copyright Diego Ramírez May 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <djp/number_theory/eratosthenes.hpp>
#include <gtest/gtest.h>

#include <algorithm> // for std::binary_search
#include <iterator>  // fot std::begin
#include <stdexcept> // for std::logic_error
#include <vector>    // for std::vector
#include <cstddef>   // for std::size_t
#include <cstdint>   // for std::int32_t

using namespace djp;

/// \brief Checks if a number is prime by using the given sieve.
/// \pre \p sieve shall not be empty.
/// \pre \p sieve shall be a sorted sequence of consecutive primes numbers
///      starting from 2.
/// \pre <tt>std::pow(sieve.back(), 2) <= number</tt>
template <class T>
static bool is_prime_sieve(T number, const std::vector<T> &sieve) {
  if (number <= sieve.back())
    return std::binary_search(begin(sieve), end(sieve), number);

  if (sieve.back() < ceil_div(number, sieve.back()))
    throw std::logic_error("The sieve is too small");

  for (const T prime : sieve) {
    if (prime > number / prime)
      break;
    if (number % prime == 0)
      return false;
  }

  return true;
}

TEST(sieve_of_eratosthenes, FindsPrimes) {
  const auto primes = sieve_of_eratosthenes(7919 + 1);
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

TEST(SieveOfEratosthenesTest, FindsAllRequiredPrimes) {
  auto num_primes_below =
      [](int32_t number) { return sieve_of_eratosthenes(number).size(); };

  EXPECT_EQ(0, num_primes_below(0));
  EXPECT_EQ(0, num_primes_below(1));
  EXPECT_EQ(0, num_primes_below(2));

  EXPECT_EQ(4, num_primes_below(10));
  EXPECT_EQ(4, num_primes_below(11));
  EXPECT_EQ(5, num_primes_below(12));

  EXPECT_EQ(9, num_primes_below(29));
  EXPECT_EQ(10, num_primes_below(30));
}

TEST(SieveOfEratosthenesTest, PrimalityTestingTest) {
  const auto small_sieve = sieve_of_eratosthenes<int32_t>(30);
  EXPECT_EQ(10, small_sieve.size());
  EXPECT_EQ(29, small_sieve.back());
  EXPECT_FALSE(is_prime_sieve(841, small_sieve));
  EXPECT_THROW(is_prime_sieve(842, small_sieve), std::logic_error);

  const auto big_sieve = sieve_of_eratosthenes(842);
  auto is_prime = [&big_sieve](int32_t p) {
    return std::binary_search(begin(big_sieve), end(big_sieve), p);
  };

  for (int32_t p = 0; p < 842; ++p)
    EXPECT_EQ(is_prime(p), is_prime_sieve(p, small_sieve));
}
