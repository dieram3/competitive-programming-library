//          Copyright Diego Ramírez October 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DJP_CONTAINER_SEGMENT_TREE_HPP
#define DJP_CONTAINER_SEGMENT_TREE_HPP

#include <vector>
#include <utility>
#include <cassert>

namespace djp {

template <class T, class BinaryOp>
class segment_tree {
 public:
  using value_type = T;
  using size_type = std::size_t;
  using pos_type = std::size_t;

 private:
  using range_t = std::pair<pos_type, pos_type>;

  struct node {
    node(pos_type b, pos_type e, size_type tp)
        : begin{b}, end{e}, middle{b + (e - b) / 2}, tree_pos{tp} {}

    bool is_leaf() const { return begin + 1 == end; }
    node left() const { return {begin, middle, 2 * tree_pos + 1}; }
    node right() const { return {middle, end, 2 * tree_pos + 2}; }

    pos_type begin;
    pos_type end;
    pos_type middle;
    size_type tree_pos;
  };

 public:
  /// Constructs the segment tree with the elements in the range [first, last)
  /// using the associative binary operator op.
  /// Complexity: 2*(last - first) applications of op.
  template <class ForwardIt>
  segment_tree(ForwardIt first, ForwardIt last, BinaryOp op = {})
      : size_(std::distance(first, last)),
        op_(op),
        tree_(optimal_tree_size(size_)) {
    auto take_element = [first](T& value) mutable { value = *first++; };
    for_each(0, size(), take_element);
  }

  // Computes the sum of the elements positioned in the range [first, last)
  // Complexity: log(N) applications of op, where N == size()
  T accumulate(pos_type first, pos_type last) const {
    return accumulate(root(), first, last);
  }

  // Applies the given function f to each element of the segmente tree
  // positioned in the range [first, last).
  // Complexity: max(log(N), 2*k) applications of op and k applications of f
  // where N == size() and k == last - first
  template <class UnaryFunction>
  void for_each(pos_type first, pos_type last, UnaryFunction f) {
    for_each(root(), range_t(first, last), f);
  }

  /// Returns the number of elements of the segment tree.
  /// Complexity: Constant
  size_type size() const { return size_; }

 private:
  node root() const { return {0, size(), 0}; }

  T& elem(const node& nd) { return tree_[nd.tree_pos]; }

  const T& elem(const node& nd) const { return tree_[nd.tree_pos]; }

  static size_type optimal_tree_size(size_type sz) {
    if (!sz) return 0;
    size_type res = 2;
    while (sz != 1) sz = sz / 2 + sz % 2, res *= 2;
    return res - 1;
  }

  T accumulate(const node& nd, pos_type first, pos_type last) const {
    if (nd.begin == first && nd.end == last) return elem(nd);
    assert(!nd.is_leaf());

    if (last <= nd.middle) return accumulate(nd.left(), first, last);
    if (first >= nd.middle) return accumulate(nd.right(), first, last);

    const auto lsum = accumulate(nd.left(), first, nd.middle);
    const auto rsum = accumulate(nd.right(), nd.middle, last);
    return op_(lsum, rsum);
  }

  template <class UnaryFunc>
  void for_each(const node& nd, const range_t& target, UnaryFunc& f) {
    if (nd.is_leaf()) return f(elem(nd));

    const auto lchild = nd.left();
    const auto rchild = nd.right();

    if (target.first < nd.middle) for_each(lchild, target, f);
    if (target.second > nd.middle) for_each(rchild, target, f);

    elem(nd) = op_(elem(lchild), elem(rchild));
  }

 private:  // Private data members
  size_type size_;
  BinaryOp op_;
  std::vector<T> tree_;
};

}  // namespace djp

#endif  // Header guard
