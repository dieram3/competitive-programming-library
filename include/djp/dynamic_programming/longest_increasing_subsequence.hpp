//          Copyright Diego Ramírez March 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DJP_DYNAMIC_PROGRAMMING_LONGEST_INCREASING_SUBSEQUENCE_HPP
#define DJP_DYNAMIC_PROGRAMMING_LONGEST_INCREASING_SUBSEQUENCE_HPP

#include <vector>
#include <algorithm>
#include <iterator>

namespace djp {

// Calculates size of the Longest Monotonically Increasing Subsequence.
template <class InputIt>
size_t calc_lis_size(InputIt first, InputIt last) {
  using T = typename std::iterator_traits<InputIt>::value_type;
  std::vector<T> tail;  // tail[i] := last element of the list with size i + 1

  std::for_each(first, last, [&tail](const T& elem) {
    auto it = std::lower_bound(tail.begin(), tail.end(), elem);
    if (it == tail.end())
      tail.push_back(elem);
    else
      *it = elem;
  });
  return tail.size();
}

}  // djp

#endif  // Header guard
