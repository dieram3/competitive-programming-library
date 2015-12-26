//          Copyright Diego Ramírez 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
/// \file
/// \brief Defines the function \c make_suffix_array.

#ifndef DJP_STRINGS_SUFFIX_ARRAY_HPP
#define DJP_STRINGS_SUFFIX_ARRAY_HPP

#include <djp/sorting/counting_sort.hpp>

#include <algorithm> // transform
#include <cstddef>   // size_t
#include <iterator>  // begin, end
#include <numeric>   // iota
#include <string>    // string
#include <vector>    // vector

namespace djp {

/// \brief Computes the suffix array of a given string.
///
/// \param str The input string.
///
/// \returns A vector containing all the suffix indices of \p str sorted
/// lexicographically i.e the suffix array.
///
/// \par Complexity
/// <tt>O(n * log(n))</tt> time and <tt>O(n)</tt> space, where
/// <tt>n = str.size()</tt>.
///
inline std::vector<size_t> make_suffix_array(const std::string &str) {
  if (str.empty())
    return {};

  const size_t N = str.size();
  std::vector<size_t> sa(N), rank(2 * N), tmp(N);

  auto char_key = [](unsigned char c) { return c + 1; };
  size_t n_keys = 257; // From 1 to 256. Zero is used for empty substrings.
  std::iota(begin(sa), end(sa), size_t{});
  std::transform(begin(str), end(str), begin(rank), char_key);

  for (size_t gap = 1;; gap *= 2) {
    // most significant rank, least significant rank
    auto msr = [&rank](size_t suffix) { return rank[suffix]; };
    auto lsr = [&rank, gap](size_t suffix) { return rank[suffix + gap]; };
    counting_sort_copy(begin(sa), end(sa), begin(tmp), n_keys, lsr);
    counting_sort_copy(begin(tmp), end(tmp), begin(sa), n_keys, msr);
    auto sne = [msr, lsr](size_t s1, size_t s2) {
      return msr(s1) != msr(s2) || lsr(s1) != lsr(s2); // suffix not equal
    };
    tmp[0] = 1;
    for (size_t i = 1; i < N; ++i)
      tmp[i] = tmp[i - 1] + sne(sa[i], sa[i - 1]);
    if (tmp.back() == N)
      break;
    n_keys = tmp.back() + 1;
    for (size_t i = 0; i < N; ++i)
      rank[sa[i]] = tmp[i];
  }
  return sa;
}

} // end namespace djp

#endif // Header guard
