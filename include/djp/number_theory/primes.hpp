//          Copyright Diego Ramírez May 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DJP_NUMBER_THEORY_PRIMES_HPP
#define DJP_NUMBER_THEORY_PRIMES_HPP

#include <djp/number_theory/modular.hpp>

#include <algorithm> // for std::fill_n, std::count
#include <array>     // for std::array
#include <iterator>  // for std::begin, std::end
#include <vector>    // for std::vector

#include <cassert> // for assert macro
#include <cstddef> // for std::size_t

namespace djp {

/// \brief Finds all primes number less than \p limit
/// \returns A sorted vector containing all primes numbers less than \p limit
/// Complexity: O(N * log log N) where N == \p limit
template <class T> std::vector<T> sieve_of_eratosthenes(T limit) {
  std::vector<bool> is_prime(limit, true);
  std::fill_n(begin(is_prime), std::min(T{2}, limit), false);

  // FIXME: if limit == std::numeric_limits<T>::max() the loop will be infinite.
  for (T i = 2; i * i < limit; ++i) {
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
/// \param p The number to be tested.
///
/// The following is guaranteed:
///   -# The function will never return a false positive if p <= UINT32_MAX
///   -# The function will never return a false negative.
template <class T> bool miller_primality_test(const T p) {
  if (p < 2)
    return false;
  if (p != 2 && p % 2 == 0)
    return false;

  // T = {2, 3, 5, 7, 11}
  // E[i] is the first false positive using T[1..i]
  // E = {2047, 1373653, 25326001, 3215031751, unknown: proven upto UINT32_MAX}
  std::array<T, 5> test_numbers = {{2, 3, 5, 7, 11}};

  if (std::binary_search(begin(test_numbers), end(test_numbers), p))
    return true;

  T s = p - 1;
  while (s % 2 == 0)
    s /= 2;

  for (const T a : test_numbers) {
    T temp = s;
    T mod = mod_pow(a, s, p);
    while (temp != p - 1 && mod != 1 && mod != p - 1) {
      mod = (mod * mod) % p;
      temp *= 2;
    }
    if (mod != p - 1 && temp % 2 == 0)
      return false;
  }
  return true;
}

} // namespace djp

#endif // Header guard
