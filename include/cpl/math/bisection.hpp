//          Copyright Diego Ramirez 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef CPL_MATH_BISECTION_HPP
#define CPL_MATH_BISECTION_HPP

#include <cassert>     // assert
#include <cmath>       // fabs, signbit
#include <limits>      // numeric_limits
#include <stdexcept>   // runtime_error
#include <type_traits> // is_floating_point

namespace cpl {

/// \brief Finds a root of the given function.
///
/// Uses the bisection method to find a root of \p f in the interval
/// <tt>[a, b]</tt>. The function \p f must be continuous in this interval,
/// otherwise the method is not guaranteed to converge.
///
/// \tparam T A floating-point type.
/// \tparam F The function type.
///
/// \param f The function to be examined.
/// \param a The lower bound of the interval to be examined.
/// \param b The upper bound of the interval to be examined.
/// \param tol The maximum allowed absolute difference between the found root
/// the actual corresponding root.
/// \param max_iter The maximum allowed number of iterations.
///
/// \pre <tt>a <= b</tt>.
/// \pre <tt>tol > 0</tt>.
/// \pre Either <tt>f(a)</tt> is a valid root, or <tt>f(b)</tt> is a valid root,
/// or <tt>f(a)</tt> and <tt>f(b)</tt> have opposite signs.
///
/// \returns A valid root (according to the given parameters).
///
/// \throws std::runtime_error if the maximum number of iterations was exceeded.
///
/// \par Complexity
/// The time complexity is logarithmic in <tt>(b - a)</tt>. The convergence
/// of the method is linear.
///
template <typename T, typename F>
T bisect(const F f, T a, T b, const T tol, const int max_iter) {
  static_assert(std::is_floating_point<T>::value, "Must be floating-point");

  auto is_zero = [](const T x) {
    return std::fabs(x) < std::numeric_limits<T>::min();
  };
  T fa = f(a);
  T fb = f(b);
  if (is_zero(fa))
    return a;
  if (is_zero(fb))
    return b;
  assert(std::signbit(fa) != std::signbit(fb));
  assert(a <= b);

  for (int iter = 0; iter < max_iter; ++iter) {
    const T c = (a + b) / 2;
    const T fc = f(c);
    if (is_zero(fc) || (b - a) / 2 <= tol)
      return c;
    if (std::signbit(fc) == std::signbit(fa))
      a = c, fa = fc;
    else
      b = c, fb = fc;
  }
  throw std::runtime_error("Max number of iterations exceeded");
}

} // end namespace cpl

#endif // Header guard
