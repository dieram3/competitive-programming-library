//          Copyright Diego Ramirez April 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DJP_SORTING_COUNTING_SORT_HPP
#define DJP_SORTING_COUNTING_SORT_HPP

#include <vector>
#include <iterator>
#include <numeric>
#include <utility>

namespace djp {

template <class ForwardIt, class RandomIt, class Key>
void counting_sort_copy(ForwardIt first, ForwardIt last, RandomIt d_first,
                        size_t num_keys, Key key) {
  using reference = typename std::iterator_traits<ForwardIt>::reference;
  using diff_t = typename std::iterator_traits<RandomIt>::difference_type;
  std::vector<diff_t> cnt(num_keys);
  std::for_each(first, last, [&](reference x) { ++cnt[key(x)]; });
  diff_t acc_freq = 0, freq;
  for (auto& cnt_k : cnt) freq = cnt_k, cnt_k = acc_freq, acc_freq += freq;
  std::for_each(first, last, [&](reference x) { d_first[cnt[key(x)]++] = x; });
}

/// \brief Stably sorts the range [first, last) according to a given key.
/// Complexity: O(N + K) Where N == std::distance(first, last) and
/// K == num_keys.
/// key(x) must return a value contained in [0, num_keys) convertible to size_t
/// being 'x' any object copied from the range [first, last)
template <class RandomIt, class Key>
void counting_sort(RandomIt first, RandomIt last, size_t num_keys, Key key) {
  using T = typename std::iterator_traits<RandomIt>::value_type;
  std::vector<T> elems(first, last);
  counting_sort_copy(elems.begin(), elems.end(), first, num_keys, key);
}

}  // namespace djp

#endif  // Header guard
