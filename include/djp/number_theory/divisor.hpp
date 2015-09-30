//          Copyright Diego Ramírez June, September 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
/// \file
/// \brief Defines several functions related to divisors.

#ifndef DJP_NUMBER_THEORY_DIVISOR_HPP
#define DJP_NUMBER_THEORY_DIVISOR_HPP

#include <algorithm> // for std::find_if_not, std::sort
#include <vector>    // for std::vector
#include <cstddef>   // for std::size_t

namespace djp {

/// \brief Counts the number of positive divisors of a positive integer.
///
/// \param prime_factors A sorted vector containing the prime factors of the
/// integer to be examined.
///
/// \returns The number of positive divisors.
///
/// \pre Complexity
/// Linear in <tt>prime_factors.size()</tt>.
///
template <typename T>
T count_divisors(const std::vector<T> &prime_factors) {
  T ans = 1;
  auto it = prime_factors.begin();
  while (it != prime_factors.end()) {
    auto is_current_pf = [it](const T &pf) { return pf == *it; };
    auto next_it = std::find_if_not(it, prime_factors.end(), is_current_pf);
    ans *= static_cast<T>(1 + (next_it - it));
    it = next_it;
  }
  return ans;
}

/// \brief Computes the divisor sum of a positive integer.
///
/// \param prime_factors A sorted vector containing the prime factors of the
/// number to be examined.
///
/// \returns The sum of all positive divisors.
///
/// \par Complexity
/// Linear in <tt>prime_factors.size()</tt>.
///
template <typename T>
T sum_divisors(const std::vector<T> &prime_factors) {
  T ans = 1;
  auto it = prime_factors.begin();
  while (it != prime_factors.end()) {
    const T pf = *it;
    T sum = 1, base = 1;
    for (; it != prime_factors.end() && *it == pf; ++it) {
      base *= pf;
      sum += base;
    }
    ans *= sum;
  }
  return ans;
}

/// \brief Generates all positive divisors of a positive integer.
///
/// \param prime_factors A sorted \c vector containing the prime factors of the
/// integer to be examined.
///
/// \returns A sorted vector containing the generated divisors.
///
/// \par Complexity
/// Linear in the number of divisors.
///
template <typename T>
std::vector<T> generate_divisors(const std::vector<T> &prime_factors) {
  std::vector<T> divs = {1};
  size_t size = 1;
  T last_pf = 0;
  for (const auto pf : prime_factors) {
    if (pf != last_pf)
      size = divs.size(), last_pf = pf;
    for (size_t end = divs.size(), i = end - size; i != end; ++i)
      divs.push_back(pf * divs[i]);
  }
  std::sort(divs.begin(), divs.end()); // Sorting is optional
  return divs;
}

} // end namespace djp

#endif // Header guard
