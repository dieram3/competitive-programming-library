#ifndef DJP_STRINGS_Z_FUNCTION_HPP
#define DJP_STRINGS_Z_FUNCTION_HPP

#include <algorithm>
#include <vector>
#include <string>
#include <cstddef>

namespace djp {

/// \brief Computes the Z-function of str.
/// Each element of the Z-function is described like this: the element z[i] is
/// the longest common prefix between str and str.substr(i).
/// Complexity: O(n) where n = str.length()
/// Returns the Z-function array of str.
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
