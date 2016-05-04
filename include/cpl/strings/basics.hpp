//          Copyright Diego Ramirez 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
/// \file
/// \brief Defines functions for working with strings.

#ifndef CPL_STRING_BASICS_HPP
#define CPL_STRING_BASICS_HPP

#include <string> // string
#include <vector> // vector

namespace cpl {

/// \brief Splits the given string in tokens delimited by the given char set.
///
/// This function is similar to \c std::strtok. It finds all tokens contained in
/// \p str and returns a copy of them in their respective order. A token is
/// defined as a sequence of consecutive characters that are not contained in
/// \p sep.
///
/// \param str The string to be splitted.
/// \param sep The set of delimiter characters.
///
/// \returns A vector of strings containing the generated tokens.
///
/// \par Complexity
/// <tt>O(N * M)</tt> where <tt>N = str.size()</tt> and <tt>M = sep.size()</tt>.
///
/// \note Be aware that this function could generate allocation overhead.
///
inline std::vector<std::string> split(const std::string& str,
                                      const std::string& sep) {
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

} // end namespace cpl

#endif // Header guard
