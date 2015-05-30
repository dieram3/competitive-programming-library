//          Copyright Diego Ramírez November 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DJP_NUMBER_THEORY_MODULAR_HPP
#define DJP_NUMBER_THEORY_MODULAR_HPP

#include <limits>
#include <stdexcept>
#include <type_traits>

#include <cassert>
#include <cstddef>

namespace djp {

template <class T, T Mod> class modular {
  static_assert(std::is_unsigned<T>::value, "T must be unsigned");
  static_assert(Mod >= 2, "Mod must be greater than or equal to 2");
  static_assert((Mod - 1) <= std::numeric_limits<T>::max() / (Mod - 1),
                "The MOD value may produce overflow");

public:
  constexpr modular(T value = 0) : value_{value < Mod ? value : value % Mod} {}

  modular &operator+=(modular other) { return *this = (*this) + other; }
  modular &operator-=(modular other) { return *this = (*this) - other; }
  modular &operator*=(modular other) { return *this = (*this) * other; }
  modular &operator/=(modular other) { return *this = (*this) / other; }

  friend modular operator+(modular a, modular b) { return T(a) + T(b); }
  friend modular operator-(modular a, modular b) { return T(a) + Mod - T(b); }
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
  friend modular inverse(modular b) {
    const auto inv_b = pow(b, Mod - 2);
    if (T(inv_b) == 0)
      throw std::domain_error("Modular number has no inverse");
    return inv_b;
  }
  friend bool congruent(modular a, modular b) { return T(a) == T(b); }

  template <class Integer> explicit operator Integer() const {
    return static_cast<Integer>(value_);
  }

private:
  T value_;
};

/// \brief Computes a ^ b % MOD
/// Complexity: If (mod - 1)^2 <= std::numeric_limits<T>::max()
/// O(log(exp)) Otherwise O(log^2(exp))
template <class T> T mod_pow(T base, std::size_t exp, T mod) {
  assert(mod > 1);

  if ((mod - 1) > std::numeric_limits<T>::max() / (mod - 1)) {
    // FIXME: delegate to safe version.
    throw std::domain_error(
        "mod_pow migh overflow. The safe version is unimplemented.");
  }

  T result = 1;
  base %= mod;
  while (exp) {
    if (exp & 1)
      result = (result * base) % mod;
    base = (base * base) % mod;
    exp >>= 1;
  }
  return result;
}

} // namespace djp

#endif // Header guard
