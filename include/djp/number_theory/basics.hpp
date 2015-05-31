//          Copyright Diego Ramírez July 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DJP_NUMBER_THEORY_BASICS_HPP
#define DJP_NUMBER_THEORY_BASICS_HPP

namespace djp {

/// Computes the greatest common divisor between  two integer numbers.
template <class T> constexpr T gcd(T a, T b) {
  return b == 0 ? a : gcd(b, a % b);
}

/// Computes the least common multiple between two integer numbers.
template <class T> constexpr T lcm(T a, T b) {
  return a || b ? a / gcd(a, b) * b : 0;
}

/// Computes the ceil of a / b
template <class T> constexpr T ceil_div(T a, T b) {
  return a / b + T(a % b == 0 ? 0 : 1);
}

} // namespace djp

#endif // Header guard
