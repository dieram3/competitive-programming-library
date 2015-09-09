//          Copyright Diego Ramírez November 2014, August 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
/// \file
/// \brief Defines classes and functions for doing modular arithmetics.

#ifndef DJP_NUMBER_THEORY_MODULAR_HPP
#define DJP_NUMBER_THEORY_MODULAR_HPP

#include <limits>      // for std::numeric_limits
#include <type_traits> // for std::make_unsigned, std::make_signed
#include <cstddef>     // for std::size_t
#include <cstdlib>     // for std::abs
#include <cstdint>     // for std::uint64_t, UINT32_MAX, INT64_MAX
#include <cassert>     // for assert

namespace djp {

/// \brief Safely computes <tt>(a * b) % m</tt>.
///
/// \param a The first operand.
/// \param b The second operand.
/// \param m The modulo.
///
/// \pre <tt>a >= 0</tt>
/// \pre <tt>b >= 0</tt>
/// \pre <tt>m < 2^63</tt>
///
/// \par Complexity
/// Constant if <tt>(a <= UINT32_MAX && b <= UINT32_MAX)</tt>, otherwise
/// logarithmic in \c b.
///
template <typename T>
T mod_mul(T a, T b, T m) {
  using uint_t = typename std::make_unsigned<T>::type;
  assert(a >= 0 && b >= 0 && "Does not handle negative numbers");
  assert(uint_t(m) <= uint64_t(INT64_MAX) && "Cannot multiply safely");

  a %= m;
  b %= m;
  if (uint_t(a) <= UINT32_MAX && uint_t(b) <= UINT32_MAX)
    return T(uint64_t(a) * b % m);

  uint_t x = 0, y = a % m;
  while (b > 0) {
    if (b % 2 == 1)
      x = (x + y) % m;
    y = (y * 2) % m;
    b /= 2;
  }
  return x % m;
}

/// \brief Computes <tt>pow(a, b) % m</tt>
///
/// \param base The base.
/// \param exp The exponent.
/// \param m The modulo.
///
/// \pre <tt>base >= 0</tt>
/// \pre <tt>exp >= 0</tt>
///
/// \par Complexity
/// <tt>O(log(exp))</tt>
///
template <typename T>
T mod_pow(T base, T exp, T m) {
  assert(base >= 0 && exp >= 0);
  base %= m;
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

/// \brief Integer like class used for doing modular arithmetics.
///
/// A modular object always is normalized, i.e each time a value \c x is
/// assigned to the object. It is stored as <tt>x % M</tt>.
///
template <typename T, T M>
class modular {
  static_assert(M >= 2, "Mod must be greater than or equal to 2");
  static_assert((M - 1) <= std::numeric_limits<T>::max() / (M - 1),
                "The MOD value may produce overflow");

public:
  /// \pre <tt>value >= 0</tt>.
  constexpr modular(T value = 0) : value_{value % M} {}

  modular &operator+=(modular other) { return *this = (*this) + other; }
  modular &operator-=(modular other) { return *this = (*this) - other; }
  modular &operator*=(modular other) { return *this = (*this) * other; }
  modular &operator/=(modular other) { return *this = (*this) / other; }

  friend modular operator+(modular a, modular b) { return T(a) + T(b); }
  friend modular operator-(modular a, modular b) { return T(a) + M - T(b); }
  friend modular operator*(modular a, modular b) { return T(a) * T(b); }
  friend modular operator/(modular a, modular b) { return a * inverse(b); }

  friend modular pow(modular base, std::size_t exp) {
    modular result = 1;
    while (exp) {
      if (exp & 1)
        result *= base;
      base *= base;
      exp >>= 1;
    }
    return result;
  }

  /// \pre \c M is prime.
  friend modular inverse(modular a) { return pow(a, M - 2); }

  friend bool congruent(modular a, modular b) { return T(a) == T(b); }

  template <class Integer>
  explicit operator Integer() const {
    return static_cast<Integer>(value_);
  }

private:
  T value_;
};

} // end namespace djp

#endif // Header guard
