//          Copyright Diego Ramírez September 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DJP_NUMBER_THEORY_POLLARD_RHO_FACTORIZATION_HPP
#define DJP_NUMBER_THEORY_POLLARD_RHO_FACTORIZATION_HPP

#include <djp/number_theory/euclid.hpp>  // For gcd
#include <djp/number_theory/modular.hpp> // For mod_mul

namespace djp {

/// \brief Finds a non-trivial factor of a composite number.
///
/// Uses the Pollard's rho algorithm to find a non-trivial factor of <tt>n</tt>.
/// It is a special-purpose integer factorization algorithm which is
/// particularly effective for composite numbers having small prime factors.
///
/// \param n The integer to be factored.
///
/// \pre \p n must be a composite number, that is, a positive integer greater
/// than 1 that is not a prime number.
/// \pre <tt>n < 2<sup>63</sup></tt>
///
/// \returns A non-trivial factor of <tt>n</tt>.
///
/// \par Complexity
/// The time complexity is <tt>O(<sup>4</sup>&radic;N)</tt> on average.
///
template <typename T>
T pollard_rho_factor(const T n) {
  auto find_factor = [&](const T c) {
    auto abs_diff = [](T x, T y) { return x < y ? y - x : x - y; };
    T power = 1, lam = 1;
    T x = 2, x_fixed = 2;
    T factor = 1;
    while (factor == 1) {
      if (power == lam)
        x_fixed = x, power *= 2, lam = 0;
      x = (mod_mul(x, x, n) + c) % n;
      ++lam;
      factor = gcd(abs_diff(x, x_fixed), n);
    }
    return factor;
  };
  T factor = n;
  for (T c = 1; factor == n; ++c)
    factor = find_factor(c);
  return factor;
}

} // end namespace djp

#endif // Header guard
