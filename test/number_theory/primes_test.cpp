//          Copyright Diego Ramírez May 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <djp/number_theory/primes.hpp>
#include <gtest/gtest.h>

#include <djp/utility/basics.hpp> // for djp::repeat

#include <algorithm> // for std::binary_search
#include <iterator>  // fot std::begin
#include <random>    // for std::mt19937
#include <stdexcept> // for std::logic_error
#include <vector>    // for std::vector

#include <cstddef> // for std::size_t
#include <cstdint> // for std::uint32_t, std::uint64_t

namespace {

/// \brief Checks if a number is prime by using the given sieve.
/// \pre \p sieve shall not be empty.
/// \pre \p sieve shall be a sorted sequence of consecutive primes numbers
///      starting from 2.
/// \pre \p std::pow(sieve.back(), 2) <= number
template <class T>
bool is_prime_sieve(T number, const std::vector<T> &sieve) {
  if (number <= sieve.back())
    return std::binary_search(begin(sieve), end(sieve), number);

  if (sieve.back() < djp::ceil_div(number, sieve.back()))
    throw std::logic_error("The sieve is too small");

  for (const T prime : sieve) {
    if (prime > number / prime)
      break;
    if (number % prime == 0)
      return false;
  }

  return true;
}

} // anonymous namesapce

TEST(sieve_of_eratosthenes, FindsPrimes) {
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

TEST(sieve_of_eratosthenes, FindsAllRequiredPrimes) {
  auto num_primes_below =
      [](int32_t number) { return djp::sieve_of_eratosthenes(number).size(); };

  EXPECT_EQ(0, num_primes_below(0));
  EXPECT_EQ(0, num_primes_below(1));
  EXPECT_EQ(0, num_primes_below(2));

  EXPECT_EQ(4, num_primes_below(10));
  EXPECT_EQ(4, num_primes_below(11));
  EXPECT_EQ(5, num_primes_below(12));

  EXPECT_EQ(9, num_primes_below(29));
  EXPECT_EQ(10, num_primes_below(30));
}

TEST(is_prime_sieve, WorksWell) {
  const auto small_sieve = djp::sieve_of_eratosthenes<int32_t>(30);
  EXPECT_EQ(10, small_sieve.size());
  EXPECT_EQ(29, small_sieve.back());
  EXPECT_FALSE(is_prime_sieve(841, small_sieve));
  EXPECT_THROW(is_prime_sieve(842, small_sieve), std::logic_error);

  const auto big_sieve = djp::sieve_of_eratosthenes(842);
  auto is_prime = [&big_sieve](int32_t p) {
    return std::binary_search(begin(big_sieve), end(big_sieve), p);
  };

  for (int32_t p = 0; p < 842; ++p)
    EXPECT_EQ(is_prime(p), is_prime_sieve(p, small_sieve));
}

TEST(is_prime_mr, WorksOnItsRange) {
  auto sieve = djp::sieve_of_eratosthenes<uint32_t>(66000);
  EXPECT_FALSE(is_prime_sieve(UINT32_MAX, sieve));
  EXPECT_FALSE(djp::is_prime_mr(UINT32_MAX));

  for (auto prime : sieve)
    EXPECT_TRUE(djp::is_prime_mr(prime));

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<uint32_t> dist;

  djp::repeat(12345, [&dist, &gen, &sieve] {
    const uint32_t number = dist(gen);
    EXPECT_EQ(is_prime_sieve(number, sieve), djp::is_prime_mr(number));
  });

  EXPECT_TRUE(djp::is_prime_mr(2u));
  EXPECT_FALSE(djp::is_prime_mr(1u));
  EXPECT_FALSE(djp::is_prime_mr(0u));
}

//#include <chrono>
//#include <iostream>
// TEST(sieve_of_eratosthenes, Benchmark) {
//  using namespace std::chrono;
//  auto start = steady_clock::now();
//  djp::sieve_of_eratosthenes(100000000);
//  auto end = steady_clock::now();
//  duration<double, std::milli> elapsed(end - start);
//  std::cout << "Elapsed: " << elapsed.count() << " ms\n";
//}
