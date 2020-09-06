//          Copyright Diego Ramirez 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef CPL_DATA_STRUCTURE_FENWICK_TREE_HPP
#define CPL_DATA_STRUCTURE_FENWICK_TREE_HPP

#include <cstddef>
#include <vector>

namespace cpl {

// Implementation of a Fenwick tree, a.k.a. Binary Indexed Tree (BIT).
// This can be used to modify an array and compute prefix sums efficiently.
//
// Note: This implementation uses arithmetic addition to combine elements, but
// any commutative operation will work.
template <class T>
class fenwick_tree {
public:
  using size_type = std::size_t;

  // Constructs a fenwick tree for `count` elements.
  explicit fenwick_tree(size_type count) : m_tree(count + 1) {}

  // Returns the sum of the first `count` elements.
  // Complexity: O(log(N)), where N is the size of the tree.
  T get_prefix_sum(size_type count) const {
    T result = 0;
    while (count != 0) {
      result += m_tree[count];
      count = reset_lowest_set_bit(count);
    }
    return result;
  }

  // Adds `value` to the element at position 'index'.
  // Complexity: O(log(N)), where N is the size of the tree.
  void add(size_type index, T value) {
    size_type count = index + 1;

    while (count < m_tree.size()) {
      m_tree[count] += value;
      count += extract_lowest_set_bit(count);
    }
  }

private:
  static constexpr size_type reset_lowest_set_bit(size_type x) {
    return x & (x - 1);
  }

  static constexpr size_type extract_lowest_set_bit(size_type x) {
    return x & (-x);
  }

  std::vector<T> m_tree;
};

} // namespace cpl

#endif // Header guard
