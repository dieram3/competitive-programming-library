//          Copyright Diego Ramírez August, December 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DJP_MATH_SIMPLEX_HPP
#define DJP_MATH_SIMPLEX_HPP

#include <djp/utility/matrix.hpp>

#include <algorithm>   // For std::min_element, std::find
#include <limits>      // For std::numeric_limits
#include <numeric>     // For std::iota
#include <type_traits> // For std::is_floating_point
#include <utility>     // For std::swap
#include <vector>      // For std::vector

#include <cassert> // For assert
#include <cmath>   // For std::fabs
#include <cstddef> // For std::size_t
#include <cstdint> // For SIZE_MAX

namespace djp {

template <typename T>
class simplex_solver {
  static_assert(std::is_floating_point<T>::value, "Must be floating-point");

public:
  using vec_t = std::vector<T>;
  using mat_t = matrix2<T>;

public:
  /// \brief Changes the epsilon value used to compare floats.
  ///
  /// \param val The new epsilon value.
  ///
  /// \pre <tt>eps >= std::numeric_limits<T>::epsilon()</tt>
  ///
  void set_eps(T val) { eps = val; }

  /// \brief Finds the optimal value of the given linear program.
  ///
  /// Uses the simplex algorithm to find the optimal solution of a linear
  /// program. The implementation is guaranteed to converge.
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
  /// \returns The optimal value. If the objetive function is unbounded, returns
  /// <tt>std::numeric_limits<T>::infinity()</tt>. If no feasible solution
  /// exists, returns <tt>std::numeric_limits<T>::quiet_NaN()</tt>.
  ///
  T maximize(const mat_t &A, const vec_t &b, const vec_t &c, vec_t &x) {
    const size_t m = A.rows();
    const size_t n = A.cols();

    // Build tableau.
    tableau.resize(m + 2, n + 2);
    for (size_t i = 0; i < m; ++i) {
      for (size_t j = 0; j < n; ++j)
        tableau[i][j] = A[i][j];
      tableau[i][n] = -1;
      tableau[i][n + 1] = b[i];
    }
    for (size_t j = 0; j < n; ++j) {
      tableau[m][j] = -c[j]; // Row m is the phase2 problem.
      tableau[m + 1][j] = 0; // Row m+1 is the phase1 problem.
    }
    tableau[m][n] = 0, tableau[m][n + 1] = 0;
    tableau[m + 1][n] = 1, tableau[m + 1][n + 1] = 0;

    N.resize(n + 1); // n variables plus 1 artificial variable initially.
    B.resize(m);     // m slack variables initially.
    std::iota(N.begin(), N.end(), size_t{0});
    std::iota(B.begin(), B.end(), N.size());
    pivcol.resize(tableau.rows());
    // 'n' is the artificial variable.

    const size_t min_b = std::min_element(b.begin(), b.end()) - b.begin();

    // Main invariant: right-hand side is always kept positive.
    if (is_neg(b[min_b])) {
      // Phase 1 is required.
      pivot(min_b, n); // Pivot to make RHS positive.
      simplex(1);
      if (!is_zero(tableau[m + 1][n + 1]))
        return std::numeric_limits<T>::quiet_NaN(); // Infeasible.
      auto it = std::find(B.begin(), B.end(), n);
      if (it != B.end()) {
        // Make 'n' a nonbasic variable.
        const size_t row = it - B.begin();
        assert(is_zero(tableau[row][n + 1]));
        size_t col;
        for (col = 0; col <= n; ++col)
          if (!is_zero(tableau[row][col]))
            break;
        assert(col <= n);
        pivot(row, col);
      }
    }

    const size_t art_pos = std::find(N.begin(), N.end(), n) - N.begin();
    assert(art_pos < N.size());
    for (size_t i = 0; i < tableau.rows(); ++i)
      tableau[i][art_pos] = 0; // Nullify the artificial column.

    if (!simplex(2))
      return std::numeric_limits<T>::infinity(); // Unbounded.

    x.assign(n, T{0});
    for (size_t i = 0; i < m; ++i)
      if (B[i] < n)
        x[B[i]] = tableau[i][n + 1];

    return tableau[m][n + 1];
  }

private:
  enum search_result { must_pivot, optimized, unbounded };

  // Requires: A feasible solution must exist.
  bool simplex(const int phase) {
    size_t r{}, c{};
    search_result res;
    while ((res = find_pivot(r, c, phase)) == must_pivot)
      pivot(r, c);
    assert(!(phase == 1 && res == unbounded)); // Phase 1 can't be unbounded.
    return res == optimized;
  }

  // Bland's rule pivot selecting.
  search_result find_pivot(size_t &r, size_t &c, const int phase) {
    const size_t m = tableau.rows() - 2;
    const size_t n = tableau.cols() - 2;
    const size_t objrow = phase == 1 ? m + 1 : m;
    // SIZE_MAX => NIL

    // Find entering variable:
    c = SIZE_MAX;
    for (size_t j = 0; j <= n; ++j) {
      if (!is_neg(tableau[objrow][j]))
        continue;
      if (c == SIZE_MAX || N[j] < N[c])
        c = j;
    }
    if (c == SIZE_MAX)
      return optimized;

    // Find leaving variable:
    r = SIZE_MAX;
    for (size_t i = 0; i < m; ++i) {
      if (!is_pos(tableau[i][c]))
        continue;
      if (r == SIZE_MAX)
        r = i;
      else {
        const auto r_ratio = tableau[r][n + 1] / tableau[r][c];
        const auto i_ratio = tableau[i][n + 1] / tableau[i][c];
        if (approx(r_ratio, i_ratio)) {
          if (B[i] < B[r])
            r = i;
        } else if (i_ratio < r_ratio)
          r = i;
      }
    }
    if (r == SIZE_MAX)
      return unbounded;
    return must_pivot;
  }

  void pivot(const size_t r, const size_t c) {
    for (size_t i = 0; i < tableau.rows(); ++i) {
      pivcol[i] = tableau[i][c];
      tableau[i][c] = 0;
    }
    tableau[r][c] = 1;

    multiply_row(r, 1 / pivcol[r]);
    for (size_t i = 0; i < tableau.rows(); ++i)
      if (i != r)
        add_row(r, -pivcol[i], i);

    std::swap(B[r], N[c]);
  }

  void multiply_row(size_t i, const T mult) {
    for (size_t j = 0; j < tableau.cols(); ++j)
      tableau[i][j] *= mult;
  }

  void add_row(size_t row_from, const T mult, size_t row_to) {
    assert(row_from != row_to);
    for (size_t j = 0; j < tableau.cols(); ++j)
      tableau[row_to][j] += mult * tableau[row_from][j];
  }

  bool is_pos(T val) const { return val > eps; }
  bool is_neg(T val) const { return val < -eps; }
  bool is_zero(T val) const { return std::fabs(val) <= eps; }
  bool approx(T v1, T v2) const { return std::fabs(v1 - v2) <= eps; }

private:
  mat_t tableau;            // Part of the tableau with information.
  vec_t pivcol;             // Temporal storage to pivot.
  std::vector<size_t> B, N; // Basic and nonbasic variables.
  T eps = std::numeric_limits<T>::epsilon();
};

} // end namespace djp

#endif // Header guard
