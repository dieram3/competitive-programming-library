//          Copyright Diego Ramírez March 2015, August 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
/// \file
/// \brief Defines functions related to the longest increasing subsequence.

#ifndef DJP_SEARCH_LONGEST_INCREASING_SUBSEQUENCE_HPP
#define DJP_SEARCH_LONGEST_INCREASING_SUBSEQUENCE_HPP

#include <vector>     // for std::vector
#include <algorithm>  // for std::for_each, std::lower_bound, std::is_sorted
#include <iterator>   // for std::iterator_traits
#include <functional> // for std::less
#include <cassert>    // for assert
#include <cstddef>    // for std::size_t
#include <cstdint>    // for SIZE_MAX

namespace djp {

/// \brief Calculates the size of the Longest monotonically Increasing
/// Subsequence.
///
/// The \e LIS is defined as the longest subsequence of <tt>[first, last)</tt>
/// such that <tt>comp(LIS[i], LIS[i + 1])</tt> evaluates to \c true for all
/// <tt>LIS[i], LIS[i +1]</tt> which are valid references.
///
/// \param first Iterator to the beginning of the target range.
/// \param last Iterator to the end of the target range.
/// \param comp Comparison function object which returns \c true if the first
/// argument is \em less than the second.
///
/// \par Complexity
/// <tt>O(N * log(N))</tt> comparisons, where
/// <tt>N = std::distance(first, last)</tt>.
///
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

/// \brief Calculates the size of the Longest Monotonically Increasing
/// Subsequence.
///
/// Calls <tt>calc_lis_size(first, last, std::less<>())</tt>.
///
template <class InputIt>
std::size_t calc_lis_size(InputIt first, InputIt last) {
  using T = typename std::iterator_traits<InputIt>::value_type;
  return calc_lis_size(first, last, std::less<T>());
}

/// \brief Finds a subsequence whose size is equal to the size of the longest
/// increasing subsequence (LIS).
///
/// Finds the longest possible subsequence \c L in \p seq, such that
/// <tt>comp(seq[l], seq[r])</tt> evaluates to \c true for any consecutive
/// pair of elements <tt>(l, r)</tt> in \c L. If there are multiple subsequences
/// with the longest size, any of them is returned.
///
/// \param seq The sequence to be examined.
/// \param comp The order comparator. Uses \c less or \c greater for strict
/// ordering and \c less_equal or \c greater_equal for non-strict ordering.
///
/// \returns A sorted vector containing the indices of elements in the chosen
/// subsequence.
///
/// \par Complexity
/// <tt>O(N * log(N))</tt> comparisons, where <tt>N = seq.size()</tt>.
///
template <class T, class Compare>
std::vector<size_t> longest_increasing_subsequence(const std::vector<T> &seq,
                                                   Compare comp) {
  if (seq.empty())
    return {};

  std::vector<size_t> tail;
  std::vector<size_t> parent(seq.size(), SIZE_MAX);
  auto value_comp = [&](size_t l, size_t r) { return comp(seq[l], seq[r]); };

  for (size_t i = 0; i != seq.size(); ++i) {
    auto it = std::lower_bound(tail.begin(), tail.end(), i, value_comp);
    parent[i] = (it == tail.begin()) ? SIZE_MAX : *(it - 1);
    if (it == tail.end())
      tail.push_back(i);
    else
      *it = i;
  }

  std::vector<size_t> lis(tail.size());
  auto rit = lis.rbegin();
  for (size_t i = tail.back(); i != SIZE_MAX; i = parent[i])
    *rit++ = i;

  assert(rit == lis.rend());
  return lis;
}

} // end namespace djp

#endif // Header guard
