//          Copyright Diego Ramírez May 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef CPL_NUMBER_THEORY_ERATOSTHENES_HPP
#define CPL_NUMBER_THEORY_ERATOSTHENES_HPP

#include <cpl/number_theory/modular.hpp>
#include <cpl/number_theory/basics.hpp>

#include <algorithm> // for std::count
#include <iterator>  // for std::begin, std::end
#include <vector>    // for std::vector
#include <cassert>   // for assert macro
#include <cstddef>   // for std::size_t
#include <cstdint>   // for std::uint32_t, std::uint64_t

namespace cpl {

/// \brief Finds all primes number less than a given number.
///
/// \param limit The upper bound limit.
///
/// \returns A sorted \c vector containing all primes numbers less than \p
/// limit.
///
/// \pre <tt>limit >= 0</tt>.
///
/// \par Complexity
/// <tt>O(N * log log N)</tt>, where <tt>N = limit</tt>.
///
template <typename T>
std::vector<T> sieve_of_eratosthenes(const T limit) {
  if (limit < 2)
    return {};

  std::vector<bool> is_prime(limit, true);
  is_prime[0] = is_prime[1] = false;

  // uint64_t is used to prevent integer overflow.
  for (uint64_t i = 2; i * i < uint64_t(limit); ++i) {
    if (!is_prime[i])
      continue;
    for (uint64_t j = i * i; j < uint64_t(limit); j += i)
      is_prime[j] = false;
  }

  const size_t num_primes = std::count(begin(is_prime), end(is_prime), true);
  std::vector<T> primes;
  primes.reserve(num_primes);
  for (T i = 2; i < limit; ++i)
    if (is_prime[i])
      primes.push_back(i);

  assert(num_primes == primes.size());
  return primes;
}

} // namespace cpl

#endif // Header guard
