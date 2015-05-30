#ifndef DJP_NUMBER_THEORY_PRIMES_HPP
#define DJP_NUMBER_THEORY_PRIMES_HPP

#include <vector>    // for std::vector
#include <array>     // for std::array
#include <cstddef>   // for std::size_t
#include <algorithm> // for std::fill_n, std::count
#include <iterator>  // for std::begin, std::end
#include <cassert>   // for assert macro
#include <djp/number_theory/modular.hpp>

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
/// If p <= UINT32_MAX it is guaranted that the answer will be correct.
/// \arg p The number to be tested.
template <class T> bool miller_primality_test(const T p) {
  if (p < 2)
    return false;
  if (p != 2 && p % 2 == 0)
    return false;

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
