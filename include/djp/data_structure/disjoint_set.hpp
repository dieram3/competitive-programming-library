//          Copyright Diego Ramírez May 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DJP_DATA_STRUCTURE_DISJOINT_SET_HPP
#define DJP_DATA_STRUCTURE_DISJOINT_SET_HPP

#include <algorithm> // for std::max
#include <memory>    // for std::unique_ptr
#include <utility>   // for std::swap
#include <cstddef>   // for std::size_t

namespace djp {

class disjoint_set {
  std::unique_ptr<size_t[]> parent, rank;

public:
  /// \brief Constructs a disjoint-set object.
  ///
  /// It reserves enough storage to maintain \p n different elements. The
  /// elements will be indexed with integers from \c 0 to <tt>n - 1</tt>.
  ///
  /// The state of each element is initially undefined so \p make_set must be
  /// called for it before being queried or merged. Using an uinitialized
  /// element invokes undefined behaviour.
  ///
  /// \param n The number of elements to be used.
  ///
  /// \par Complexity
  /// Constant.
  ///
  disjoint_set(size_t n) : parent(new size_t[n]), rank(new size_t[n]) {}

  /// \brief Removes the given element to a singleton set.
  ///
  /// \param x The element to be removed.
  ///
  /// \par Complexity
  /// Constant.
  ///
  void make_set(size_t x) {
    parent[x] = x;
    rank[x] = 0;
  }

  /// \brief Finds the standard representative of the set containing \p x.
  ///
  /// \param x Member of the set to be searched.
  ///
  /// \par Complexity
  /// It uses path compression so the amortized complexity is constant.
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
  /// was made. Otherwise returns \c false.
  ///
  /// \par Complexity
  /// The amortized complexity is constant.
  ///
  bool union_set(size_t x, size_t y) {
    x = find_set(x);
    y = find_set(y);
    if (x == y)
      return false;
    if (rank[x] < rank[y])
      parent[x] = y;
    else if (rank[x] > rank[y])
      parent[y] = x;
    else
      parent[y] = x, ++rank[x];
    return true;
  }
};

} // namespace djp

#endif // Header guard
