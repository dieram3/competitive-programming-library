//          Copyright Diego Ramírez April 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DJP_GRAPH_ADJACENCY_LIST_HPP
#define DJP_GRAPH_ADJACENCY_LIST_HPP

#include <djp/data_structure/segment_tree.hpp>
#include <djp/sorting/counting_sort.hpp>

#include <algorithm> // for std::transform, std::minmax
#include <iterator>  // for std::begin, std::end
#include <numeric>   // for std::iota
#include <vector>    // for std::vector
#include <string>    // for std::string
#include <tuple>     // for std::tie

#include <cstddef> // for std::size_t

namespace djp {

/// \brief Computes the suffix array.
/// Complexity: O(n*log(n)) time, O(n) space
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

/// \brief Computes the LCP array.
/// Complexity: O(n) time, O(n) space
/// \note lcp[i] := longest common prefix between sa[i] and sa[i+1]
inline std::vector<size_t> make_lcp_array(const std::string &str,
                                          const std::vector<size_t> &sa) {
  const size_t N = str.size();
  std::vector<size_t> rank(N), lcp(N - 1);

  for (size_t i = 0; i != N; ++i)
    rank[sa[i]] = i; // inverse suffix array.

  for (size_t i = 0, len = 0; i < N; ++i) {
    if (rank[i] == N - 1)
      continue;
    for (size_t j = sa[rank[i] + 1]; str[i + len] == str[j + len];)
      ++len;
    lcp[rank[i]] = len;
    if (len)
      --len;
  }
  return lcp;
}

/// \brief Function like class used to compute the longest common prefix between
///        any two substrings of the given string.
class lcp_querier {
  struct minimum {
    size_t operator()(size_t x, size_t y) const { return std::min(x, y); }
  };

public:
  /// \brief Contructs the LCP Querier
  /// \param str The target string.
  /// \param sa The suffix array of the given string.
  /// \pre \p The target string \p str shall not be empty.
  /// Complexity: O(N) where N == str.size()
  lcp_querier(const std::string &str, const std::vector<size_t> &sa)
      : rank_(sa.size()) {
    const size_t N = sa.size();
    for (size_t i = 0; i < N; ++i)
      rank_[sa[i]] = i; // inverse suffix array.

    const auto lcp_array = make_lcp_array(str, sa);
    rmq_.assign(begin(lcp_array), end(lcp_array));
  }

  /// \brief Computes the LCP between substrings i and j
  /// Complexity: O(log(N)) where N == The size of the target string.
  size_t operator()(size_t i, size_t j) const {
    if (i == j)
      return rank_.size() - i;
    std::size_t rank1, rank2;
    std::tie(rank1, rank2) = std::minmax(rank_.at(i), rank_.at(j));
    return rmq_.accumulate(rank1, rank2);
  }

private:
  std::vector<size_t> rank_;
  segment_tree<size_t, minimum> rmq_;
};

// TODO: Add a linear version of make_suffix_array. It is required for some
//       contests

} // namespace djp

#endif // Header guard
