#ifndef DJP_UTILITY_MATRIX_HPP
#define DJP_UTILITY_MATRIX_HPP

#include <vector>
#include <ostream>

namespace djp {

template <class T>
class matrix {
 public:
  using index_type = std::pair<size_t, size_t>;
  using reference = typename std::vector<T>::reference;
  using const_reference = typename std::vector<T>::const_reference;

  matrix(index_type bounds) : bounds_(bounds), data_(rows() * cols()) {}

  size_t pos(index_type idx) const { return idx.first * cols() + idx.second; }
  size_t rows() const { return bounds_.first; }
  size_t cols() const { return bounds_.second; }
  reference operator[](index_type idx) { return data_[pos(idx)]; }
  const_reference operator[](index_type idx) const { return data_[pos(idx)]; }

 private:
  index_type bounds_;
  std::vector<T> data_;
};

template <class T>
std::ostream& operator<<(std::ostream& output, const matrix<T>& mat) {
  for (size_t i = 0; i != mat.rows(); ++i) {
    for (size_t j = 0; j != mat.cols(); ++j) output << mat[{i, j}] << ' ';
    output << '\n';
  }
  return output;
}

}  // namespace djp

#endif  // Header guard
