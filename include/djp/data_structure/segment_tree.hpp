//          Copyright Diego Ramírez October 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DJP_DATA_STRUCTURE_SEGMENT_TREE_HPP
#define DJP_DATA_STRUCTURE_SEGMENT_TREE_HPP

#include <vector>
#include <utility>
#include <cassert>
#include <cstddef>

namespace djp {

template <class T, class BinaryOp>
class segment_tree {
  size_t size_;
  BinaryOp op_;
  std::vector<T> tree_;

public:
  using pos_type = std::size_t;

  struct range {
    pos_type begin;
    pos_type end;
  };

private:
  struct node {
    node(pos_type b, pos_type e, size_t tp)
        : begin{b}, end{e}, middle{b + (e - b) / 2}, tree_pos{tp} {}

    bool is_leaf() const { return begin + 1 == end; }
    node left() const { return {begin, middle, 2 * tree_pos + 1}; }
    node right() const { return {middle, end, 2 * tree_pos + 2}; }

    pos_type begin;
    pos_type end;
    pos_type middle;
    size_t tree_pos;
  };

public:
  /// Constructs a segment tree of with \p sz elements value initialized.
  explicit segment_tree(size_t sz = 0, BinaryOp op = {})
      : size_{sz}, op_(op), tree_() {}

  /// Constructs a segment tree with the elements in the range [first, last)
  /// using the associative binary operator op.
  /// Complexity: 2*(last - first) applications of op.
  template <class ForwardIt>
  segment_tree(ForwardIt first, ForwardIt last, BinaryOp op = {})
      : size_(std::distance(first, last)), op_(op),
        tree_(optimal_tree_size(size_)) {
    for_each(0, size_, [first](T &elem) mutable { elem = *first++; });
  }

  /// Reconstruct the segment tree with the elements in the range [first, last)
  /// using the associative binary operator op.
  /// Complexity: 2*(last - first) applications of op.
  template <class ForwardIt>
  void assign(ForwardIt first, ForwardIt last, BinaryOp op = {}) {
    *this = segment_tree(first, last, op);
  }

  /// Computes the sum of the elements positioned in the range [first, last)
  /// Complexity: log(N) applications of op, where N == size()
  T accumulate(pos_type first, pos_type last) const {
    assert(first < last);
    return accumulate(root(), {first, last});
  }

  /// Applies the given function f to each element of the segmente tree
  /// positioned in the range [first, last).
  /// Complexity: max(log(N), 2*k) applications of op and k applications of f
  /// where N == size() and k == last - first
  template <class UnaryFunction>
  void for_each(pos_type first, pos_type last, UnaryFunction f) {
    for_each(root(), {first, last}, f);
  }

  /// Returns the number of elements of the segment tree.
  /// Complexity: Constant
  size_t size() const { return size_; }

private:
  node root() const { return {0, size(), 0}; }

  T &elem(const node &nd) { return tree_[nd.tree_pos]; }

  const T &elem(const node &nd) const { return tree_[nd.tree_pos]; }

  static size_t optimal_tree_size(size_t sz) {
    if (!sz)
      return 0;
    size_t res = 2;
    while (sz != 1)
      sz = sz / 2 + sz % 2, res *= 2;
    return res - 1;
  }

  T accumulate(const node &nd, range target) const {
    if (nd.begin == target.begin && nd.end == target.end)
      return elem(nd);
    assert(!nd.is_leaf());

    if (target.end <= nd.middle)
      return accumulate(nd.left(), target);
    if (target.begin >= nd.middle)
      return accumulate(nd.right(), target);

    const auto lsum = accumulate(nd.left(), {target.begin, nd.middle});
    const auto rsum = accumulate(nd.right(), {nd.middle, target.end});
    return op_(lsum, rsum);
  }

  template <class UnaryFunc>
  void for_each(const node &nd, range target, UnaryFunc &f) {
    if (nd.is_leaf())
      return f(elem(nd));

    const auto lchild = nd.left();
    const auto rchild = nd.right();

    if (target.begin < nd.middle)
      for_each(lchild, target, f);
    if (target.end > nd.middle)
      for_each(rchild, target, f);

    elem(nd) = op_(elem(lchild), elem(rchild));
  }
};

// TODO: Iterative segment tree

} // namespace djp

#endif // Header guard
