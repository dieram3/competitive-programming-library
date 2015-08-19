//          Copyright Diego Ramírez November 2014, August 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
/// \file
/// \brief Defines classes and functions for doing modular arithmetics.

#ifndef DJP_NUMBER_THEORY_MODULAR_HPP
#define DJP_NUMBER_THEORY_MODULAR_HPP

#include <limits>  // for std::numeric_limits
#include <cstddef> // for std::size_t
#include <cstdint> // for std::uint32_t, std::uint64_t

namespace djp {

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
/// Constant.
///
template <typename T>
T mod_mul(T a, T b, T m) {
  static_assert(sizeof(T) <= sizeof(uint32_t), "Can't multiply safely");
  return uint64_t(a) * b % m;
}

/// \brief Computes <tt>pow(a, b) % m</tt>
///
/// \param base The base.
/// \param exp The exponent.
/// \param m The modulo.
///
/// \pre <tt>0 <= base < m</tt>
///
/// \par Complexity
/// <tt>O(log(exp))</tt>
///
template <typename T>
T mod_pow(T base, size_t exp, T m) {
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
/// <tt>gcd(a, m) == 1</tt> and <tt>a != 0</tt>, otherwise \p a will not be
/// invertible.
///
/// The current implementation uses the Euler's theorem for computing the
/// inverse of \p a given that \p m is a prime number. Therefore, this function
/// can be used iff \p m is prime.
///
/// \param a The number to be inverted.
/// \param m The modulo.
///
/// \returns The modular inverse of <tt>a (mod m)</tt>.
///
/// \pre \p m is prime.
/// \pre <tt>0 < a < m</tt>
///
/// \par Complexity
/// <tt>O(log(m))</tt>
///
template <typename T>
T mod_inverse(T a, T m) {
  return mod_pow(a, m - 2, m);
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
