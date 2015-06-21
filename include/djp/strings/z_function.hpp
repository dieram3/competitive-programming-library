//          Copyright Jorge Aguirre, Diego Ramírez June 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
/// \file
/// \brief Defines the function \c z_function.

#ifndef DJP_STRINGS_Z_FUNCTION_HPP
#define DJP_STRINGS_Z_FUNCTION_HPP

#include <algorithm>
#include <vector>
#include <string>
#include <cstddef>

namespace djp {

/// \brief Computes the Z-function array of a given string.
///
/// Each element of the Z-function array is described like this: the element
/// <tt>z[i]</tt> is the longest common prefix between \p str and
/// <tt>str.substr(i)</tt>.
///
/// \returns The Z-function array of \p str.
///
/// \par Complexity
/// <tt>O(n)</tt> where <tt>n = str.length()</tt>
///
inline std::vector<size_t> z_function(const std::string &str) {
  size_t n = str.length();
  std::vector<size_t> z(n);

  z[0] = n; // since str and str.substr(0) are equal
  for (size_t i = 1, l = 0, r = 0; i < n; ++i) {
    if (i < r)
      z[i] = std::min(r - i, z[i - l]);
    while (i + z[i] < n && str[z[i]] == str[i + z[i]])
      ++z[i];
    if (i + z[i] > r)
      l = i, r = i + z[i];
  }
  return z;
}

} // end namespace djp

#endif // Header guard
