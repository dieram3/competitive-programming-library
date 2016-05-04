//          Copyright Jorge Aguirre 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef CPL_STRINGS_Z_ALGORITHM_HPP
#define CPL_STRINGS_Z_ALGORITHM_HPP

#include <algorithm> // min
#include <cstddef>   // size_t
#include <string>    // string
#include <vector>    // vector

namespace cpl {

/// \brief Computes the Z-array of a given string.
///
/// The Z-array of a string \c str is a sequence \c z of \c str.length()
/// elements. In this sequence, the element \c z[i] is defined as the longest
/// common prefix between \p str and <tt>str.substr(i)</tt>.
///
/// \param str The input string.
///
/// \returns The Z-array of <tt>str</tt>.
///
/// \par Complexity
/// Linear in <tt>str.size()</tt>.
///
inline std::vector<size_t> z_algorithm(const std::string& str) {
  if (str.empty())
    return {};

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

} // end namespace cpl

#endif // Header guard
