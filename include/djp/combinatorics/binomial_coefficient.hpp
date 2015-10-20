//          Copyright Diego Ramírez October 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DJP_COMBINATORICS_BINOMIAL_COEFFICIENT_HPP
#define DJP_COMBINATORICS_BINOMIAL_COEFFICIENT_HPP

#include <djp/utility/matrix.hpp>

namespace djp {

/// \brief Generates a table of binomial coefficient values by using a DP
/// approach.
///
/// Generates a square matrix C of <tt>(n_max + 1)</tt> dimensions such that
/// <tt>C[{n, k}]</tt> evaluates to <tt>C(n, k)</tt> for all <tt>0 <= n <=
/// n_max && 0 <= k <= n</tt>.
///
/// \param n_max The upper limit of <tt>n</tt>.
///
/// \returns The generated matrix <tt>C</tt>.
///
/// \par Complexity
/// Quadratic in <tt>n_max</tt>.
///
/// \note
/// If <tt>n_max <= 34</tt>, all the computed values fit in an unsigned 32-bit
/// integer. If <tt>n_max <= 67</tt>, all the computed values fit in an unsigned
/// 64-bit integer.
///
template <typename T>
matrix<T> gen_binomial_coefficient_matrix(const T n_max) {
  matrix<T> C({n_max + 1, n_max + 1});
  for (T n = 0; n <= n_max; ++n) {
    C[{n, 0}] = 1;
    C[{n, n}] = 1;
    for (T k = 1; k < n; ++k)
      C[{n, k}] = C[{n - 1, k - 1}] + C[{n - 1, k}];
  }
  return C;
}

} // end namespace djp

#endif // Header guard
