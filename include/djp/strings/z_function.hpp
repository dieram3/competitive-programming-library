#ifndef DJP_STRING_Z_FUNCTION_HPP
#define DJP_STRING_Z_FUNCTION_HPP

#include <vector>
#include <string>

/// \brief Computes the Z-function of str.
/// Each element of the Z-function is described like this: the element z[i] is
/// the longest common prefix between str and str.substr(i).
/// Complexity: O(n) where n = str.length()
/// Returns the Z-function array of str.
std::vector<size_t> z_function(const std::string &str) {
  size_t n = str.length();
  std::vector<size_t> z(n);
  for (size_t i = 1, l = 0, r = 0; i < n; ++i) {
    if (i <= r) z[i] = std::min(r - i + 1, z[i - l]);
    while (i + z[i] < n && str[z[i]] == str[i + z[i]]) ++z[i];
    if (i + z[i] - 1 > r) l = i, r = i + z[i] - 1;
  }
  return std::move(z);
}

#endif  // HEADER GUARD
