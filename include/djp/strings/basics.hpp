#ifndef DJP_STRING_BASICS_HPP
#define DJP_STRING_BASICS_HPP

#include <string>
#include <vector>

namespace djp {

/// \brief Splits the given string in tokens delimited by the given char set.
/// This function is similar to strtok. It finds all tokens contained in str
/// and returns a copy of them in their respective order.
/// A token is defined as a sequence of consecutive characters that are not
/// contained in \p sep.
/// Complexity: O(N*M) where N == str.size() && M = sep.size()
/// \note Be aware that this function could generate allocation overhead.
std::vector<std::string> split(const std::string &str, const std::string &sep) {
  std::vector<std::string> tokens;
  auto pos = str.find_first_not_of(sep);
  while (pos != std::string::npos) {
    auto end_pos = str.find_first_of(sep, pos + 1);
    tokens.emplace_back(str, pos, end_pos - pos);
    if (end_pos == std::string::npos)
      break;
    pos = str.find_first_not_of(sep, end_pos + 1);
  }
  return tokens;
}

} // namespace djp

#endif // Header guard
