//          Copyright Diego Ramirez 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
/// \file
/// \brief Defines common functions used in number theory.

#ifndef CPL_NUMBER_THEORY_BASICS_HPP
#define CPL_NUMBER_THEORY_BASICS_HPP

#include <cassert> // assert

namespace cpl {

/// Computes the ceil of `x / y`.
///
/// \pre `y != 0`
///
template <typename T>
T ceil_div(const T x, const T y) {
  assert(y != 0);
  const auto quot = x / y;
  const auto rem = x % y;
  const bool opposite_sign = (x >= 0) ^ (y >= 0);
  const bool is_correct = (rem == 0) | opposite_sign;

  // The branch below should be optimized away by the compiler.
  return quot + (is_correct ? 0 : 1);
}

/// Computes the floor of `x / y`.
///
/// \pre `y != 0`
///
template <typename T>
T floor_div(const T x, const T y) {
  assert(y != 0);
  const auto quot = x / y;
  const auto rem = x % y;
  const bool same_sign = (x >= 0) ^ (y < 0);
  const bool is_correct = (rem == 0) | same_sign;

  // The branch below should be optimized away by the compiler.
  return quot - (is_correct ? 0 : 1);
}

/// \brief Safely checks if <tt>a * b < c</tt>
/// \pre a, b, c shall not be negative numbers.
template <typename T>
constexpr bool multiply_less(T a, T b, T c) {
  return b == 0 ? c > 0 : a < ceil_div(c, b);
}

/// \brief Safely checks if <tt>a * b > c</tt>
/// \pre a, b, c shall not be negative numbers.
template <typename T>
constexpr bool multiply_greater(T a, T b, T c) {
  return b != 0 && a > c / b;
}

/// \brief Computes <tt>pow(a, b)</tt> without using floating-point arithmetic.
///
/// \param base The base.
/// \param exp The exponent.
///
/// \pre <tt>base >= 0</tt>
/// \pre <tt>exp >= 0</tt>
///
/// \returns <tt>base</tt> raised to <tt>exp</tt>.
///
/// \par Complexity
/// Logarithmic in <tt>exp</tt>.
///
template <typename T>
T ipow(T base, T exp) {
  assert(base >= 0 && exp >= 0);
  T result = 1;
  while (exp > 0) {
    if (exp % 2 == 1)
      result *= base;
    base *= base;
    exp /= 2;
  }
  return result;
}

} // end namespace cpl

#endif // Header guard
