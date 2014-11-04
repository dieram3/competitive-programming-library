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

  friend std::istream& operator>>(std::istream& is, matrix& mat) {
    for (auto& elem : mat.data_) is >> elem;
    return is;
  }

  friend std::ostream& operator<<(std::ostream& os, const matrix& mat) {
    for (size_type i = 0; i != mat.rows_; ++i) {
      for (size_type j = 0; j != mat.cols_; ++j) os << mat[i][j] << ' ';
      os << '\n';
    }
    return os;
  }

 private:
  size_type rows_;
  size_type cols_;
  std::vector<T> data_;
};

}  // namespace djp

#endif  // Header guard
