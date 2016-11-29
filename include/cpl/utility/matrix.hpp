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
  using row_iterator = typename std::vector<T>::iterator;
  using const_row_iterator = typename std::vector<T>::const_iterator;

public:
  matrix() = default;

  matrix(size_t r, size_t c, const T& value = T())
      : data(r * c, value), nrows{r}, ncols{c} {}

  void assign(size_t r, size_t c, const T& value) {
    data.assign(r * c, value);
    nrows = r;
    ncols = c;
  }

  void resize(size_t r, size_t c) {
    data.resize(r * c);
    nrows = r;
    ncols = c;
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
  std::vector<T> data;
  size_t nrows{};
  size_t ncols{};
};

} // end namespace cpl

#endif // Header guard
