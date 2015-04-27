//          Copyright Diego Ramírez April 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DJP_GRAPH_ADJACENCY_LIST_HPP
#define DJP_GRAPH_ADJACENCY_LIST_HPP

#include <string>
#include <vector>
#include <algorithm>
#include <numeric>
#include <cstddef>
#include <djp/sorting/counting_sort.hpp>

namespace djp {

/// \brief Computes the suffix array.
/// Complexity: O(n*log(n)) time, O(n) space
std::vector<size_t> make_suffix_array(const std::string& str) {
  if (str.empty()) return {};
  const size_t N = str.size();
  std::vector<size_t> sa(N), rank(2 * N), tmp(N);

  auto char_key = [](unsigned char c) { return c + 1; };
  size_t n_keys = 257;  // From 1 to 256. Zero is used for empty substrings.
  std::iota(begin(sa), end(sa), size_t{});
  std::transform(begin(str), end(str), begin(rank), char_key);

  for (size_t gap = 1;; gap *= 2) {
    // most significant rank, least significant rank
    auto msr = [&rank](size_t suffix) { return rank[suffix]; };
    auto lsr = [&rank, gap](size_t suffix) { return rank[suffix + gap]; };
    counting_sort_copy(begin(sa), end(sa), begin(tmp), n_keys, lsr);
    counting_sort_copy(begin(tmp), end(tmp), begin(sa), n_keys, msr);
    auto sne = [msr, lsr](size_t s1, size_t s2) {
      return msr(s1) != msr(s2) || lsr(s1) != lsr(s2);  // suffix not equal
    };
    tmp[0] = 1;
    for (size_t i = 1; i < N; ++i) tmp[i] = tmp[i - 1] + sne(sa[i], sa[i - 1]);
    if (tmp.back() == N) break;
    n_keys = tmp.back() + 1;
    for (size_t i = 0; i < N; ++i) rank[sa[i]] = tmp[i];
  }
  return sa;
}

/// \brief Computes the LCP array.
/// Complexity: O(n) time, O(n) space
/// \note lcp[i] := longest common prefix between sa[i] and sa[i+1]
std::vector<size_t> make_lcp_array(const std::string& str,
                                   const std::vector<size_t>& sa) {
  const size_t N = sa.size();
  std::vector<size_t> rank(N), lcp(N - 1);

  for (size_t i = 0; i != N; ++i) rank[sa[i]] = i;  // inverse suffix array.

  for (size_t i = 0, len = 0; i < N; ++i) {
    if (rank[i] == N - 1) continue;
    for (size_t j = sa[rank[i] + 1]; str[i + len] == str[j + len];) ++len;
    lcp[rank[i]] = len;
    if (len) --len;
  }
  return lcp;
}

}  // namespace djp

#endif  // Header guard
