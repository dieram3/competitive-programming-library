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
  using value_type = typename std::iterator_traits<RandomIt>::value_type;

  std::vector<size_t> cnt(num_keys);
  std::vector<value_type> elems(std::make_move_iterator(first),
                                std::make_move_iterator(last));

  // Count frequencies. Now cnt[k] == the frequency of elements with key 'k'.
  for (const auto& x : elems) ++cnt[key(x)];

  // Transform cnt. Now cnt[k] contains the index where the first (or next)
  // element with key 'k' shall be placed.
  size_t acc_freq = 0;
  for (size_t k = 0; k != num_keys; ++k) {
    size_t freq = cnt[k];
    cnt[k] = acc_freq;
    acc_freq += freq;
  }
  assert(acc_freq == elems.size());

  // Sort the range preserving order of inputs with equal keys
  for (const auto& x : elems) first[cnt[key(x)]++] = std::move(x);
}

}  // namespace djp

#endif  // Header guard
