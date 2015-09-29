//          Copyright Diego Ramírez September 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DJP_NUMBER_THEORY_EULER_PHI_HPP
#define DJP_NUMBER_THEORY_EULER_PHI_HPP

#include <vector> // For std::vector

namespace djp {

/// \brief Computes the totient of a positive integer.
///
/// Computes the totient of <tt>n</tt>, that is, the number of positive integers
/// less than or equal to <tt>n</tt> that are relatively prime to <tt>n</tt>.
///
/// \param n The input integer.
/// \param prime_factors A sorted \c vector containing the prime factors of
/// <tt>n</tt>.
///
/// \pre <tt>n > 0</tt>
///
/// \returns The totient of <tt>n</tt>.
///
/// \par Complexity
/// Linear in <tt>prime_factors.size()</tt>.
///
template <typename T>
T euler_phi(const T n, const std::vector<T> &prime_factors) {
  T ans = n;
  T last_pf = 0;
  for (const T pf : prime_factors) {
    if (pf == last_pf)
      continue;
    ans -= ans / pf;
    last_pf = pf;
  }
  return ans;
}

} // end namespace djp

#endif // Header guard
