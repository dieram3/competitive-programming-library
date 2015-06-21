//          Copyright Diego Ramírez June 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
/// \file
/// \brief Defines several functions related to divisors.

#ifndef DJP_NUMBER_THEORY_DIVISOR_HPP
#define DJP_NUMBER_THEORY_DIVISOR_HPP

#include <stdexcept> // for std::logic_error
#include <vector>    // for std::vector
#include <cassert>   // for assert
#include <cstddef>   // for std::size_t

namespace djp {

/// \brief Counts how many divisors has a given number.
/// \param n The number to be tested.
/// \returns The number of divisors.
/// \pre The number to be tested shall be positive.
/// \sa find_divisors
template <typename T>
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

/// \brief Counts how many divisors has a given number.
/// \param n The number to be tested.
/// \param sieve A sieve of prime numbers.
/// \returns The number of divisors of \p n.
/// \pre \p n shall be a positive integer.
/// \pre \p sieve shall not be empty.
/// \pre \p sieve shall be a sorted sequence of consecutive prime numbers
/// starting from 2.
/// \throws std::logic_error if the sieve was not sufficient to determine how
/// many divisors has \p n. In order to avoid this, the square of the last prime
/// of the sieve should be greater than or equal to number to \p n.
/// \note A sieve containing all primes numbers less than 66000 is enough if the
/// value of \p n fits in a 32-bit unsigned integer.
/// \sa find_divisors
/// \sa sieve_of_eratosthenes
template <typename T, typename Range>
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

/// \brief Finds all divisors of a given number.
/// \param n The number to be tested.
/// \param sieve A sieve of prime numbers.
/// \returns A \c vector containing all divisors of \p n. Note that the returned
/// \c vector might be unordered.
/// \pre \p n shall be a positive integer.
/// \pre \p sieve shall not be empty.
/// \pre \p sieve shall be a sorted sequence of consecutive prime numbers
/// starting from 2.
/// \throws std::logic_error if the sieve was not sufficient to determine how
/// many divisors has \p n. In order to avoid this, the square of the last prime
/// of the sieve should be greater than or equal to \p n.
/// \note A sieve containing all primes numbers less than 66000 is enough if the
/// value of \p n fits in a 32-bit unsigned integer.
/// \sa count_divisors
/// \sa sieve_of_eratosthenes
template <typename T, typename Range>
std::vector<T> find_divisors(T n, const Range &sieve) {
  assert(n > 0);

  std::vector<T> divisors = {1};

  auto add_prime_factor = [&divisors](T prime, size_t begin, size_t end) {
    for (size_t i = begin; i != end; ++i)
      divisors.push_back(prime * divisors[i]);
  };

  for (auto d : sieve) {
    const size_t size = divisors.size();
    for (size_t begin = 0; n % d == 0; begin += size) {
      add_prime_factor(d, begin, begin + size);
      n /= d;
    }

    if (d > n / d) {
      if (n > 1)
        add_prime_factor(n, 0, divisors.size());
      return divisors;
    }
  }

  throw std::logic_error("The sieve is too small");
}

} // end namespace djp

#endif // Header guard
