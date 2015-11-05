//          Copyright Diego Ramírez November 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DJP_COMBINATORICS_CATALAN_NUMBERS
#define DJP_COMBINATORICS_CATALAN_NUMBERS

#include <vector>                       // For std::vector
#include <cassert>                      // For assert
#include <cstddef>                      // For std::size_t
#include <djp/number_theory/euclid.hpp> // For djp::gcd

namespace djp {

/// \brief Generates a sequence of Catalan numbers.
///
/// Uses a DP-based approach to create a list of the first \p total Catalan
/// numbers. GCD is used to avoid integer overflow.
///
/// \param total The number of Catalan numbers to be computed.
///
/// \pre The generated Catalan numbers must fit in <tt>T</tt>.
///
/// \returns A vector <tt>c</tt>, where <tt>c[n]</tt> contains the value of the
/// <em>nth</em> Catalan number (starting from <tt>n=0</tt>).
///
/// \par Complexity
/// Exactly <tt>(total - 1)</tt> GCD computations. Note that GCD is applied to
/// small numbers only and not to catalan numbers.
///
/// \note The first 20 catalan numbers fit in <tt>std::uint32_t</tt>. The first
/// 37 catalan numbers fit in <tt>std::uint64_t</tt>.
///
template <typename T>
std::vector<T> generate_catalan_numbers(size_t total) {
  if (total == 0)
    return {};
  std::vector<T> cat(total);
  cat[0] = 1;
  for (size_t n = 0; n + 1 < total; ++n) {
    // c[n + 1] = (4 * n + 2) * c[n] / (n + 2); // No overflow control version
    const size_t g1 = gcd(4 * n + 2, n + 2);
    const size_t a = (4 * n + 2) / g1;
    const size_t b = (n + 2) / g1;
    assert(cat[n] % b == 0 && "Integer overflow detected");
    cat[n + 1] = T(a) * (cat[n] / T(b));
  }
  return cat;
}

} // end namespace djp

#endif // Header guard
