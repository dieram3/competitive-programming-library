#ifndef DJP_NUMBER_THEORY_PRIMES_HPP
#define DJP_NUMBER_THEORY_PRIMES_HPP

#include <vector>    // for std::vector<bool>
#include <cstddef>   // for std::size_t
#include <algorithm> // for std::fill_n, std::count
#include <iterator>  // for std::begin, std::end

namespace djp {

/// \brief Finds all primes number less than \p limit
/// \returns A sorted vector containing all primes numbers less than \p limit
/// Complexity: O(N * log log N) where N == \p limit
std::vector<size_t> sieve_of_eratosthenes(size_t limit) {
  std::vector<bool> is_prime(limit, true);
  std::fill_n(begin(is_prime), std::min(size_t{2}, limit), false);

  for (size_t i = 2; i * i < limit; ++i) {
    if (!is_prime[i])
      continue;
    for (size_t j = i * i; j < limit; j += i)
      is_prime[j] = false;
  }

  // Now construct the prime list
  const size_t num_primes = std::count(begin(is_prime), end(is_prime), true);
  std::vector<size_t> primes;
  primes.reserve(num_primes);
  for (size_t i = 2; i < limit; ++i)
    if (is_prime[i])
      primes.push_back(i);

  return primes;
}

} // namespace djp

#endif // Header guard
