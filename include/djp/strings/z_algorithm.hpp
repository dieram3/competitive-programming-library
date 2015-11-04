//          Copyright Jorge Aguirre, Diego Ramírez June 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DJP_STRINGS_Z_ALGORITHM_HPP
#define DJP_STRINGS_Z_ALGORITHM_HPP

#include <algorithm> // for std::min
#include <string>    // for std::string
#include <vector>    // for std::vector
#include <cstddef>   // for std::size_t

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
inline std::vector<size_t> z_algorithm(const std::string &str) {
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
