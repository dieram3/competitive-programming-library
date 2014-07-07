#ifndef DJP_UTILITY_BASICS_HPP
#define DJP_UTILITY_BASICS_HPP

#include <istream>

namespace djp {

/// Reads a value of type T from input as formatted input.
template <class T>
T input_value(std::istream& input) {
  T value;
  input >> value;
  return value;
}

/// Calls \p n times the function \p f.
template <class Function>
void repeat(std::size_t n, Function f) {
  while (n--) f();
}

}

#endif  // Header guard
