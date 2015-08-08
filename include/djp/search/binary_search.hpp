//          Copyright Diego Ramírez August 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DJP_SEARCH_BINARY_SEARCH_HPP
#define DJP_SEARCH_BINARY_SEARCH_HPP

#include <algorithm>

namespace djp {

/// \brief Binary searches for the first integer that does not satisfy the
/// predicate.
///
/// Finds a integer \c l in the range <tt>[a, b)</tt> such that <tt>p(x)</tt>
/// evaluates to \c true for all integers in the range <tt>[a, l)</tt> and
/// evaluates to \c false for all integers in the range <tt>[l, b)</tt>.
///
/// \param a The inclusive lower bound.
/// \param b The exclusive upper bound.
/// \param p The unary predicate.
///
/// \returns The first integer that does not satisfy the predicate, or \p b if
/// no such integer is found.
///
/// \pre <tt>a <= b</tt>.
/// \pre The given range is partitioned by the predicate \p p.
/// \pre The result of <tt>(b - a)</tt> fits in the type \c T.
///
/// \par Complexity
/// <tt>O(log(N))</tt>, where <tt>N = b - a</tt>.
///
template <typename T, typename Pred>
T int_binary_search(T a, T b, Pred p) {
  while (a != b) {
    const T mid = a + (b - a) / 2;
    if (p(mid))
      a = mid + 1;
    else
      b = mid;
  }
  return a;
}

} // end namespace djp

#endif // Header guard
