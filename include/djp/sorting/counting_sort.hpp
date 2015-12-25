//          Copyright Diego Ramirez April 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
/// \file
/// \brief Defines the counting sort algorithm.

#ifndef DJP_SORTING_COUNTING_SORT_HPP
#define DJP_SORTING_COUNTING_SORT_HPP

#include <algorithm> // for_each
#include <iterator>  // iterator_traits
#include <vector>    // vector

namespace djp {

template <typename ForwardIt, typename RandomIt, typename Key>
void counting_sort_copy(ForwardIt first, ForwardIt last, RandomIt d_first,
                        size_t num_keys, Key key) {
  using reference = typename std::iterator_traits<ForwardIt>::reference;
  using diff_t = typename std::iterator_traits<RandomIt>::difference_type;
  std::vector<diff_t> cnt(num_keys);
  std::for_each(first, last, [&](reference x) { ++cnt[key(x)]; });
  diff_t acc_freq = 0, freq;
  for (auto &cnt_k : cnt)
    freq = cnt_k, cnt_k = acc_freq, acc_freq += freq;
  std::for_each(first, last, [&](reference x) { d_first[cnt[key(x)]++] = x; });
}

/// \brief Stably sorts a range of elements according to a given key.
///
/// \param first The beginning of the range to be sorted.
/// \param last The end of the range to be sorted.
/// \param num_keys The number of different keys that the key function can
/// generate.
/// \param key Function object which returns the key value of its argument (any
/// element from the range). The returned value must be an integer number
/// convertible to \c size_t inside the range <tt>[0, num_keys)</tt>.
///
/// \par Complexity
/// O(N + K) where <tt>N = std::distance(first, last)</tt> and
/// <tt>K = num_keys</tt>.
///
template <typename RandomIt, typename Key>
void counting_sort(RandomIt first, RandomIt last, size_t num_keys, Key key) {
  using T = typename std::iterator_traits<RandomIt>::value_type;
  std::vector<T> elems(first, last);
  counting_sort_copy(elems.begin(), elems.end(), first, num_keys, key);
}

} // end namespace djp

#endif // Header guard
