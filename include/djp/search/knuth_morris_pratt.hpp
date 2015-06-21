//          Copyright Diego Ramírez April 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
// Note: The interface is inspired on C++ Extensions for Library Fundamentals
// Note: The source code is inspired on
// http://www.inf.fh-flensburg.de/lang/algorithmen/pattern/kmpen.htm
/// \file
/// \brief Defines the class \c kmp_searcher.

#ifndef DJP_SEARCH_KNUTH_MORRIS_PRATT_HPP
#define DJP_SEARCH_KNUTH_MORRIS_PRATT_HPP

#include <vector>  // for std::vector
#include <cstddef> // for std::ptrdiff_t

namespace djp {

/// \brief Function like class that searchs for a pattern using the
/// Knuth-Morris-Pratt algorithm.
template <typename RandomIt1>
class kmp_searcher {
  using diff_t = std::ptrdiff_t;
  RandomIt1 pattern_; // Iterator to the first element of the pattern.
  std::vector<diff_t> mismatch_; // Table to skip back on mismatch

public:
  /// \brief Constructs a \c kmp_searcher according to the given pattern.
  /// \param p_first The beginning of the pattern.
  /// \param p_last The end of the pattern.
  /// \pre The pattern shall not be empty i.e <tt>p_first < p_last</tt>.
  /// \par Complexity
  /// <tt>O(K)</tt>, where <tt>k = std::distance(p_first, p_last)</tt>
  kmp_searcher(RandomIt1 p_first, RandomIt1 p_last)
      : pattern_{p_first}, mismatch_(p_last - p_first + 1) {
    mismatch_[0] = -1;
    for (diff_t p_pos = 0, matched = -1; p_pos < pattern_length();) {
      while (matched >= 0 && pattern_[p_pos] != pattern_[matched])
        matched = mismatch_[matched];
      mismatch_[++p_pos] = ++matched;
    }
  }

  /// \brief Gets the length of the target pattern.
  /// \returns The length of the pattern used in the constructor.
  diff_t pattern_length() const { return mismatch_.size() - 1; }

  /// \brief Searches on the given text for the pattern given at construction
  /// time.
  /// \param first The beginning of the text.
  /// \param last The end of the text.
  /// \returns Iterator to first ocurrence of the pattern if it was found in the
  /// text. Otherwise returns \p last.
  /// \par Complexity
  /// <tt>O(N)</tt>, where <tt>N = std::distance(first, last)</tt>
  template <typename RandomIt2>
  RandomIt2 operator()(RandomIt2 first, RandomIt2 last) const {
    diff_t matched = 0;
    while (first != last) {
      while (matched >= 0 && *first != pattern_[matched])
        matched = mismatch_[matched];
      ++first;
      ++matched;
      if (matched == pattern_length())
        return first - matched;
    }
    return last;
  }
};

/// \brief Constructs a \c kmp_searcher using the given pattern.
/// \param p_first The beginning of the pattern.
/// \param p_last The end of the pattern.
/// \returns The constructed \c kmp_searcher.
template <typename RandomIt>
kmp_searcher<RandomIt> make_kmp_searcher(RandomIt p_first, RandomIt p_last) {
  return kmp_searcher<RandomIt>(p_first, p_last);
}

} // end namespace djp

#endif // Header guard
