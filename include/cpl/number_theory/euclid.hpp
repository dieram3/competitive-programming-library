//          Copyright Diego Ramírez September 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef CPL_NUMBER_THEORY_EUCLID_HPP
#define CPL_NUMBER_THEORY_EUCLID_HPP

namespace cpl {

/// \brief Computes the greatest common divisor of two integers.
///
/// Uses the Euclidean algorithm to find the greatest common divisor (GCD) of
/// the integers \p a and \p b. The GCD is defined as the largest positive
/// integer that evenly divides both \p a and \p b.
///
/// \tparam T An integer type.
///
/// \param a The first integer.
/// \param b The second integer.
///
/// \returns The greatest common divisor of \p a and \p b. If both \p a and \p b
/// are zero, the GCD is undefined so this function returns 0.
///
template <typename T>
T gcd(T a, T b) {
  T tmp;
  while (b)
    tmp = b, b = a % b, a = tmp;
  return a < 0 ? -a : a;
}

/// \brief Computes the least common multiple of two integers.
///
/// Uses the Euclidean algorithm to find the least common multiple (LCM) of the
/// integers \p a and \p b. The LCM is defined as the smallest positive integer
/// that is divisible by both \p a and \p b.
///
/// \tparam T An integer type.
////
/// \param a The first integer.
/// \param b The second integer.
///
/// \pre The least common multiple of \p a and \p b must fit in T.
///
/// \returns The least common multiple between \p a and \p b. If either \p a or
/// \p b is zero, the LCM is undefined so this function returns 0.
///
template <typename T>
T lcm(T a, T b) {
  if (a == 0 || b == 0)
    return 0;
  T ans = a / gcd(a, b) * b;
  return ans < 0 ? -ans : ans;
}

} // end namespace cpl

#endif // Header guard
