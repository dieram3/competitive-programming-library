//          Copyright Diego Ramírez September 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DJP_NUMBER_THEORY_TRIAL_DIVISION_HPP
#define DJP_NUMBER_THEORY_TRIAL_DIVISION_HPP

#include <stdexcept> // For std::logic_error
#include <vector>    // For std::vector

namespace djp {

/// \brief Finds the prime factors of a positive integer.
///
/// Uses the trial division algorithm to find the prime factors of <tt>n</tt>.
/// It is the most laborious integer factorization algorithm, so should be used
/// with discretion.
///
/// \param n The integer to be factored.
/// \param sieve A sieve of prime numbers.
///
/// \pre <tt>n > 0</tt>
/// \pre \p sieve must be a sorted vector containing the first
/// <tt>sieve.size()</tt> prime numbers.
///
/// \returns A sorted \c vector containing the prime factors of <tt>n</tt>.
///
/// \throws std::logic_error if the sieve is not big enough to factor
/// <tt>n</tt>. To ensure success, the square of <tt>sieve.back()</tt> should be
/// greater than or equal to <tt>n</tt>.
///
/// \par Complexity
/// The time complexity is <tt>O(&radic;n)</tt>.
///
/// \note A sieve containing all primes numbers less than \c 66000 is big enough
/// to factor integers less than <tt>2<sup>32</sup></tt>.
///
/// \sa sieve_of_eratosthenes
///
template <typename T>
std::vector<T> trial_division(T n, const std::vector<T> &sieve) {
  std::vector<T> primes;
  for (const T p : sieve) {
    while (n % p == 0) {
      primes.push_back(p);
      n /= p;
    }
    if (p > n / p) {
      if (n > 1)
        primes.push_back(n);
      return primes;
    }
  }
  throw std::logic_error("The sieve is not big enough");
}

} // end namespace djp

#endif // Header guard
