//          Copyright Diego Ramirez 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef CPL_NUMBER_THEORY_PRIME_FACTORS_HPP
#define CPL_NUMBER_THEORY_PRIME_FACTORS_HPP

#include <algorithm> // sort
#include <stack>     // stack
#include <vector>    // vector

namespace cpl {

/// \brief Finds the prime factors of a positive integer.
///
/// This is a convenience function which uses a primality testing function and
/// an integer factorization function, both provided by the user, to decompose a
/// positive integer into prime factors.
///
/// \param n The integer to be factored.
///
/// \param is_prime The underlying primality test function. Given a positive
/// integer \c m greater than 1, <tt>is_prime(m)</tt> must return \c true if \c
/// m is a prime number.
///
/// \param get_factor The underlying integer factorization function. Given a
/// composite number \c m, <tt>get_factor(m)</tt> must return a non-trivial
/// factor of <tt>m</tt>.
///
/// \pre <tt>n > 0</tt>.
///
/// \returns A sorted vector containing the prime factors of <tt>n</tt>. If
/// <tt>n == 1</tt>, it returns an empty vector.
///
/// \par Complexity
/// Exactly \c M applications of \p is_prime and <tt>(M-1)</tt> applications
/// of <tt>get_factor</tt>, where \c M is the total number of prime factors of
/// <tt>n</tt>.
///
template <typename T, typename F1, typename F2>
std::vector<T> find_prime_factors(T n, F1 is_prime, F2 get_factor) {
  std::vector<T> primes;
  std::stack<T> stack;
  if (n > 1)
    stack.push(n);
  while (!stack.empty()) {
    n = stack.top();
    stack.pop();
    if (is_prime(n))
      primes.push_back(n);
    else {
      const T d = get_factor(n);
      stack.push(d);
      stack.push(n / d);
    }
  }
  std::sort(primes.begin(), primes.end()); // Sorting is optional.
  return primes;
}

} // end namespace cpl

#endif // Header guard
