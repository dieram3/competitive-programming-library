//          Copyright Diego Ramírez March 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
/// \file
/// \brief Defines functions related to the longest increasing subsequence.

#ifndef DJP_MISCELLANEOUS_LONGEST_INCREASING_SUBSEQUENCE_HPP
#define DJP_MISCELLANEOUS_LONGEST_INCREASING_SUBSEQUENCE_HPP

#include <vector>
#include <algorithm>
#include <iterator>
#include <functional>

namespace djp {

/// \brief Calculates size of the Longest monotonically Increasing Subsequence.
///
/// The \e LIS is defined as the longest subsequence of <tt>[first, last)</tt>
/// such that <tt>comp(LIS[i], LIS[i + 1])</tt> evaluates to \c true for all
/// <tt>LIS[i], LIS[i +1]</tt> which are valid references.
///
/// \param first Iterator to the beginning of the target range.
/// \param last Iterator to the end of the target range.
/// \param comp Comparison function object which returns \c true if the first
/// argument is \em less than the second.
/// \par Complexity
/// <tt>O(N * log(N))</tt> comparisons, where <tt>N = std::distance(first,
/// last)</tt>.
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

/// \brief Calculates size of the Longest Monotonically Increasing Subsequence.
///
/// Calls <tt>calc_lis_size(first, last, std::less<>())</tt>.
template <class InputIt>
std::size_t calc_lis_size(InputIt first, InputIt last) {
  using T = typename std::iterator_traits<InputIt>::value_type;
  return calc_lis_size(first, last, std::less<T>());
}

} // end namespace djp

#endif // Header guard
