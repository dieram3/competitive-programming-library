//          Copyright Diego Ramirez 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef CPL_BITS_GRAY_CODE_HPP
#define CPL_BITS_GRAY_CODE_HPP

namespace cpl {

/// \brief Finds the \a nth gray code.
///
/// \param nth The position (rank) of the desired gray code.
///
/// \returns The \a nth gray code.
///
/// \par Complexity
/// Constant.
///
template <typename T>
T nth_gray_code(T nth) {
  return (nth >> 1) ^ nth;
}

/// \brief Finds the position (rank) of the given gray code.
///
/// \param code The gray code to be examined.
///
/// \returns The rank of the given gray code.
///
/// \par Complexity
/// Logarithmic in \p code.
///
template <typename T>
T rank_gray_code(T code) {
  for (T mask = code >> 1; mask; mask >>= 1)
    code ^= mask;
  return code;
}

} // end namespace cpl

#endif // Header guard
