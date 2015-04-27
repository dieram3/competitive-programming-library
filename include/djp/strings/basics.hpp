#ifndef DJP_STRING_BASICS_HPP
#define DJP_STRING_BASICS_HPP

#include <string>
#include <algorithm>
#include <vector>

namespace djp {

std::vector<std::string> split(const std::string& str, const std::string& sep) {
  std::vector<std::string> tokens;
  auto pos = str.find_first_not_of(sep);
  while (pos != std::string::npos) {
    auto end_pos = str.find_first_of(sep, pos + 1);
    end_pos = std::min(end_pos, str.size());
    tokens.emplace_back(str, pos, end_pos - pos);
    if (end_pos == str.size()) break;
    pos = str.find_first_not_of(sep, end_pos + 1);
  }
  return tokens;
}

}  // namespace djp

#endif  // Header guard
