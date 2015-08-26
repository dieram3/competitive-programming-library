//          Copyright Diego Ramírez August 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DJP_NUMERICAL_SIMPLEX_HPP
#define DJP_NUMERICAL_SIMPLEX_HPP

// This implementation of simplex is based on:
// https://web.stanford.edu/~liszt90/acm/notebook.html#file17

#include <djp/utility/matrix.hpp>
#include <algorithm>   // For std::swap
#include <limits>      // For std::numeric_limits
#include <type_traits> // For std::is_floating_point
#include <vector>      // For std::vector
#include <cassert>     // For assert
#include <cmath>       // For std::fabs
#include <cstddef>     // For std::size_t
#include <cstdint>     // For SIZE_MAX

namespace djp {

/// \brief Finds the optimal solution of a linear program.
///
/// Uses the simplex algorithm for solving linear programs of the form:
///
/// \li Maximize <tt>c^T*x</tt>
/// \li Subject to <tt>A*x <= b</tt>
/// \li and <tt>x >= 0</tt>
///
/// \tparam T A floating-point type.
///
/// \param A Matrix of \c m rows and \c n columns.
/// \param b An <tt>m</tt>-dimensional vector.
/// \param c An <tt>n</tt>-dimensional vector.
/// \param[out] x Vector where the optimal solution will be stored.
/// \param eps The epsilon value to compare floats.
///
/// \returns Value of the optimal solution. If the solution is unbounded
/// returns <tt>std::numeric_limits<T>::infinity()</tt>.
///
/// \pre <tt>std::fabs(eps) >= std::numeric_limits<T>::epsilon()</tt>.
///
/// \note If \p A, \p b and \p c are restricted to integer values and the
/// matrix \p A is totally unimodular, then it is guaranteed that every basic
/// feasible solution is restricted to integer values.
///
template <typename T>
T simplex_maximize(const matrix<T> &A, const std::vector<T> &b,
                   const std::vector<T> &c, std::vector<T> &x,
                   const T eps = std::numeric_limits<T>::epsilon()) {

  static_assert(std::is_floating_point<T>::value, "Must be floating-point");
  assert(A.rows() == b.size());
  assert(A.cols() == c.size());
  enum phase_t { phase_1, phase_2 };

  const size_t m = A.rows();
  const size_t n = A.cols();
  std::vector<size_t> B(m);
  std::vector<size_t> N(n + 1);
  matrix<T> D({m + 2, n + 2});

  // Init data
  // =========
  for (size_t i = 0; i != m; ++i)
    for (size_t j = 0; j != n; ++j)
      D[{i, j}] = A[{i, j}];
  for (size_t i = 0; i != m; ++i)
    B[i] = n + i, D[{i, n}] = -1, D[{i, n + 1}] = b[i];
  for (size_t j = 0; j != n; ++j)
    N[j] = j, D[{m, j}] = -c[j];
  N[n] = SIZE_MAX;
  D[{m + 1, n}] = 1;

  // Auxiliary functions
  // ===================
  auto approx =
      [eps](const T v1, const T v2) { return std::fabs(v1 - v2) < eps; };

  auto relax_s = [&](const size_t i, const size_t j, size_t &s) {
    const auto dj = D[{i, j}], ds = D[{i, s}];
    if (s == SIZE_MAX || dj < ds || (approx(dj, ds) && N[j] < N[s]))
      s = j;
  };
  auto relax_r = [&](const size_t j, const size_t i, size_t &r) {
    const auto div_i = D[{i, n + 1}] / D[{i, j}];
    const auto div_r = D[{r, n + 1}] / D[{r, j}];
    if (r == SIZE_MAX || div_i < div_r || (approx(div_i, div_r) && B[i] < B[r]))
      r = i;
  };
  auto pivot = [&](const size_t r, const size_t s) {
    for (size_t i = 0; i != m + 2; ++i)
      if (i != r)
        for (size_t j = 0; j != n + 2; ++j)
          if (j != s)
            D[{i, j}] -= D[{r, j}] * D[{i, s}] / D[{r, s}];
    for (size_t j = 0; j != n + 2; ++j)
      if (j != s)
        D[{r, j}] /= D[{r, s}];
    for (size_t i = 0; i != m + 2; ++i)
      if (i != r)
        D[{i, s}] /= -D[{r, s}];
    D[{r, s}] = T{1} / D[{r, s}];
    std::swap(B[r], N[s]);
  };
  auto simplex = [&](const phase_t phase) {
    const size_t ix = (phase == phase_1) ? m + 1 : m;
    while (true) {
      size_t s = SIZE_MAX;
      for (size_t j = 0; j != n + 1; ++j)
        if (phase == phase_1 || N[j] != SIZE_MAX)
          relax_s(ix, j, s);
      if (D[{ix, s}] >= -eps)
        return true;
      size_t r = SIZE_MAX;
      for (size_t i = 0; i != m; ++i)
        if (D[{i, s}] > 0)
          relax_r(s, i, r);
      if (r == SIZE_MAX)
        return false;
      pivot(r, s);
    }
  };

  // Solve the system
  // ================
  size_t r = 0;
  for (size_t i = 1; i != m; ++i)
    if (D[{i, n + 1}] < D[{r, n + 1}])
      r = i;
  if (D[{r, n + 1}] <= -eps) {
    pivot(r, n);
    if (!simplex(phase_1) || D[{m + 1, n + 1}] < -eps)
      return -std::numeric_limits<T>::infinity();
    for (size_t i = 0; i != m; ++i)
      if (B[i] == SIZE_MAX) {
        size_t s = SIZE_MAX;
        for (size_t j = 0; j != n + 1; ++j)
          relax_s(i, j, s);
        pivot(i, s);
      }
  }
  if (!simplex(phase_2))
    return std::numeric_limits<T>::infinity();
  x.assign(n, 0);
  for (size_t i = 0; i != m; ++i)
    if (B[i] < n)
      x[B[i]] = D[{i, n + 1}];
  return D[{m, n + 1}];
}

} // end namespace djp

#endif // Header guard
