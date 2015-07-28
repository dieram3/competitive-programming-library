//          Copyright Diego Ramírez May 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DJP_DATA_STRUCTURE_DISJOINT_SET_HPP
#define DJP_DATA_STRUCTURE_DISJOINT_SET_HPP

#include <vector>    // for std::vector
#include <algorithm> // for std::max
#include <utility>   // for std::swap
#include <cstddef>   // for std::size_t

namespace djp {

class disjoint_set {
public:
  struct node {
    const node *parent = nullptr;
    size_t depth = 0;
    size_t num_elements = 1; // Note: This field is not neccesary.
  };

  // sets are indexed from 0 to num_sets - 1
  disjoint_set(size_t num_sets) : nodes_(num_sets) {}

  const node *find_root(size_t i) const {
    const node *px = &nodes_[i];
    while (px->parent)
      px = px->parent;
    return px;
  }

  /// \brief Makes an union between a pair of sets.
  ///
  /// \param i Index of element contained on the first set.
  /// \param j Index of element contained on the second set.
  ///
  /// Makes a union between the set contained the first element and the set
  /// containing the second element. If both elements belong to the same set,
  /// this function does noting.
  ///
  /// \returns \c true if the elements i and j was part of different sets so
  /// an union was made, \c false otherwise.
  ///
  bool make_union(size_t i, size_t j) {
    auto px = find_root(i);
    auto py = find_root(j);
    if (px == py)
      return false;
    if (px->depth < py->depth)
      std::swap(px, py);
    auto &parent = const_cast<node &>(*px);
    auto &child = const_cast<node &>(*py);
    child.parent = &parent;
    parent.num_elements += child.num_elements;
    parent.depth = std::max(parent.depth, child.depth + 1);
    return true;
  }

private:
  std::vector<node> nodes_;
};

} // namespace djp

#endif // Header guard
