//          Copyright Diego Ramírez April 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

// Note: The interface is inspired on C++ Extensions for Library Fundamentals
// Note: The source code is inspired on
// http://www.inf.fh-flensburg.de/lang/algorithmen/pattern/kmpen.htm

#ifndef DJP_SEARCH_KNUTH_MORRIS_PRATT_HPP
#define DJP_SEARCH_KNUTH_MORRIS_PRATT_HPP

#include <vector>
#include <cstddef>

namespace djp {

template <class RandomIt1>
class kmp_searcher {
  using diff_t = std::ptrdiff_t;
  RandomIt1 pattern_;  // Iterator to the first element of the pattern.
  std::vector<diff_t> mismatch_;  // Table to skip back on mismatch

 public:
  kmp_searcher(RandomIt1 p_first, RandomIt1 p_last)
      : pattern_{p_first}, mismatch_(p_last - p_first + 1) {
    mismatch_[0] = -1;
    for (diff_t p_pos = 0, matched = -1; p_pos < pattern_length();) {
      while (matched >= 0 && pattern_[p_pos] != pattern_[matched])
        matched = mismatch_[matched];
      mismatch_[++p_pos] = ++matched;
    }
  }

  diff_t pattern_length() const { return mismatch_.size() - 1; }

  template <class RandomIt2>
  RandomIt2 operator()(RandomIt2 first, RandomIt2 last) const {
    diff_t matched = 0;
    while (first != last) {
      while (matched >= 0 && *first != pattern_[matched])
        matched = mismatch_[matched];
      ++first;
      ++matched;
      if (matched == pattern_length()) return first - matched;
    }
    return last;
  }
};

/// \brief Returns a Knuth-Morris-Pratt searcher
/// The construction time has complexity O(k) where k == p_last - p_first
/// The returned searcher 'search_on' can be invoked as search_on(first, last)
/// where [first,last) is the text to be searched on. This will return an
/// iterator to the beginning of the first match on success, or last otherwise.
/// The search has complexity O(n) where n == last - first
/// Requires: p_last - p_first > 0, i.e pattern shall not be empty.
template <class RandomIt>
kmp_searcher<RandomIt> make_kmp_searcher(RandomIt p_first, RandomIt p_last) {
  return {p_first, p_last};
}

}  // namespace djp

#endif  // Header guard