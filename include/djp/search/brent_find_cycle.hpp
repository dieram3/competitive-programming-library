//          Copyright Diego Ramírez September 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DJP_SEARCH_BRENT_FIND_CYCLE_HPP
#define DJP_SEARCH_BRENT_FIND_CYCLE_HPP

#include <utility> // for std::pair
#include <cstddef> // for std::size_t

namespace djp {

/// \brief Finds a cycle in an iterated function.
///
/// Uses the Brent's algorithm to find the cycle length and the cycle start in
/// an iterated function according to an initial state. This function finds two
/// values: \p lam and \p mu. The cycle length \p lam is defined as the smallest
/// value that satisfies the loop condition <tt>x_i == x_(i + lam)</tt> for all
/// <tt>i >= mu</tt> and <tt>mu >= 0</tt>. The cycle start \p mu is defined as
/// the smallest value needed to comply the above condition, in other words, the
/// number of iterations needed to reach the loop.
///
/// \param f The function to be examined.
/// \param x0 The initial state.
///
/// \pre The function \p f must have a cycle reachable from \p x0.
///
/// \returns A pair containing <tt>(lam, mu)</tt>.
///
/// \par Complexity
/// <tt>O(lam + mu)</tt> applications of \p f.
///
template <typename F, typename T>
std::pair<size_t, size_t> brent_find_cycle(F f, T x0) {
  size_t power = 1, lam = 1;
  T tortoise = x0;
  T hare = f(x0);
  while (tortoise != hare) {
    if (power == lam) {
      tortoise = hare;
      power *= 2;
      lam = 0;
    }
    hare = f(hare);
    ++lam;
  }
  // At this point, the cycle length 'lam' is known.
  size_t mu = 0;
  tortoise = hare = x0;
  for (size_t rep = 0; rep != lam; ++rep)
    hare = f(hare);
  while (tortoise != hare) {
    tortoise = f(tortoise);
    hare = f(hare);
    ++mu;
  }
  return {lam, mu};
}

} // end namespace djp

#endif // Header guard
