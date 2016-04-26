//          Copyright Diego Ramirez 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
/// \file
/// \brief Defines basic utility functions.

#ifndef CPL_UTILITY_BASICS_HPP
#define CPL_UTILITY_BASICS_HPP

namespace cpl {

/// \brief Invokes a given function a fixed number of times.
///
/// \param n The number of times to invoke \p f.
/// \param f The function to be invoked on each repetition.
///
template <typename Size, typename UnaryFunction>
inline void repeat(const Size n, UnaryFunction f) {
  for (Size i = 0; i < n; ++i)
    f();
}

} // end namespace cpl

#endif // Header guard
