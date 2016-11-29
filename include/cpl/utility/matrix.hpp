//          Copyright Diego Ramirez 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
/// \file
/// \brief Defines the class \c matrix.

#ifndef CPL_UTILITY_MATRIX_HPP
#define CPL_UTILITY_MATRIX_HPP

#include <cstddef> // size_t
#include <utility> // pair
#include <vector>  // vector

namespace cpl {

/// \brief Two-dimensional array.
///
/// This is a convenience class used for working with two-dimensional arrays on
/// a contiguous chunk of memory.
///
template <typename T>
class matrix {
public:
  using bounds_type = std::pair<size_t, size_t>;
  using row_iterator = typename std::vector<T>::iterator;
  using const_row_iterator = typename std::vector<T>::const_iterator;

public:
  matrix() = default;

  matrix(bounds_type bounds, const T& value = T())
      : nrows{bounds.first}, ncols{bounds.second}, data(nrows * ncols, value) {}

  void assign(bounds_type bounds, const T& value) {
    nrows = bounds.first;
    ncols = bounds.second;
    data.assign(nrows * ncols, value);
  }

  void resize(bounds_type bounds) {
    nrows = bounds.first;
    ncols = bounds.second;
    data.resize(nrows * ncols);
  }

  size_t num_rows() const {
    return nrows;
  }
  size_t num_cols() const {
    return ncols;
  }

  row_iterator operator[](size_t row_idx) {
    return data.begin() + row_idx * ncols;
  }

  const_row_iterator operator[](size_t row_idx) const {
    return data.begin() + row_idx * ncols;
  }

private:
  size_t nrows{};
  size_t ncols{};
  std::vector<T> data;
};

} // end namespace cpl

#endif // Header guard
