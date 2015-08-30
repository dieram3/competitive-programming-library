//          Copyright Diego Ramírez August 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DJP_NUMBER_THEORY_MILLER_RABIN_HPP
#define DJP_NUMBER_THEORY_MILLER_RABIN_HPP

#include <djp/number_theory/modular.hpp> // For mod_mul, mod_pow
#include <algorithm>                     // For std::all_of
#include <initializer_list>              // For std::initializer_list
#include <stdexcept>                     // For std::domain_error
#include <cassert>                       // For assert
#include <cstddef>                       // For std::size_t

namespace djp {

/// \brief Checks if a given number is prime.
///
/// Uses a deterministic version of the Miller-Rabin algorithm to check if the
/// given number is prime.
///
/// \param n The number to be tested.
///
/// \pre n > 0
/// \pre n < 2^64
///
/// \par Complexity
/// At most <tt>O(k * log^2(n))</tt> modular multiplications, where \c k is the
/// number of base numbers to test against \c n (at most 12).
///
template <typename T>
bool miller_rabin_primality_test(const T n) {

  if (n == 2)
    return true;
  if (n < 2 || n % 2 == 0)
    return false;

  // write n − 1 as 2^s*d with d odd by factoring powers of 2 from n − 1
  const size_t s = __builtin_ctzll(n - 1);
  const T d = (n - 1) >> s;

  auto witness = [n, s, d](const T a) {
    assert(a >= 2 && a < n);
    T x = mod_pow(a, d, n);
    if (x == 1 || x == n - 1)
      return true;

    for (size_t rep = 0; rep < s - 1; ++rep) {
      x = mod_mul(x, x, n);
      if (x == 1)
        return false;
      if (x == n - 1)
        return true;
    }
    return false;
  };

  auto test_with = [witness](std::initializer_list<T> ilist) {
    return std::all_of(begin(ilist), end(ilist), witness);
  };

  if (n < 2047)
    return test_with({2});
  if (n < 9080191)
    return test_with({31, 73});
  if (n < 4759123141)
    return test_with({2, 7, 61}); // Until here it's enough for 32-bits ints.
  if (n < 1122004669633)
    return test_with({2, 13, 23, 1662803});
  if (n < 2152302898747)
    return test_with({2, 3, 5, 7, 11});
  if (n < 3474749660383)
    return test_with({2, 3, 5, 7, 11, 13});
  if (n < 341550071728321)
    return test_with({2, 3, 5, 7, 11, 13, 17});
  if (n < 3825123056546413051)
    return test_with({2, 3, 5, 7, 11, 13, 17, 19, 23});
  // The first 12 primes taken as base are enough to test numbers less than
  // 318665857834031151167461 ~ 3.18E23 i.e numbers having upto 78 bits.
  return test_with({2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37});
}

} // end namespace djp

#endif // Header guard
