#include <djp/utility/matrix.hpp>
#include <iostream>

using djp::matrix;

int main() {
  matrix<int> mat({5, 4});

  for (size_t i = 0; i != mat.rows(); ++i)
    for (size_t j = 0; j != mat.cols(); ++j) mat[{i,j}] = i * mat.cols() + j;

  std::cout << mat << '\n';
}
