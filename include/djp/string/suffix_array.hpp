//          Copyright Diego Ramírez April 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DJP_GRAPH_ADJACENCY_LIST_HPP
#define DJP_GRAPH_ADJACENCY_LIST_HPP

#include <string>
#include <vector>
#include <numeric>
#include <cstddef>
#include <djp/sorting/counting_sort.hpp>

namespace djp {

/// \brief Computes the suffix array.
/// Complexity: O(n*log(n)) time, O(n) space
std::vector<size_t> make_suffix_array(const std::string& str) {
  if (str.empty()) return {};

  const size_t N = str.size();
  std::vector<size_t> sa(N), rank(2 * N), tmp(N, 1);

  for (size_t i = 0; i != N; ++i) sa[i] = i, rank[i] = str[i];
  size_t num_keys = 256;

  for (size_t gap = 1; tmp.back() != N; gap *= 2, num_keys = tmp.back() + 1) {
    auto ms_rank = [&rank](size_t suffix) { return rank[suffix]; };
    auto ls_rank = [gap, &rank](size_t suffix) { return rank[suffix + gap]; };
    counting_sort(sa.begin(), sa.end(), num_keys, ls_rank);
    counting_sort(sa.begin(), sa.end(), num_keys, ms_rank);

    auto suff_ne = [=](size_t suff_a, size_t suff_b) {
      return ms_rank(suff_a) != ms_rank(suff_b) ||
             ls_rank(suff_a) != ls_rank(suff_b);
    };

    for (size_t i = 1; i != N; ++i) tmp[i] = suff_ne(sa[i], sa[i - 1]);
    std::partial_sum(tmp.begin(), tmp.end(), tmp.begin());
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

  for (size_t i = 0; i != N; ++i) rank[sa[i]] = i;

  for (size_t i = 0, k = 0; i < N; ++i) {
    if (rank[i] == N - 1) continue;
    for (size_t j = sa[rank[i] + 1]; str[i + k] == str[j + k];) ++k;
    lcp[rank[i]] = k;
    if (k) --k;
  }
  return lcp;
}

}  // namespace djp

#endif  // Header guard
