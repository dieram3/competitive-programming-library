//          Copyright Diego Ramírez June 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
/// \file
/// \brief Defines the class \c matrix.

#ifndef DJP_UTILITY_MATRIX_HPP
#define DJP_UTILITY_MATRIX_HPP

#include <utility> // for std::pair
#include <vector>  // for std::vector
#include <cstddef> // for std::size_t

namespace djp {

/// \brief Two-dimensional array.
///
/// This is a convenience class used for working with two-dimensional arrays on
/// a contiguous chunk of memory.
///
template <typename T>
class matrix {
public:
  using index_type = std::pair<size_t, size_t>;
  using reference = typename std::vector<T>::reference;
  using const_reference = typename std::vector<T>::const_reference;

  matrix(index_type bounds) : bounds_(bounds), data_(rows() * cols()) {}

  matrix(index_type bounds, const T &value)
      : bounds_(bounds), data_(rows() * cols(), value) {}

  size_t pos(index_type idx) const { return idx.first * cols() + idx.second; }
  size_t rows() const { return bounds_.first; }
  size_t cols() const { return bounds_.second; }
  reference operator[](index_type idx) { return data_[pos(idx)]; }
  const_reference operator[](index_type idx) const { return data_[pos(idx)]; }

private:
  index_type bounds_;
  std::vector<T> data_;
};

} // end namespace djp

#endif // Header guard
