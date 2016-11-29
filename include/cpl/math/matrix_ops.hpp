//          Copyright Diego Ramirez 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef CPL_MATH_MATRIX_OPS_HPP
#define CPL_MATH_MATRIX_OPS_HPP

#include <cpl/utility/matrix.hpp> // matrix
#include <cassert>                // assert
#include <cstddef>                // size_t

namespace cpl {

/// \brief Multiplies a pair of matrices.
///
/// \par a The left-hand side matrix.
/// \par b The right-hand side matrix.
///
/// \returns The result of multiply <tt>a</tt> by <tt>b</tt>.
///
/// \par Complexity
/// Exactly <tt>A.rows()*B.cols()*A.cols()</tt> scalar multiplications.
///
template <typename T>
matrix<T> mat_mul(const matrix<T>& a, const matrix<T>& b) {
  assert(a.num_cols() == b.num_rows() && "Matrices cannot be multiplied");
  matrix<T> r({a.num_rows(), b.num_cols()});
  for (size_t i = 0; i < r.num_rows(); ++i) {
    for (size_t j = 0; j < r.num_cols(); ++j) {
      T sum = 0;
      for (size_t k = 0; k < a.num_cols(); ++k)
        sum += a[i][k] * b[k][j];
      r[i][j] = sum;
    }
  }
  return r;
}

/// \brief Generates the identity matrix.
///
/// \param dim The dimension of the identity matrix to be generated.
///
/// \returns The generated identity matrix.
///
template <typename T>
matrix<T> mat_identity(const size_t dim) {
  matrix<T> res({dim, dim});
  for (size_t k = 0; k < dim; ++k)
    res[k][k] = 1;
  return res;
}

/// \brief Raises a matrix to the given power.
///
/// \param base The base matrix.
/// \param exp The (integer) exponent.
///
/// \pre <tt>exp >= 0</tt>
///
/// \returns The result of <tt>base</tt> raised to the power <tt>exp</tt>.
///
/// \par Complexity
/// <tt>O(log(exp))</tt> matrix multiplications.
///
template <typename T, typename Integer>
matrix<T> mat_pow(matrix<T> base, Integer exp) {
  assert(base.num_rows() == base.num_cols() && "Base matrix must be square");
  assert(exp >= 0 && "Exponent cannot be negative");
  matrix<T> result = mat_identity<T>(base.num_rows());
  while (exp > 0) {
    if (exp % 2 == 1)
      result = mat_mul(result, base);
    base = mat_mul(base, base);
    exp /= 2;
  }
  return result;
}

} // end namespace cpl

#endif // Header guard
