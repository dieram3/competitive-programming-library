//          Copyright Diego Ramírez September 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DJP_NUMBER_THEORY_PRIME_FACTORS_HPP
#define DJP_NUMBER_THEORY_PRIME_FACTORS_HPP

#include <stack>  // For std::stack
#include <vector> // For std::vector

namespace djp {

/// \brief Finds the prime factors of a positive integer.
///
/// This is a convenience function which uses a primality testing function and
/// an integer factorization function, both provided by the user, to decompose a
/// positive integer into prime factors.
///
/// \param n The integer to be factored.
///
/// \param is_prime The underlying primality test function. Given a positive
/// number \c m greater than 1, <tt>is_prime(m)</tt> must return \c true if \c m
/// is a prime number.
///
/// \param get_factor The underlying integer factorization function. Given a
/// composite number \c m, <tt>get_factor(m)</tt> must return a non-trivial
/// factor of <tt>m</tt>.
///
/// \pre <tt>n > 0</tt>.
///
/// \returns An unordered list containing the prime factors of <tt>n</tt>. If
/// <tt>n == 1</tt>, it returns an empty list.
///
/// \par Complexity
/// Exactly \c M applications of \p is_prime and <tt>(M-1)</tt> applications
/// of <tt>get_factor</tt>, where \c M is the total number of prime factors of
/// <tt>n</tt>.
///
template <typename T, typename F1, typename F2>
std::vector<T> find_prime_factors(T n, F1 is_prime, F2 get_factor) {
  std::vector<T> primes;
  std::stack<T> S;
  if (n > 1)
    S.push(n);
  while (!S.empty()) {
    n = S.top();
    S.pop();
    if (is_prime(n))
      primes.push_back(n);
    else {
      const T d = get_factor(n);
      S.push(d);
      S.push(n / d);
    }
  }
  return primes;
}

} // end namespace djp

#endif // Header guard
