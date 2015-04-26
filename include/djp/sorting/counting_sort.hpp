//          Copyright Diego Ramirez April 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DJP_SORTING_COUNTING_SORT_HPP
#define DJP_SORTING_COUNTING_SORT_HPP

#include <vector>
#include <iterator>
#include <cassert>

namespace djp {

/// \brief Stably sorts the range [first, last) according to a given key.
/// Complexity: O(N + K) Where N == std::distance(first, last) and
/// K == num_keys.
/// key(x) must return a value contained in [0, num_keys) convertible to size_t
/// being 'x' any object copied from the range [first, last)
template <class RandomIt, class Key>
void counting_sort(RandomIt first, RandomIt last, size_t num_keys, Key key) {
  using T = typename std::iterator_traits<RandomIt>::value_type;
  std::vector<size_t> cnt(num_keys);
  std::vector<T> elems(first, last);  // Make move iterator if necessary.
  for (const T& x : elems) ++cnt[key(x)];
  for (size_t k = 0, acc_freq = 0, freq; k != num_keys; ++k)
    freq = cnt[k], cnt[k] = acc_freq, acc_freq += freq;
  for (const T& x : elems) first[cnt[key(x)]++] = std::move(x);
}

}  // namespace djp

#endif  // Header guard
