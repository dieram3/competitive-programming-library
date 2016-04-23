//          Copyright Diego Ramirez 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cpl/utility/matrix.hpp>
#include <gtest/gtest.h>

#include <cstddef>     // size_t
#include <type_traits> // is const, remove_reference

using cpl::matrix;

TEST(MatrixTest, ConstrucstWell) {

  // elements are value-initialized
  {
    matrix<int> mat({5, 7});
    EXPECT_EQ(5u, mat.rows());
    EXPECT_EQ(7u, mat.cols());

    for (std::size_t i = 0; i < mat.rows(); ++i)
      for (std::size_t j = 0; j < mat.cols(); ++j)
        EXPECT_EQ(0, (mat[{i, j}]));
  }

  // elements are copy-initialized
  {
    matrix<bool> mat({3, 4}, true);
    EXPECT_EQ(3u, mat.rows());
    EXPECT_EQ(4u, mat.cols());

    for (std::size_t i = 0; i < mat.rows(); ++i)
      for (std::size_t j = 0; j < mat.cols(); ++j)
        EXPECT_TRUE((mat[{i, j}]));
  }
}

TEST(MatrixTest, IndexesWell) {
  matrix<int> mat({8, 5});
  EXPECT_EQ(0u, mat.pos({0, 0}));
  EXPECT_EQ(mat.cols(), mat.pos({1, 0}));
  EXPECT_EQ(3u, mat.pos({0, 3}));
  EXPECT_EQ(4u * mat.cols(), mat.pos({4, 0}));
  EXPECT_EQ(6u * mat.cols() + 4, mat.pos({6, 4}));
}

TEST(MatrixTest, IsConstAware) {
  matrix<int> mat({1, 1});
  const auto& cmat = mat;

  using ref = decltype(mat[{0, 0}]);
  using cref = decltype(cmat[{0, 0}]);
  using type = typename std::remove_reference<ref>::type;
  using ctype = typename std::remove_reference<cref>::type;

  static_assert(!std::is_const<type>::value,
                "Non-const access to matrix is const");

  static_assert(std::is_const<ctype>::value,
                "Const access to matrix is non-const");

  // Note: The static asserts will fail if the underlying type of the matrix
  // is bool, since it returns a proxy instead of a reference.

  EXPECT_EQ(0, (cmat[{0, 0}]));
  mat[{0, 0}] = 4;
  EXPECT_EQ(4, (cmat[{0, 0}]));
}

TEST(MatrixTest, AssignDefaultTest) {
  matrix<int> mat({2, 2}, 3); // Initial garbage.
  mat.assign({3, 4});
  ASSERT_EQ(3, mat.rows());
  ASSERT_EQ(4, mat.cols());
  for (size_t i = 0; i != mat.rows(); ++i)
    for (size_t j = 0; j != mat.cols(); ++j)
      EXPECT_EQ(0, (mat[{i, j}])) << " at (" << i << ", " << j << ")";
}

TEST(MatrixTest, AssignWithFillValueTest) {
  matrix<int> mat({1, 4}, 2); // Initial garbage.
  mat.assign({5, 4}, 7);
  ASSERT_EQ(5, mat.rows());
  ASSERT_EQ(4, mat.cols());
  for (size_t i = 0; i != mat.rows(); ++i)
    for (size_t j = 0; j != mat.cols(); ++j)
      EXPECT_EQ(7, (mat[{i, j}])) << " at (" << i << ", " << j << ")";
}
