//          Copyright Diego Ramírez June 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DJP_UTILITY_BASICS_HPP
#define DJP_UTILITY_BASICS_HPP

#include <cstddef> // for std::size_t

namespace djp {

template <class Function> inline void repeat(std::size_t n, Function f) {
  while (n--)
    f();
}

} // namespace djp

#endif // Header guard
