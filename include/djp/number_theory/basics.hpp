//          Copyright Diego Ramírez July 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
/// \file
/// \brief Defines common functions used in number theory.

#ifndef DJP_NUMBER_THEORY_BASICS_HPP
#define DJP_NUMBER_THEORY_BASICS_HPP

namespace djp {

/// Computes the ceil of <tt>a / b</tt>
template <typename T>
constexpr T ceil_div(T a, T b) {
  return a / b + T(a % b == 0 ? 0 : 1);
}

/// \brief Safely checks if <tt>a * b < c</tt>
/// \pre a, b, c shall not be negative numbers.
template <typename T>
constexpr bool multiply_less(T a, T b, T c) {
  return b == 0 ? a * b < c : a < ceil_div(c, b);
}

/// \brief Safely checks if <tt>a * b > c</tt>
/// \pre a, b, c shall not be negative numbers.
template <typename T>
constexpr bool multiply_greater(T a, T b, T c) {
  return b != 0 && a > c / b;
}

} // end namespace djp

#endif // Header guard
