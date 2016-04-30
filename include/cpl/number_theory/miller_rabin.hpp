//          Copyright Diego Ramirez 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef CPL_NUMBER_THEORY_MILLER_RABIN_HPP
#define CPL_NUMBER_THEORY_MILLER_RABIN_HPP

#include <cpl/number_theory/modular.hpp> // mod_mul, mod_pow
#include <algorithm>                     // none_of
#include <cassert>                       // assert
#include <cstddef>                       // size_t
#include <initializer_list>              // initializer_list

namespace cpl {

/// \brief Checks if a given number is prime.
///
/// Uses a deterministic version of the Miller-Rabin algorithm to check if the
/// given number is prime.
///
/// \param n The number to be tested.
///
/// \pre n < 318665857834031151167461 (~3.19E23)
///
/// \par Complexity
/// At most <tt>O(k * log(n))</tt> modular multiplications, where \c k is the
/// number of base numbers to test against \c n (at most 12).
///
template <typename T>
bool miller_rabin_primality_test(const T n) {
  if (n == 2)
    return true;
  if (n < 2 || n % 2 == 0)
    return false;
  // Write n − 1 as 2^s*d (with d odd)
  T d = n - 1;
  size_t s = 0;
  while (d % 2 == 0)
    d /= 2, ++s;

  auto reveals_compositeness = [n, s, d](const T a) {
    assert(a >= 2 && a < n && "The witness is outside range");
    T x = mod_pow(a, d, n);
    if (x == 1 || x == n - 1)
      return false;
    for (size_t r = 1; r < s && x != 1; ++r) {
      x = mod_mul(x, x, n);
      if (x == n - 1)
        return false;
    }
    return true;
  };
  auto test_with = [&](std::initializer_list<T> witnesses) {
    return std::none_of(witnesses.begin(), witnesses.end(),
                        reveals_compositeness);
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
  // According to sequence A014233 in OEIS, the first 12 primes taken as base
  // are enough to test numbers less than 318665857834031151167461 ~ 3.19E23.
  return test_with({2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37});
}

} // end namespace cpl

#endif // Header guard
