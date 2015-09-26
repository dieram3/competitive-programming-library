//          Copyright Diego Ramírez November 2014, August 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
/// \file
/// \brief Defines functions for doing modular arithmetics.

#ifndef DJP_NUMBER_THEORY_MODULAR_HPP
#define DJP_NUMBER_THEORY_MODULAR_HPP

#include <type_traits> // for std::make_signed
#include <cassert>     // for assert

namespace djp {

/// \brief Safely computes <tt>(a + b) % m</tt>.
///
/// \param a The first operand.
/// \param b The second operand.
///
/// \param m The modulo.
///
/// \pre <tt>0 <= a < m</tt>.
/// \pre <tt>0 <= b < m</tt>
///
/// \par Complexity
/// Constant.
///
template <typename T>
T mod_sum(T a, T b, T m) {
  assert(a >= 0 && a < m);
  assert(b >= 0 && b < m);
  if (a < m - b)
    return a + b;
  return a - (m - b);
}

/// \brief Safely computes <tt>(a * b) % m</tt>.
///
/// \param a The first operand.
/// \param b The second operand.
/// \param m The modulo.
///
/// \pre <tt>0 <= a < m</tt>
/// \pre <tt>0 <= b < m</tt>
///
/// \par Complexity
/// Logarithmic in \c b.
///
template <typename T>
T mod_mul(T a, T b, T m) {
  assert(a >= 0 && a < m);
  assert(b >= 0 && b < m);
  T x = 0, y = a;
  while (b > 0) {
    if (b % 2 == 1)
      x = mod_sum(x, y, m);
    y = mod_sum(y, y, m);
    b /= 2;
  }
  return x;
}

/// \brief Computes <tt>pow(a, b) % m</tt>
///
/// \param base The base.
/// \param exp The exponent.
/// \param m The modulo.
///
/// \pre <tt>0 <= base < m</tt>
/// \pre <tt>0 <= exp</tt>
///
/// \par Complexity
/// <tt>O(log(exp))</tt> modular multiplications.
///
template <typename T>
T mod_pow(T base, T exp, T m) {
  assert(base >= 0 && base < m);
  assert(exp >= 0);
  T result = 1;
  while (exp) {
    if (exp & 1)
      result = mod_mul(result, base, m);
    base = mod_mul(base, base, m);
    exp >>= 1;
  }
  return result;
}

/// \brief Computes the modular multiplicative inverse of the given integer
/// modulo \c m.
///
/// Finds a value \c x such that  <tt>a * x == 1 (mod m)</tt>. It requires that
/// <tt>gcd(a, m) == 1</tt>, otherwise \p a will not be invertible.
///
/// \param a The number to be inverted.
/// \param m The modulo.
///
/// \pre <tt>m > 0</tt>.
///
/// \returns The modular inverse of <tt>a (mod m)</tt> in the range <tt>[0,
/// m)</tt>. If no inverse exists, returns <tt>m</tt>.
///
template <typename T>
T mod_inverse(T a, T m) {
  // If m is prime, pow(a, m - 2, m) gives the answer.
  using ST = typename std::make_signed<T>::type;
  ST r = a, old_r = m;
  ST t = 1, old_t = 0;
  while (r) {
    ST quotient = old_r / r;
    ST tmp;
    tmp = r, r = old_r - quotient * r, old_r = tmp;
    tmp = t, t = old_t - quotient * t, old_t = tmp;
  }
  if (old_r < 0)
    old_r = -old_r, old_t = -old_t;
  if (old_r != 1)
    return m; // a is not invertible.
  return old_t < 0 ? old_t + ST(m) : old_t;
}

} // end namespace djp

#endif // Header guard
