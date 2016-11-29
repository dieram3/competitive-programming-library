//          Copyright Diego Ramirez 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cpl/utility/matrix.hpp>
#include <gtest/gtest.h>

#include <cstddef>     // size_t
#include <type_traits> // is const, remove_reference

using cpl::matrix;
using std::size_t;

namespace {
class MatrixTest : public ::testing::Test {};
} // end namespace

TEST_F(MatrixTest, DefaultCtorTest) {
  matrix<int> mat;
  EXPECT_EQ(0, mat.num_rows());
  EXPECT_EQ(0, mat.num_cols());
}

TEST_F(MatrixTest, DimCtorTest) {
  matrix<int> mat(5, 7);
  EXPECT_EQ(5, mat.num_rows());
  EXPECT_EQ(7, mat.num_cols());

  // elements are value-initialized
  for (size_t i = 0; i < mat.num_rows(); ++i)
    for (size_t j = 0; j < mat.num_cols(); ++j)
      EXPECT_EQ(0, mat[i][j]);
}

TEST_F(MatrixTest, DimAndFillCtorTest) {
  matrix<bool> mat(3, 4, true);
  EXPECT_EQ(3, mat.num_rows());
  EXPECT_EQ(4, mat.num_cols());

  // elements are copy-initialized
  for (size_t i = 0; i < mat.num_rows(); ++i)
    for (size_t j = 0; j < mat.num_cols(); ++j)
      EXPECT_TRUE(mat[i][j]);
}

TEST_F(MatrixTest, IsConstAware) {
  matrix<int> mat(1, 1);
  const auto& cmat = mat;

  using ref = decltype(mat[0][0]);
  using cref = decltype(cmat[0][0]);
  using type = typename std::remove_reference<ref>::type;
  using ctype = typename std::remove_reference<cref>::type;

  static_assert(!std::is_const<type>::value,
                "Non-const access to matrix is const");

  static_assert(std::is_const<ctype>::value,
                "Const access to matrix is non-const");

  // Note: The static asserts will fail if the underlying type of the matrix
  // is bool, since it returns a proxy instead of a reference.

  EXPECT_EQ(0, cmat[0][0]);
  mat[0][0] = 4;
  EXPECT_EQ(4, cmat[0][0]);
}

TEST_F(MatrixTest, ResizeTest) {
  matrix<int> mat(2, 3);
  ASSERT_EQ(2, mat.num_rows());
  ASSERT_EQ(3, mat.num_cols());

  mat.resize(5, 2);
  EXPECT_EQ(5, mat.num_rows());
  EXPECT_EQ(2, mat.num_cols());

  for (size_t i = 0; i != mat.num_rows(); ++i) {
    for (size_t j = 0; j != mat.num_cols(); ++j)
      EXPECT_EQ(0, mat[i][j]) << " at (" << i << ", " << j << ")";
  }
}

TEST_F(MatrixTest, AssignWithFillValueTest) {
  matrix<int> mat(1, 4, /*value=*/2);
  ASSERT_EQ(1, mat.num_rows());
  ASSERT_EQ(4, mat.num_cols());
  ASSERT_EQ(2, mat[0][0]);

  mat.assign(5, 3, /*value=*/7);
  EXPECT_EQ(5, mat.num_rows());
  EXPECT_EQ(3, mat.num_cols());
  for (size_t i = 0; i != mat.num_rows(); ++i) {
    for (size_t j = 0; j != mat.num_cols(); ++j)
      EXPECT_EQ(7, mat[i][j]) << " at (" << i << ", " << j << ")";
  }
}
