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
  using index_type = std::pair<size_t, size_t>;
  using reference = typename std::vector<T>::reference;
  using const_reference = typename std::vector<T>::const_reference;

  explicit matrix(index_type bounds = {0, 0}, const T& value = T())
      : bounds_(bounds), data_(rows() * cols(), value) {}

  void assign(index_type bounds, const T& value = T()) {
    bounds_ = bounds;
    data_.assign(rows() * cols(), value);
  }

  size_t pos(index_type idx) const {
    return idx.first * cols() + idx.second;
  }
  size_t rows() const {
    return bounds_.first;
  }
  size_t cols() const {
    return bounds_.second;
  }
  reference operator[](index_type idx) {
    return data_[pos(idx)];
  }
  const_reference operator[](index_type idx) const {
    return data_[pos(idx)];
  }

private:
  index_type bounds_;
  std::vector<T> data_;
};

template <typename T>
class matrix2 {
public:
  using row_iterator = typename std::vector<T>::iterator;
  using const_row_iterator = typename std::vector<T>::const_iterator;

public:
  matrix2() = default;

  matrix2(size_t r, size_t c, const T& value = T())
      : data(r * c, value), nrows{r}, ncols{c} {}

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

  row_iterator operator[](const size_t row_idx) {
    return data.begin() + row_idx * ncols;
  }

  const_row_iterator operator[](const size_t row_idx) const {
    return data.begin() + row_idx * ncols;
  }

private:
  std::vector<T> data;
  size_t nrows{};
  size_t ncols{};
};

} // end namespace cpl

#endif // Header guard
