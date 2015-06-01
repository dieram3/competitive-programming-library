//          Copyright Diego Ramírez May 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DJP_NUMBER_THEORY_PRIMES_HPP
#define DJP_NUMBER_THEORY_PRIMES_HPP

#include <djp/number_theory/modular.hpp>
#include <djp/number_theory/basics.hpp>

#include <algorithm> // for std::fill_n, std::count
#include <array>     // for std::array
#include <iterator>  // for std::begin, std::end
#include <stdexcept> // for std::domain_error
#include <vector>    // for std::vector

#include <cassert> // for assert macro
#include <cstddef> // for std::size_t
#include <cstdint> // for UINT32_MAX

namespace djp {

/// \brief Finds all primes number less than \p limit
/// \returns A sorted vector containing all primes numbers less than \p limit
/// \pre \p limit shall not be a negative number.
/// Complexity: O(N * log log N) where N == \p limit
template <class T> std::vector<T> sieve_of_eratosthenes(T limit) {
  std::vector<bool> is_prime(limit, true);
  std::fill_n(begin(is_prime), std::min(T{2}, limit), false);

  // Safely query if i * i < limit ( otherwise overflow could occur.)
  for (T i = 2; i < ceil_div(limit, i); ++i) {
    if (!is_prime[i])
      continue;
    for (T j = i * i; j < limit; j += i)
      is_prime[j] = false;
  }

  // Now construct the prime list
  const size_t num_primes = std::count(begin(is_prime), end(is_prime), true);
  std::vector<T> primes;
  primes.reserve(num_primes);
  for (T i = 2; i < limit; ++i)
    if (is_prime[i])
      primes.push_back(i);

  assert(num_primes == primes.size());

  return primes;
}

/// \brief Test if a number is a probable prime.
/// \param n The number to be tested.
/// \returns true if p is a probable prime, false is p is composite.
/// \pre All numbers of \p test_numbers shall be in the range [2, n - 1]
/// \pre n shall be and odd number greater than 2.
template <class T, class Range>
bool miller_rabin_primality_test(const T n, const Range &test_numbers) {

  // write n − 1 as 2^s*d with d odd by factoring powers of 2 from n − 1
  const size_t s = __builtin_ctzll(n - 1);
  const T d = (n - 1) >> s;

  for (const T a : test_numbers) {
    assert(a >= 2 && a < n);
    T x = mod_pow(a, d, n);
    if (x == 1 || x == n - 1)
      continue;

    size_t reps = s - 1;
    bool probable_prime = false;
    while (reps--) {
      x = x * x % n;
      if (x == 1)
        return false;
      if (x == n - 1) {
        probable_prime = true;
        break;
      }
    }
    if (!probable_prime)
      return false;
  }
  return true;
}

/// \brief Checks if \p n is a prime number.
/// \param n The number to be tested.
/// \throws std::domain error if p > UINT32_MAX and p is odd.
template <class T> bool miller_rabin_primality_test(const T n) {
  if (n == 2)
    return true;
  if (n < 2 || n % 2 == 0)
    return false;

  auto test_with = [n](std::initializer_list<T> ilist) {
    return miller_rabin_primality_test(n, ilist);
  };

  // http://en.wikipedia.org/wiki/Miller–Rabin_primality_test
  // Pomerance, Selfridge and Wagstaff[8] and Jaeschke[9] have verified that
  if (n < 2047)
    return test_with({2});
  if (n < 9080191)
    return test_with({31, 73});
  if (n <= T(UINT32_MAX))
    return test_with({2, 7, 61});
  throw std::domain_error("miller_rabin_primality_test");
}

} // namespace djp

#endif // Header guard
