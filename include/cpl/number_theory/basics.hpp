//          Copyright Diego Ramirez 2014-2016
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef CPL_NUMBER_THEORY_BASICS_HPP
#define CPL_NUMBER_THEORY_BASICS_HPP

#include <cassert>     // assert
#include <type_traits> // is_integral

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

/// \brief Safely checks if `a * b < c`.
///
/// \pre `a, b, c` shall not be negative numbers.
///
template <typename T>
constexpr bool multiply_less(T a, T b, T c) {
  return b == 0 ? c > 0 : a < ceil_div(c, b);
}

/// \brief Safely checks if `a * b > c`.
///
/// \pre `a, b, c` shall not be negative numbers.
///
template <typename T>
constexpr bool multiply_greater(T a, T b, T c) {
  return b != 0 && a > c / b;
}

/// \brief Computes `pow(a, b)` without using floating-point arithmetic.
///
/// \param base The base.
/// \param exp The exponent.
///
/// \pre `exp >= 0`
///
/// \returns `base` raised to `exp`.
///
/// \par Complexity
/// Logarithmic in `exp`.
///
template <typename T, typename U>
T ipow(T base, U exp) {
  static_assert(std::is_integral<U>::value, "");
  assert(exp >= 0);

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
