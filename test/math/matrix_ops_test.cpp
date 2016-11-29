//          Copyright Diego Ramirez 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cpl/math/matrix_ops.hpp>
#include <gtest/gtest.h>

#include <cpl/utility/matrix.hpp> // matrix
#include <cstddef>                // size_t
#include <ostream>                // ostream

using cpl::mat_mul;
using cpl::mat_identity;
using cpl::mat_pow;
using cpl::matrix2;
using std::size_t;

template <typename T, size_t M, size_t N>
static matrix2<T> as_matrix(const T (&mat)[M][N]) {
  matrix2<T> res(M, N);
  for (size_t i = 0; i < M; ++i)
    for (size_t j = 0; j < N; ++j)
      res[i][j] = mat[i][j];
  return res;
}

template <typename T>
static bool is_square(const matrix2<T>& mat) {
  return mat.num_rows() == mat.num_cols();
}

template <typename T>
static bool is_identity(const matrix2<T>& mat) {
  if (!is_square(mat))
    return false;
  for (size_t i = 0; i < mat.num_rows(); ++i) {
    for (size_t j = 0; j < mat.num_cols(); ++j) {
      const T expected = (i == j) ? 1 : 0;
      if (mat[i][j] != expected)
        return false;
    }
  }
  return true;
}

namespace cpl {

template <typename T>
static bool operator==(const matrix2<T>& a, const matrix2<T>& b) {
  if (a.num_rows() != b.num_rows() || a.num_cols() != b.num_cols())
    return false;
  for (size_t i = 0; i < a.num_rows(); ++i) {
    for (size_t j = 0; j < a.num_cols(); ++j) {
      if (a[i][j] != b[i][j])
        return false;
    }
  }
  return true;
}

template <typename T>
static std::ostream& operator<<(std::ostream& os, const matrix2<T>& mat) {
  os << "A " << mat.num_rows() << 'x' << mat.num_cols() << " matrix\n";
  for (size_t i = 0; i < mat.num_rows(); ++i)
    for (size_t j = 0; j < mat.num_cols(); ++j)
      os << mat[i][j] << (j + 1 == mat.num_cols() ? '\n' : ' ');
  return os;
}

} // end namespace cpl

TEST(MatrixOpsTest, MatMulTest) {
  {
    const int a[2][3] = {{1, 2, 3}, {3, 4, 5}};
    const int b[3][1] = {{4}, {1}, {5}};
    const matrix2<int> r = mat_mul(as_matrix(a), as_matrix(b));
    EXPECT_EQ(2, r.num_rows());
    EXPECT_EQ(1, r.num_cols());
    EXPECT_EQ(21, r[0][0]);
    EXPECT_EQ(41, r[1][0]);
  }

  {
    const int a[3][2] = {{2, 3},  // row 0
                         {3, 4},  // row 1
                         {5, 7}}; // row 2

    const int b[2][4] = {{1, 5, 2, 4},  // row 0
                         {2, 4, 1, 5}}; // row 1

    const int r[3][4] = {{8, 22, 7, 23},    // row 0
                         {11, 31, 10, 32},  // row 1
                         {19, 53, 17, 55}}; // row 2

    const auto res = mat_mul(as_matrix(a), as_matrix(b));
    EXPECT_EQ(as_matrix(r), res);
  }
}

TEST(MatrixOpsTest, MatIdentityTest) {
  const matrix2<int> a = mat_identity<int>(1);
  EXPECT_TRUE(a.num_rows() == 1 && is_identity(a));
  const matrix2<int> b = mat_identity<int>(4);
  EXPECT_TRUE(b.num_rows() == 4 && is_identity(b));
}

TEST(MatrixOpsTest, MatPowTest) {
  using int_t = unsigned long long;
  using matrix_t = matrix2<int_t>;

  const int_t b[3][3] = {{1, 2, 1},  // row 0
                         {3, 5, 2},  // row 1
                         {1, 3, 7}}; // row 2

  const matrix_t base = as_matrix(b);
  {
    const matrix_t res = mat_pow(base, 0);
    EXPECT_TRUE(res.num_rows() == 3 && is_identity(res));
  }

  EXPECT_EQ(base, mat_pow(base, 1));

  EXPECT_EQ(mat_mul(base, base), mat_pow(base, 2));

  {
    const int_t r[3][3] = {{47105, 94941, 103041},
                           {111539, 224638, 243129},
                           {164786, 333821, 368975}};
    EXPECT_EQ(as_matrix(r), mat_pow(base, 6));
  }
  {
    const int_t r[3][3] = {{346200227, 699564099, 766313862},
                           {817939586, 1652765800, 1810315599},
                           {1229126987, 2484066836, 2722580399}};
    EXPECT_EQ(as_matrix(r), mat_pow(base, 10));
  }
}
