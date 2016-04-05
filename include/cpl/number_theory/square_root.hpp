//          Copyright Diego Ramírez September 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef CPL_NUMBER_THEORY_SQUARE_ROOT_HPP
#define CPL_NUMBER_THEORY_SQUARE_ROOT_HPP

#include <algorithm>   // For std::min
#include <limits>      // For std::numeric_limits
#include <type_traits> // For std::make_unsigned
#include <cmath>       // For std::sqrt

namespace cpl {

/// \brief Computes the integer square root of a non-negative integer.
///
/// \param x The input integer.
///
/// \pre <tt>x >= 0</tt>.
///
/// \returns The integer square root of <tt>x</tt>.
///
template <typename T>
T isqrt(const T x) {
  using std::sqrt;
  using U = typename std::make_unsigned<T>::type;
  constexpr U max_ans = (U(1) << (std::numeric_limits<U>::digits / 2)) - 1;
  U ans = std::min(max_ans, static_cast<U>(sqrt(x)));
  // At this point, 'ans' is an approximate square root.
  while (ans * ans > U(x))
    --ans;
  while (ans < max_ans && (ans + 1) * (ans + 1) <= U(x))
    ++ans;
  return ans;
}

/// \brief Checks whether a non-negative integer is a perfect square.
///
/// \param x The input integer.
///
/// \pre <tt>x >= 0</tt>
///
/// \returns \c true if \p x is a perfect square, \c false otherwise.
///
template <typename T>
bool is_square(const T x) {
  auto brute_check = [x] {
    T t = isqrt(x);
    return t * t == x;
  };
  T d = x % 16;
  return (d == 0 || d == 1 || d == 4 || d == 9) && brute_check();
}

} // end namespace cpl

#endif // Header guard
