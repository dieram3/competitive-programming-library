//          Copyright Diego Ramírez October 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef CPL_NUMBER_THEORY_CHINESE_REMAINDER_THEOREM_HPP
#define CPL_NUMBER_THEORY_CHINESE_REMAINDER_THEOREM_HPP

#include <cpl/number_theory/modular.hpp> // For mod_mul, mod_inverse
#include <utility>                       // For std::pair, std::make_pair
#include <vector>                        // For std::vector
#include <cassert>                       // For assert
#include <cstddef>                       // For std::size_t

namespace cpl {

/// \brief Solves the system of simultaneous congruences stated by the chinese
/// remainder theorem.
///
/// Uses the method of successive substitution to find a valid solution on the
/// given system. Let <tt>(\b a, \b m)</tt> be a system of \c N congruences, it
/// finds an integer \c x such that:
///
/// <blockquote><tt>x ≅ a<sub>i</sub> mod m<sub>i</sub></tt>, for all
/// <tt>i</tt></blockquote>
///
/// A solution \c x exists if and only if:
/// <blockquote><tt>a<sub>i</sub> ≅ a<sub>j</sub> (mod gcd(m<sub>i</sub>,
/// m<sub>j</sub></tt>)), for all \c i and \c j</blockquote>
///
/// \param a The sequence of remainders.
/// \param m The sequence of modulos.
///
/// \pre <tt>a.size() >= 1</tt>
/// \pre <tt>a.size() == m.size()</tt>
/// \pre <tt>a[i] >= 0 && a[i] < m[i]</tt>, for all <tt>i</tt>.
/// \pre The product of the <tt>m<sub>i</sub></tt> will be representable as a
/// value of type <tt>T</tt>.
/// \pre All values of <tt>m<sub>i</sub></tt> will be pairwise coprime.
///
/// \returns The smallest non-negative solution for the given system. All
/// other solutions \c x are congruent modulo the product of the
/// <tt>m<sub>i</sub></tt>.
///
/// \par Complexity
/// Exactly <tt>N - 1</tt> applications of <tt>mod_inverse</tt>,
/// where <tt>N = a.size()</tt>.
///
template <typename T>
T chinese_remainder_theorem(const std::vector<T> &a, const std::vector<T> &m) {
  auto solve2 = [](T a0, T m0, T a1, T m1) {
    T t = mod_inverse(m0 % m1, m1);
    assert(t != m1); // Otherwise no solution exists.
    t = mod_mul(mod_sub(a1, a0 % m1, m1), t, m1);
    return std::make_pair(a0 + m0 * t, m0 * m1);
  };
  std::pair<T, T> reduced{a[0], m[0]};
  for (size_t i = 1; i < a.size(); ++i) {
    reduced = solve2(reduced.first, reduced.second, a[i], m[i]);
    assert(reduced.first >= 0 && reduced.first < reduced.second);
  }
  return reduced.first;
}

} // end namespace cpl

#endif // Header guard
