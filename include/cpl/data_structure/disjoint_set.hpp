//          Copyright Diego Ramirez 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef CPL_DATA_STRUCTURE_DISJOINT_SET_HPP
#define CPL_DATA_STRUCTURE_DISJOINT_SET_HPP

#include <cstddef> // size_t
#include <memory>  // unique_ptr

namespace cpl {

class disjoint_set {
  std::unique_ptr<size_t[]> parent, rank;

public:
  /// \brief Constructs a disjoint-set object.
  ///
  /// Creates \p n elements indexed with integers from \c 0 to <tt>n - 1</tt>.
  /// Initially, each element belongs to a singleton set (make-set is done for
  /// each of them).
  ///
  /// \param n The number of elements to be used.
  ///
  /// \par Complexity
  /// Linear in the number of elements.
  ///
  explicit disjoint_set(size_t n) : parent(new size_t[n]), rank(new size_t[n]) {
    for (size_t x = 0; x != n; ++x) {
      parent[x] = x;
      rank[x] = 0;
    }
  }

  /// \brief Finds the representative element of the set containing \p x.
  ///
  /// \param x Member of the set to be searched.
  ///
  /// \returns The index of the representative element of the searched set.
  ///
  /// \par Complexity
  /// The amortized complexity is constant.
  ///
  size_t find_set(size_t x) {
    if (parent[x] != x)
      parent[x] = find_set(parent[x]);
    return parent[x];
  }

  /// \brief Merges the set containing \p x with the set containing \p y.
  ///
  /// \param x Member of the first set.
  /// \param y Member of the second set.
  ///
  /// \returns \c true if \p x and \p y were part of different sets so an union
  /// was made. Otherwise \c false.
  ///
  /// \par Complexity
  /// The amortized complexity is constant.
  ///
  bool union_set(size_t x, size_t y) {
    x = find_set(x);
    y = find_set(y);
    if (x == y)
      return false;

    if (rank[x] < rank[y]) {
      parent[x] = y;
    } else if (rank[x] > rank[y]) {
      parent[y] = x;
    } else {
      parent[y] = x;
      ++rank[x];
    }
    return true;
  }
};

} // namespace cpl

#endif // Header guard
