//          Copyright Diego Ramírez April 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DJP_COMBINATORICS_NTH_PERMUTATION_HPP
#define DJP_COMBINATORICS_NTH_PERMUTATION_HPP

#include <vector>  // for std::vector
#include <cstddef> // for std::ptrdiff_t
#include <cstdint> // for std::uint64_t

namespace djp {

/// \brief Computes the nth smallest permutation of
/// <tt>[0, 1, ..., len - 1]</tt>
///
/// \param len The length of the range to get the permutation. The range will
/// be an increasing sequence of consecutive integers starting from 0.
/// \param nth The number of permutation required.
///
/// \pre <tt>len > 0</tt>
/// \pre <tt>factorial(len - 1) < 2<sup>64</sup></tt> i.e <tt>len <= 21</tt>
/// \pre <tt>nth < factorial(len)</tt>
///
/// \returns A \c vector containing the \p nth smallest permutation of the
/// range.
///
/// \par Complexity
/// <tt>O(N<sup>2</sup>)</tt> where <tt>N = len</tt>.
///
inline std::vector<size_t> nth_permutation(const size_t len, uint64_t nth) {
  std::vector<size_t> perm(len);
  std::vector<uint64_t> fact(len);

  // compute factorials
  fact[0] = 1;
  for (size_t k = 1; k < len; ++k)
    fact[k] = k * fact[k - 1];

  // compute factoradic
  for (size_t k = 0; k < len; ++k) {
    const auto base = fact[(len - 1) - k];
    perm[k] = nth / base;
    nth %= base;
  }

  // Readjust values to obtain the permutation.
  for (ptrdiff_t k = len - 1; k > 0; --k) {
    for (ptrdiff_t j = k - 1; j >= 0; --j) {
      if (perm[k] >= perm[j])
        ++perm[k];
    }
  }

  return perm;
}

} // namespace djp

#endif // Header guard
