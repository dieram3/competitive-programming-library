//          Copyright Diego Ramírez April 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DJP_GRAPH_ADJACENCY_LIST_HPP
#define DJP_GRAPH_ADJACENCY_LIST_HPP

#include <string>
#include <vector>
#include <algorithm>
#include <cstddef>

namespace djp {

/// \brief Computes the suffix array.
/// Complexity: O(n*log^2(n)) time, O(n) space
std::vector<size_t> make_suffix_array(const std::string& str) {
  if (str.empty()) return {};

  const char* const S = str.c_str();
  const size_t N = str.size();
  std::vector<size_t> sa(N), rank(N), tmp(N);

  for (size_t i = 0; i != N; ++i) sa[i] = i, rank[i] = S[i];

  for (size_t gap = 1;; gap *= 2) {
    auto suf_cmp = [&rank, gap, N](size_t i, size_t j) {
      if (rank[i] != rank[j]) return rank[i] < rank[j];
      i += gap;
      j += gap;
      return (i < N && j < N) ? rank[i] < rank[j] : i > j;
    };

    std::sort(sa.begin(), sa.end(), suf_cmp);
    for (size_t i = 0; i < N - 1; ++i)
      tmp[i + 1] = tmp[i] + suf_cmp(sa[i], sa[i + 1]);
    for (size_t i = 0; i < N; ++i) rank[sa[i]] = tmp[i];
    if (tmp[N - 1] == N - 1) break;
  }
  return sa;
}

/// \brief Computes the LCP array.
/// Complexity: O(n) time, O(n) space
/// \note lcp[i] := longest common prefix between sa[i] and sa[i+1]
std::vector<size_t> make_lcp_array(const std::string& str,
                                   const std::vector<size_t>& sa) {
  const char* const S = str.c_str();
  const size_t N = sa.size();
  std::vector<size_t> rank(N), lcp(N - 1);

  for (size_t i = 0; i != N; ++i) rank[sa[i]] = i;

  for (size_t i = 0, k = 0; i != N; ++i) {
    if (rank[i] == N - 1) continue;
    for (size_t j = sa[rank[i] + 1]; S[i + k] == S[j + k];) ++k;
    lcp[rank[i]] = k;
    if (k) --k;
  }
  return lcp;
}

}  // namespace djp

#endif  // Header guard
