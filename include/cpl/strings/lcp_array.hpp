//          Copyright Diego Ramírez 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef CPL_STRINGS_LCP_ARRAY_HPP
#define CPL_STRINGS_LCP_ARRAY_HPP

#include <cpl/data_structure/segment_tree.hpp>

#include <algorithm> // max, minmax
#include <cstddef>   // size_t
#include <string>    // string
#include <tuple>     // tie
#include <vector>    // vector

namespace cpl {

/// \brief Generates the LCP array of the given string.
///
/// Generates an array \c lcp of size \c sa.size() where \c lcp[i] is
/// defined as the longest common prefix between the suffixes \c sa[i] and
/// \c sa[i+1]. For convenience, \c lcp[sa.size()-1] is left as zero.
///
/// \param str The string to get the LCP array from.
/// \param sa The suffix array of \p str.
///
/// \pre <tt>str.size() == sa.size()</tt>.
///
/// \returns The generated LCP array.
///
/// \par Complexity
/// <tt>O(N)</tt> time and <tt>O(N)</tt> space, where <tt>N = sa.size()</tt>.
///
inline std::vector<size_t> make_lcp_array(const std::string &str,
                                          const std::vector<size_t> &sa) {
  const size_t N = str.size();
  std::vector<size_t> rank(N), lcp(N);

  for (size_t i = 0; i != N; ++i)
    rank[sa[i]] = i; // inverse suffix array.

  for (size_t i = 0, len = 0; i < N; ++i) {
    if (rank[i] == N - 1)
      continue;
    {
      const size_t j = sa[rank[i] + 1];
      const size_t max_len = str.size() - std::max(i, j);
      while (len < max_len && str[i + len] == str[j + len])
        ++len;
    }
    lcp[rank[i]] = len;
    if (len)
      --len;
  }
  return lcp;
}

/// \brief Function like class used to compute the longest common prefix between
///        any pair of suffixes of a given string.
///
class lcp_querier {
  struct minimum {
    size_t operator()(size_t x, size_t y) const { return std::min(x, y); }
  };

public:
  /// \brief Constructs the LCP Querier.
  ///
  /// \param str The input string.
  /// \param sa The suffix array of the given string.
  ///
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
  ///
  /// \param i The first suffix.
  /// \param j The second suffix.
  ///
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

} // end namespace cpl

#endif // Header guard
