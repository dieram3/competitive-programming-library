#ifndef DJP_UTILITY_MATRIX_HPP
#define DJP_UTILITY_MATRIX_HPP

#include <vector>
#include <ostream>

namespace djp {

template <class T>
class matrix {
 public:
  using size_type = std::size_t;

  matrix(size_type r, size_type c) : rows_{r}, cols_{c}, data_(r * c) {}

  size_type rows() const noexcept { return rows_; }
  size_type cols() const noexcept { return cols_; }

  T* operator[](size_type i) noexcept { return &data_[i * cols_]; }
  const T* operator[](size_type i) const noexcept { return &data_[i * cols_]; }

 private:
  size_type rows_;
  size_type cols_;
  std::vector<T> data_;
};

template <class T>
void print_matrix(const matrix<T>& mat, std::ostream& output) {
  for (size_t i = 0; i != mat.rows(); ++i) {
    for (size_t j = 0; j != mat.cols(); ++j) output << mat[i][j] << ' ';
    output << '\n';
  }
}

}  // namespace djp

#endif  // Header guard
