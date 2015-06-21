//          Copyright Diego Ramírez June 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
/// \todo Put \c lcp_querier in a different file.

#ifndef DJP_STRINGS_LCP_ARRAY_HPP
#define DJP_STRINGS_LCP_ARRAY_HPP

#include <djp/data_structure/segment_tree.hpp>
#include <algorithm>
#include <string>
#include <tuple>
#include <vector>
#include <cstddef>

namespace djp {

/// \brief Generates the LCP array of the given string.
///
/// \param str The string to get the LCP array.
/// \param sa The suffix array of \p str.
///
/// \par Complexity
/// <tt>O(N)</tt> time, <tt>O(N)</tt> space.
///
/// \note <tt>lcp[i] :=</tt> longest common prefix between <tt>sa[i]</tt> and
/// <tt>sa[i + 1]</tt>
///
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
///        any two suffixes of a given string.
class lcp_querier {
  struct minimum {
    size_t operator()(size_t x, size_t y) const { return std::min(x, y); }
  };

public:
  /// \brief Contructs the LCP Querier.
  /// \param str The target string.
  /// \param sa The suffix array of the given string.
  /// \pre The target string \p str shall not be empty.
  /// \par Complexity
  /// <tt>O(N)</tt>, where <tt>N = str.size()</tt>
  ///
  lcp_querier(const std::string &str, const std::vector<size_t> &sa)
      : rank_(sa.size()) {
    const size_t N = sa.size();
    for (size_t i = 0; i < N; ++i)
      rank_[sa[i]] = i; // inverse suffix array.

    const auto lcp_array = make_lcp_array(str, sa);
    rmq_.assign(begin(lcp_array), end(lcp_array));
  }

  /// \brief Computes the LCP between a pair of suffixes.
  /// \param i The first suffix.
  /// \param j The second suffix.
  /// \par Complexity
  /// <tt>O(log(N))</tt> where <tt>N = </tt>The size of the target string.
  ///
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

} // end namespace djp

#endif // Header guard
