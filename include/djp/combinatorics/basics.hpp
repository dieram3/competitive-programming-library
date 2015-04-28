//          Copyright Diego Ramírez April 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DJP_COMBINATORICS_BASICS_HPP
#define DJP_COMBINATORICS_BASICS_HPP

#include <vector>
#include <cstddef>
#include <cstdint>
#include <iterator>
#include <numeric>

namespace djp {

/// \brief Computes the nth permutation of [0, 1, ..., len - 1]
/// Requires: len != 0
/// Requires: factorial(len - 1) < 2^64 i.e len <= 21
/// Requires: nth < factorial(len)
/// Complexity: O(N^2) where N == len
std::vector<size_t> nth_permutation(const size_t len, uint64_t nth) {
  std::vector<size_t> perm(len);
  std::vector<uint64_t> fact(len);

  // compute factorials
  fact[0] = 1;
  for (size_t k = 1; k < len; ++k) fact[k] = k * fact[k - 1];

  // compute factoradic
  for (size_t k = 0; k < len; ++k) {
    const auto base = fact[(len - 1) - k];
    perm[k] = nth / base;
    nth %= base;
  }

  // Readjust values to obtain the permutation.
  for (ptrdiff_t k = len - 1; k > 0; --k) {
    for (ptrdiff_t j = k - 1; j >= 0; --j) {
      if (perm[k] >= perm[j]) ++perm[k];
    }
  }

  return perm;
}

}  // namespace djp

#endif  // Header guard
