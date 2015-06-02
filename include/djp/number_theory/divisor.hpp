//          Copyright Diego Ramírez June 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DJP_NUMBER_THEORY_DIVISOR_HPP
#define DJP_NUMBER_THEORY_DIVISOR_HPP

#include <stdexcept> // for std::logic_error
#include <cassert>   // for assert
#include <cstddef>   // for std::size_t

namespace djp {

/// \brief Counts how many divisors has a number.
/// \param n The number to be tested.
/// \returns The number of divisors.
/// \pre The number to be tested shall be positive.
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

  return n > 1 ? 2 * ans : ans;
}

/// \brief Counts how many divisors has a number.
/// \param n The number to be tested.
/// \param sieve A sieve of prime numbers.
/// \returns The number of divisors.
/// \pre The number to be tested shall be positive.
/// \pre The sieve shall not be empty.
/// \pre The sieve shall be a sorted sequence of consecutive prime numbers
///      starting from 2.
/// \throws std::logic_error if the sieve was not sufficient to determine
///         how many divisors has \p n. In order to avoid this, the square of
///         of the last prime of the sieve should be greater than or equal to
///         number to be tested.
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
