//          Copyright Diego Ramírez June 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DJP_NUMBER_THEORY_DIVISOR_HPP
#define DJP_NUMBER_THEORY_DIVISOR_HPP

#include <stdexcept>
#include <cassert> // for assert
#include <cstddef> // for std::size_t

namespace djp {

/// \brief Counts how may divisors has \p n
/// \param n The number to be tested.
/// \pre n > 0
template <class T>
std::size_t count_divisors(T n) {
  std::size_t ans = 1;
  for (T d = 2; !(d > n / d); ++d) {
    std::size_t exp = 0;
    while (n % d == 0) {
      n /= d;
      ++exp;
    }
    ans *= (exp + 1);
  }

  if (n > 1)
    ans *= 2;

  return ans;
}

/// \brief Counts how may divisors has \p n
/// \param n The number to be tested.
/// \param sieve A sieve of prime numbers.
/// \pre \p n > 0
/// \pre \p sieve shall not be empty.
/// \pre \p sieve shall be a sorted sequence of consecutive prime numbers
///      starting from 2.
template <class T, class Range>
std::size_t count_divisors(T n, const Range &sieve) {
  assert(n > 0);

  std::size_t ans = 1;
  for (auto d : sieve) {
    std::size_t exp = 0;
    while (n % d == 0) {
      n /= d;
      ++exp;
    }
    ans *= (exp + 1);

    if (d > n / d)
      return n > 1 ? 2 * ans : ans;
  }

  throw std::logic_error("The sieve is too small");
}

} // namespace djp

#endif // Header guard
