//          Copyright Diego Ramírez November 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DJP_MATH_MODULAR_HPP
#define DJP_MATH_MODULAR_HPP

#include <type_traits>
#include <stdexcept>

namespace djp {

template <class T, T Mod>
class modular {
  static_assert(std::is_unsigned<T>::value, "T must be unsigned");
  static_assert(Mod >= 2, "Mod must be greater than or equal to 2");

 public:
  constexpr modular(T value = 0) : value_{value < Mod ? value : value % Mod} {}

  modular& operator+=(modular other) { return *this = (*this) + other; }
  modular& operator-=(modular other) { return *this = (*this) - other; }
  modular& operator*=(modular other) { return *this = (*this) * other; }
  modular& operator/=(modular other) { return *this = (*this) / other; }

  friend modular operator+(modular a, modular b) { return T(a) + T(b); }
  friend modular operator-(modular a, modular b) { return T(a) + Mod - T(b); }
  friend modular operator*(modular a, modular b) { return T(a) * T(b); }
  friend modular operator/(modular a, modular b) { return a * inverse(b); }

  friend modular pow(modular base, T exp) {
    if (exp == 0) return 1;
    if (exp == 1) return base;
    if (exp % 2 == 1) return base * pow(base, exp - 1);
    return pow(base * base, exp / 2);
  }
  friend modular inverse(modular b) {
    const auto inv_b = pow(b, Mod - 2);
    if (T(inv_b) == 0) throw std::domain_error("Modular number has no inverse");
    return inv_b;
  }
  friend bool congruent(modular a, modular b) { return T(a) == T(b); }

  template <class Integer>
  explicit operator Integer() const {
    return static_cast<Integer>(value_);
  }

 private:
  T value_;
};

}  // namespace djp

#endif  // Header guard
