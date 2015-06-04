//          Copyright Diego Ramírez October 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DJP_DATA_STRUCTURE_SEGMENT_TREE_HPP
#define DJP_DATA_STRUCTURE_SEGMENT_TREE_HPP

#include <algorithm> // for std::copy
#include <vector>    // for std::vector
#include <cstddef>   // for std::size_t

namespace djp {

// The idea of this segment tree implementation was taken from
// http://coutcode.com/blog/segment-trees-prt1/
template <typename T, typename BinaryOp>
class segment_tree {
  size_t elems;
  BinaryOp op;
  std::vector<T> tree;

public:
  segment_tree(size_t sz = 0) : elems{sz}, op(), tree(2 * elems) {}

  template <class ForwardIt>
  segment_tree(ForwardIt first, ForwardIt last)
      : segment_tree(std::distance(first, last)) {
    if (first == last)
      return;
    std::copy(first, last, tree.begin() + elems);
    for (size_t i = elems - 1; i > 0; --i)
      tree[i] = op(tree[i << 1], tree[i << 1 | 1]);
  }

  template <class ForwardIt>
  void assign(ForwardIt first, ForwardIt last) {
    *this = segment_tree(first, last);
  }

  void modify(size_t pos, const T &value) {
    tree[pos += elems] = value;
    for (; pos > 1; pos >>= 1)
      tree[pos >> 1] = (tree[pos] + tree[pos ^ 1]);
  }

  T accumulate(size_t l, size_t r) const {
    T ans = elem(l++);
    for (l += elems, r += elems; l < r; l >>= 1, r >>= 1) {
      if (l & 1)
        ans = op(ans, tree[l++]);
      if (r & 1)
        ans = op(ans, tree[--r]);
    }
    return ans;
  }

  T elem(size_t pos) const { return tree[elems + pos]; }
  size_t size() const { return elems; }
};

} // end namespace djp

#endif // Header guard
