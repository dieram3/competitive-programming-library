//          Copyright Diego Ramírez September 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DJP_NUMBER_THEORY_SQUARE_ROOT_HPP
#define DJP_NUMBER_THEORY_SQUARE_ROOT_HPP

#include <algorithm>   // For std::min
#include <limits>      // For std::numeric_limits
#include <type_traits> // For std::is_unsigned
#include <cmath>       // For std::sqrtf

namespace djp {

/// \brief Computes the integer square root of the given integer.
///
/// \param x The target integer.
///
/// \pre <tt>x >= 0</tt>.
///
/// \returns The integer square root of <tt>x</tt>.
///
/// \par Complexity
/// Logarithmic in <tt>x</tt>.
///
template <typename T>
T isqrt(const T x) {
  static_assert(std::is_unsigned<T>::value, "T must be unsigned type");
  constexpr T max_ans = (T{1} << std::numeric_limits<T>::digits / 2) - 1;
  T ans = std::min(max_ans, static_cast<T>(std::sqrtf(x)));
  // At this point, ans is an approximate square root.
  while (ans * ans > x)
    --ans;
  while (ans < max_ans && (ans + 1) * (ans + 1) <= x)
    ++ans;
  return ans;
}

} // end namespace djp

#endif // Header guard
