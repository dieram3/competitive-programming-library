//          Copyright Diego Ramírez August 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DJP_MATH_SIMPLEX_HPP
#define DJP_MATH_SIMPLEX_HPP

// This implementation is mainly taken from:
// https://web.stanford.edu/~liszt90/acm/notebook.html#file17

#include <djp/utility/matrix.hpp>
#include <algorithm>   // For std::swap
#include <limits>      // For std::numeric_limits
#include <stdexcept>   // For std::domain_error
#include <type_traits> // For std::is_floating_point
#include <vector>      // For std::vector
#include <cassert>     // For assert
#include <cmath>       // For std::fabs
#include <cstddef>     // For std::size_t
#include <cstdint>     // For SIZE_MAX

namespace djp {

/// \brief Simplex solver.
///
/// \tparam T A floating-point type.
///
template <typename T>
class simplex_solver {
  static_assert(std::is_floating_point<T>::value, "Must be floating-point");
  using vec_t = std::vector<T>;
  using matrix_t = matrix<T>;

public:
  /// \brief Changes the epsilon value used to compare floats.
  ///
  /// \param val The new epsilon value.
  ///
  /// \pre <tt>eps >= std::numeric_limits<T>::epsilon()</tt>
  ///
  void set_eps(T val) { eps = val; }

  /// \brief Changes maximum number of iterations allowed before throwing.
  ///
  /// \param val The new upper bound value.
  ///
  void set_max_iters(size_t val) { max_iters = val; }

  /// \brief Finds the optimal value of the given linear program.
  ///
  /// The program must be given as follows:
  ///
  /// \li Maximize <tt>c^T*x</tt>
  /// \li Subject to <tt>A*x <= b</tt>
  /// \li and <tt>x >= 0</tt>
  ///
  /// \param A Matrix of \c m rows and \c n columns.
  /// \param b An <tt>m</tt>-dimensional vector.
  /// \param c An <tt>n</tt>-dimensional vector.
  /// \param[out] x Vector where the optimal solution will be stored.
  ///
  ///
  /// \returns Value of the optimal solution. If the solution is unbounded
  /// returns <tt>std::numeric_limits<T>::infinity()</tt>. If no feasible
  /// solution exists returns <tt>std::numeric_limits<T>::quiet_NaN()</tt>.
  ///
  /// \throws std::domain_error If the simplex algorithm was stalled in an
  /// infinite loop.
  ///
  T maximize(const matrix_t &A, const vec_t &b, const vec_t &c, vec_t &x) {
    assert(A.rows() == b.size());
    assert(A.cols() == c.size());

    m = A.rows(), n = A.cols();
    B.resize(m), N.resize(n + 1);
    D.assign({m + 2, n + 2});

    for (size_t i = 0; i < m; ++i)
      for (size_t j = 0; j < n; ++j)
        D[{i, j}] = A[{i, j}];
    for (size_t i = 0; i < m; ++i)
      B[i] = n + i, D[{i, n}] = -1, D[{i, n + 1}] = b[i];
    for (size_t j = 0; j < n; ++j)
      N[j] = j, D[{m, j}] = -c[j];
    N[n] = SIZE_MAX;
    D[{m + 1, n}] = 1;
    return solve_system(x);
  }

private:
  T solve_system(vec_t &x) {
    size_t r = 0;
    for (size_t i = 1; i < m; ++i)
      if (D[{i, n + 1}] < D[{r, n + 1}])
        r = i;
    if (is_neg(D[{r, n + 1}])) {
      pivot(r, n);
      if (!simplex(1) || is_neg(D[{m + 1, n + 1}]))
        return std::numeric_limits<T>::quiet_NaN();
      for (size_t i = 0; i < m; ++i)
        if (B[i] == SIZE_MAX) {
          size_t s = SIZE_MAX;
          for (size_t j = 0; j <= n; ++j)
            if (must_update_s(i, j, s))
              s = j;
          pivot(i, s);
        }
    }
    if (!simplex(2))
      return std::numeric_limits<T>::infinity();
    x.assign(n, 0);
    for (size_t i = 0; i < m; ++i)
      if (B[i] < n)
        x[B[i]] = D[{i, n + 1}];
    return D[{m, n + 1}];
  }

  bool simplex(const int phase) {
    assert(phase == 1 || phase == 2);
    const size_t ix = (phase == 1) ? m + 1 : m;
    for (size_t iter = 0; iter < max_iters; ++iter) {
      size_t s = SIZE_MAX;
      for (size_t j = 0; j <= n; ++j)
        if ((phase == 1 || N[j] != SIZE_MAX) && must_update_s(ix, j, s))
          s = j;
      if (!is_neg(D[{ix, s}]))
        return true; // true if non-negative
      size_t r = SIZE_MAX;
      for (size_t i = 0; i < m; ++i)
        if (is_pos(D[{i, s}]) && must_update_r(s, i, r))
          r = i;
      if (r == SIZE_MAX)
        return false;
      pivot(r, s);
    }
    throw std::domain_error("Infinite loop detected\n");
  }

  void pivot(const size_t r, const size_t s) {
    for (size_t i = 0; i < m + 2; ++i)
      if (i != r)
        for (size_t j = 0; j < n + 2; ++j)
          if (j != s)
            D[{i, j}] -= D[{r, j}] * D[{i, s}] / D[{r, s}];
    for (size_t j = 0; j < n + 2; ++j)
      if (j != s)
        D[{r, j}] /= D[{r, s}];
    for (size_t i = 0; i < m + 2; ++i)
      if (i != r)
        D[{i, s}] /= -D[{r, s}];
    D[{r, s}] = T{1} / D[{r, s}];
    std::swap(B[r], N[s]);
  }

  bool is_pos(T val) const { return val > eps; }
  bool is_neg(T val) const { return val < -eps; }
  bool approx(T v1, T v2) const { return std::fabs(v1 - v2) <= eps; }

  bool must_update_s(size_t i, size_t j, size_t s) const {
    if (s == SIZE_MAX)
      return true;
    const auto dj = D[{i, j}], ds = D[{i, s}];
    if (approx(dj, ds))
      return N[j] < N[s];
    return dj < ds; // true if s = j must be executed
  }

  bool must_update_r(size_t j, size_t i, size_t r) const {
    if (r == SIZE_MAX)
      return true;
    const auto div_i = D[{i, n + 1}] / D[{i, j}];
    const auto div_r = D[{r, n + 1}] / D[{r, j}];
    if (approx(div_i, div_r))
      return B[i] < B[r];
    return div_i < div_r; // true if r = i must be executed
  }

private:
  matrix_t D;
  std::vector<size_t> B, N;
  size_t m{}, n{};
  size_t max_iters = 500;
  T eps = std::numeric_limits<T>::epsilon();
};

} // end namespace djp

#endif // Header guard
