#ifndef DJP_NUMBER_THEORY_PRIMES_HPP
#define DJP_NUMBER_THEORY_PRIMES_HPP

#include <vector>  // for std::vector<bool>
#include <cstddef> // for std::size_t

namespace djp {

/// Generates a vector<bool> v of the given size, where
/// v[i] indicates if i is a prime number or not.
/// Complexity: O(N * log log N) where N == size
std::vector<bool> sieve_of_eratosthenes(size_t size) {
  std::vector<bool> is_prime(size, true);
  if (0 < size)
    is_prime[0] = false;
  if (1 < size)
    is_prime[1] = false;

  for (size_t i = 2; i * i < size; ++i) {
    if (!is_prime[i])
      continue;
    for (size_t j = i * i; j < size; j += i)
      is_prime[j] = false;
  }
  return is_prime;
}

} // namespace djp

#endif // Header guard
