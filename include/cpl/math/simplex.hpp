//          Copyright Diego Ramirez 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef CPL_MATH_SIMPLEX_HPP
#define CPL_MATH_SIMPLEX_HPP

#include <cpl/utility/matrix.hpp> // matrix2
#include <algorithm>              // min_element, find
#include <cassert>                // assert
#include <cmath>                  // fabs
#include <cstddef>                // size_t
#include <cstdint>                // SIZE_MAX
#include <iterator>               // begin, end
#include <limits>                 // numeric_limits
#include <numeric>                // iota
#include <type_traits>            // is_floating_point
#include <utility>                // swap
#include <vector>                 // vector

namespace cpl {

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
  void set_eps(T val) {
    eps = val;
  }

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
  /// \param a Matrix of \c m rows and \c n columns.
  /// \param b An <tt>m</tt>-dimensional vector.
  /// \param c An <tt>n</tt>-dimensional vector.
  /// \param[out] x Vector where the optimal solution will be stored.
  ///
  ///
  /// \returns The optimal value. If the objetive function is unbounded, returns
  /// <tt>std::numeric_limits<T>::infinity()</tt>. If no feasible solution
  /// exists, returns <tt>std::numeric_limits<T>::quiet_NaN()</tt>.
  ///
  T maximize(const mat_t& a, const vec_t& b, const vec_t& c, vec_t& x) {
    const size_t m = a.num_rows();
    const size_t n = a.num_cols();

    // Build tableau.
    tableau.resize(m + 2, n + 2);
    for (size_t i = 0; i < m; ++i) {
      for (size_t j = 0; j < n; ++j)
        tableau[i][j] = a[i][j];
      tableau[i][n] = -1;
      tableau[i][n + 1] = b[i];
    }
    for (size_t j = 0; j < n; ++j) {
      tableau[m][j] = -c[j]; // Row m is the phase2 problem.
      tableau[m + 1][j] = 0; // Row m+1 is the phase1 problem.
    }
    tableau[m][n] = 0, tableau[m][n + 1] = 0;
    tableau[m + 1][n] = 1, tableau[m + 1][n + 1] = 0;

    nonbasic.resize(n + 1); // n variables plus 1 artificial variable initially.
    basic.resize(m);        // m slack variables initially.
    std::iota(begin(nonbasic), end(nonbasic), size_t{0});
    std::iota(begin(basic), end(basic), nonbasic.size());
    pivcol.resize(tableau.num_rows());
    // 'n' is the artificial variable.

    const size_t min_b = std::min_element(begin(b), end(b)) - begin(b);

    // Main invariant: right-hand side is always kept positive.
    if (is_neg(b[min_b])) {
      // Phase 1 is required.
      pivot(min_b, n); // Pivot to make RHS positive.
      simplex(1);
      if (!is_zero(tableau[m + 1][n + 1]))
        return std::numeric_limits<T>::quiet_NaN(); // Infeasible.
      const auto it = std::find(begin(basic), end(basic), n);
      if (it != basic.end()) {
        // Make 'n' a nonbasic variable.
        const size_t row = it - begin(basic);
        assert(is_zero(tableau[row][n + 1]));
        size_t col;
        for (col = 0; col <= n; ++col)
          if (!is_zero(tableau[row][col]))
            break;
        assert(col <= n);
        pivot(row, col);
      }
    }

    const size_t art_pos =
        std::find(begin(nonbasic), end(nonbasic), n) - begin(nonbasic);
    assert(art_pos < nonbasic.size());
    for (size_t i = 0; i < tableau.num_rows(); ++i)
      tableau[i][art_pos] = 0; // Nullify the artificial column.

    if (!simplex(2))
      return std::numeric_limits<T>::infinity(); // Unbounded.

    x.assign(n, T{0});
    for (size_t i = 0; i < m; ++i)
      if (basic[i] < n)
        x[basic[i]] = tableau[i][n + 1];

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
  search_result find_pivot(size_t& r, size_t& c, const int phase) {
    const size_t m = tableau.num_rows() - 2;
    const size_t n = tableau.num_cols() - 2;
    const size_t objrow = phase == 1 ? m + 1 : m;
    // SIZE_MAX => NIL

    // Find entering variable:
    c = SIZE_MAX;
    for (size_t j = 0; j <= n; ++j) {
      if (!is_neg(tableau[objrow][j]))
        continue;
      if (c == SIZE_MAX || nonbasic[j] < nonbasic[c])
        c = j;
    }
    if (c == SIZE_MAX)
      return optimized;

    // Find leaving variable:
    auto ratio_less = [&](const size_t r1, const size_t r2) {
      const auto ratio1 = tableau[r1][n + 1] / tableau[r1][c];
      const auto ratio2 = tableau[r2][n + 1] / tableau[r2][c];
      if (approx(ratio1, ratio2))
        return basic[r1] < basic[r2];
      return ratio1 < ratio2;
    };
    r = SIZE_MAX;
    for (size_t i = 0; i < m; ++i) {
      if (!is_pos(tableau[i][c]))
        continue;
      if (r == SIZE_MAX || ratio_less(i, r))
        r = i;
    }
    if (r == SIZE_MAX)
      return unbounded;
    return must_pivot;
  }

  void pivot(const size_t r, const size_t c) {
    for (size_t i = 0; i < tableau.num_rows(); ++i) {
      pivcol[i] = tableau[i][c];
      tableau[i][c] = 0;
    }
    tableau[r][c] = 1;

    multiply_row(r, 1 / pivcol[r]);
    for (size_t i = 0; i < tableau.num_rows(); ++i)
      if (i != r)
        add_row(r, -pivcol[i], i);

    std::swap(basic[r], nonbasic[c]);
  }

  void multiply_row(size_t i, const T mult) {
    for (size_t j = 0; j < tableau.num_cols(); ++j)
      tableau[i][j] *= mult;
  }

  void add_row(size_t row_from, const T mult, size_t row_to) {
    assert(row_from != row_to);
    for (size_t j = 0; j < tableau.num_cols(); ++j)
      tableau[row_to][j] += mult * tableau[row_from][j];
  }

  bool is_pos(T val) const {
    return val > eps;
  }
  bool is_neg(T val) const {
    return val < -eps;
  }
  bool is_zero(T val) const {
    return std::fabs(val) <= eps;
  }
  bool approx(T v1, T v2) const {
    return std::fabs(v1 - v2) <= eps;
  }

private:
  mat_t tableau;                       // Part of the tableau with information.
  vec_t pivcol;                        // Temporal storage to pivot.
  std::vector<size_t> basic, nonbasic; // Basic and nonbasic variables.
  T eps = std::numeric_limits<T>::epsilon();
};

} // end namespace cpl

#endif // Header guard
