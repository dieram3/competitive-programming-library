#include <djp/utility/matrix.hpp>
#include <gtest/gtest.h>
#include <type_traits> // is const, remove_reference
#include <cstddef>     // size_t

TEST(matrix, ConstrucstWell) {

  // elements are value-initialized
  {
    djp::matrix<int> mat({5, 7});
    EXPECT_EQ(5, mat.rows());
    EXPECT_EQ(7, mat.cols());

    for (std::size_t i = 0; i < mat.rows(); ++i)
      for (std::size_t j = 0; j < mat.cols(); ++j)
        EXPECT_EQ(0, (mat[{i, j}]));
  }

  // elements are copy-initialized
  {
    djp::matrix<bool> mat({3, 4}, true);
    EXPECT_EQ(3, mat.rows());
    EXPECT_EQ(4, mat.cols());

    for (std::size_t i = 0; i < mat.rows(); ++i)
      for (std::size_t j = 0; j < mat.cols(); ++j)
        EXPECT_TRUE((mat[{i, j}]));
  }
}

TEST(matrix, IndexesWell) {
  djp::matrix<int> mat({8, 5});
  EXPECT_EQ(0, mat.pos({0, 0}));
  EXPECT_EQ(mat.cols(), mat.pos({1, 0}));
  EXPECT_EQ(3, mat.pos({0, 3}));
  EXPECT_EQ(4 * mat.cols(), mat.pos({4, 0}));
  EXPECT_EQ(6 * mat.cols() + 4, mat.pos({6, 4}));
}

TEST(matrix, IsConstAware) {
  djp::matrix<int> mat({1, 1});
  const auto &cmat = mat;

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
