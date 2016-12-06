//          Copyright Diego Ramirez 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef CPL_NUMBER_THEORY_EXTENDED_EUCLID_HPP
#define CPL_NUMBER_THEORY_EXTENDED_EUCLID_HPP

#include <tuple>       // tuple, make_tuple
#include <type_traits> // is_signed

namespace cpl {

/// \brief Finds the greatest common divisor of two integers besides a pair of
/// Bézout coefficients.
///
/// Uses the extended euclidean algorithm to find a pair of Bézout coefficients
/// for \c a and \p b, that is, a pair of integers <tt>(x, y)</tt> such that:
/// <blockquote><tt>a*x + b*y = gcd(a, b)</tt></blockquote>
///
/// Bézout identity guarantees that a solution exists for any pair (a, b),
/// provided that either \p a or \p b is a non-zero integer.
///
/// \param a The first input integer.
/// \param b The second input integer.
///
/// \pre <tt>a * b != 0</tt>, that is, either \p a or \p b shall be a non-zero
/// integer.
///
/// \returns An \c std::tuple containing <tt>(d, x, y)</tt>, where <tt>d =
/// gcd(a, b)</tt> and <tt>(x, y)</tt> are the found Bézout coefficients for
/// <tt>(a, b)</tt>.
///
template <typename T>
std::tuple<T, T, T> extended_euclid(const T a, const T b) {
  static_assert(std::is_signed<T>::value, "");

  T r = b, old_r = a;
  T s = 0, old_s = 1;
  T t = 1, old_t = 0;
  while (r) {
    const T quotient = old_r / r;
    T tmp;
    tmp = r, r = old_r - quotient * r, old_r = tmp;
    tmp = s, s = old_s - quotient * s, old_s = tmp;
    tmp = t, t = old_t - quotient * t, old_t = tmp;
  }
  return old_r > 0 ? std::make_tuple(old_r, old_s, old_t)
                   : std::make_tuple(-old_r, -old_s, -old_t);
}

} // end namespace cpl

#endif // Header guard
