//          Copyright Diego Ramírez March 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DJP_MISCELLANEOUS_LONGEST_INCREASING_SUBSEQUENCE_HPP
#define DJP_MISCELLANEOUS_LONGEST_INCREASING_SUBSEQUENCE_HPP

#include <vector>
#include <algorithm>
#include <iterator>
#include <functional>

namespace djp {

/// \brief Calculates size of the Longest Monotonically Increasing Subsequence.
/// Complexity: O(N*log(N)) comparisons, where N == std::distance(first, last)
/// comp(lhs, rhs) must returns true if lhs is considered strictly less than
/// rhs
/// \note: If you need a longest increasing that is not Monotonically Increasing
/// you can use a <= or >= compare
template <class InputIt, class Compare>
std::size_t calc_lis_size(InputIt first, InputIt last, Compare comp) {
  using T = typename std::iterator_traits<InputIt>::value_type;
  std::vector<T> tail; // tail[i] := last element of the list with size i + 1

  std::for_each(first, last, [&tail, comp](const T &elem) {
    auto it = std::lower_bound(tail.begin(), tail.end(), elem, comp);
    if (it == tail.end())
      tail.push_back(elem);
    else
      *it = elem;
  });
  return tail.size();
}

template <class InputIt>
std::size_t calc_lis_size(InputIt first, InputIt last) {
  using T = typename std::iterator_traits<InputIt>::value_type;
  return calc_lis_size(first, last, std::less<T>());
}

} // djp

#endif // Header guard
