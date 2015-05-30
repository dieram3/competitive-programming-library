//          Copyright Diego Ramírez May 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <djp/number_theory/primes.hpp>
#include <gtest/gtest.h>

#include <algorithm> // for std::binary_search
#include <iterator>  // fot std::begin
#include <random>    // for std::mt19937
#include <stdexcept> // for std::logic_error
#include <vector>    // for std::vector

#include <cstddef> // for std::size_t
#include <cstdint> // for std::uint32_t, std::uint64_t

namespace {

/// \brief Safely checks if a * b < c
/// \pre a, b, c shall not be negative integers.
template <class T> constexpr bool multiply_lt(T a, T b, T c) {
  return b != 0 && a < c / b;
}

/// \brief Safely checks if a * b > c
/// \pre a, b, c shall not be negative integers.
template <class T> constexpr bool multiply_gt(T a, T b, T c) {
  return b != 0 && a > c / b;
}

/// \brief Checks if a number is prime by using the given sieve.
/// \pre \p sieve shall not be empty.
/// \pre \p sieve shall be a sorted sequence of consecutive primes numbers
///      starting from 2.
/// \pre \p std::pow(sieve.back(), 2) <= number
template <class T> bool is_prime_sieve(T number, const std::vector<T> &sieve) {
  if (number <= sieve.back())
    return std::binary_search(begin(sieve), end(sieve), number);

  if (multiply_lt(sieve.back(), sieve.back(), number))
    throw std::logic_error("The sieve is too small");

  for (const T prime : sieve) {
    if (multiply_gt(prime, prime, number))
      break;
    if (number % prime == 0)
      return false;
  }

  return true;
}

} // anonymous namesapce

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

TEST(miller_primality_test, WorksOnItsRange) {
  auto sieve = djp::sieve_of_eratosthenes<uint32_t>(66000);
  EXPECT_FALSE(is_prime_sieve(UINT32_MAX, sieve));
  EXPECT_FALSE(djp::miller_primality_test<uint64_t>(UINT32_MAX));

  for (auto prime : sieve)
    EXPECT_TRUE(djp::miller_primality_test<uint64_t>(prime));

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<uint32_t> dist(0, UINT32_MAX);

  for (size_t i = 0; i < 10000; ++i) {
    const uint32_t number = dist(gen);
    EXPECT_EQ(is_prime_sieve(number, sieve),
              djp::miller_primality_test<uint64_t>(number));
  }
}
