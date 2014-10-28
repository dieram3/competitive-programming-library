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

 private:
  using range_t = std::pair<size_type, size_type>;

  struct node {
    node(size_type b, size_type e, size_type p)
        : begin{b}, end{e}, middle{b + (e - b) / 2}, pos{p} {}

    bool is_leaf() const { return begin + 1 == end; }

    node left() const { return {begin, middle, 2 * pos + 1}; }

    node right() const { return {middle, end, 2 * pos + 2}; }

    size_type begin;
    size_type end;
    size_type middle;
    size_type pos;
  };

 public:
  /// Constructs the segment tree with the elements in the range [first, last)
  /// using the associative operator op.
  /// Complexity: (last - first) applications of op.
  template <class ForwardIt>
  segment_tree(ForwardIt first, ForwardIt last, BinaryOp op = BinaryOp())
      : size_(std::distance(first, last)),
        op_(op),
        tree_(optimal_tree_size(size_)) {
    auto take_element = [&first](T &value) { value = *first++; };
    for_each(0, size(), take_element);
  }

  /// Computes the sum of the elements in the range [first, last)
  /// Complexity: log(N) applications of op, where N == size()
  T accumulate(size_type first, size_type last) const {
    return accumulate(root(), range_t(first, last));
  }

  /// Applies the given function f to each element in the range [first, last)
  /// Complexity: max(log(N), 2*k) aplications of op and k applications of f
  /// where N == size() and k == last - first
  template <class UnaryFunction>
  void for_each(size_type first, size_type last, UnaryFunction f) {
    for_each(root(), range_t(first, last), f);
  }

  /// Returns the number of elements.
  /// Complexity: Constant
  size_type size() const { return size_; }

 private:
  node root() const { return {0, size(), 0}; }

  T &elem(const node &nd) { return tree_[nd.pos]; }

  const T &elem(const node &nd) const { return tree_[nd.pos]; }

  static size_type optimal_tree_size(size_type sz) {
    if (!sz) return 0;
    size_type res = 2;
    while (sz != 1) sz = sz / 2 + sz % 2, res *= 2;
    return res - 1;
  }

  T accumulate(const node &nd, const range_t &target) const {
    if (nd.begin >= target.first && nd.end <= target.second) return elem(nd);
    assert(!nd.is_leaf());

    if (target.second <= nd.middle) return accumulate(nd.left(), target);
    if (target.first >= nd.middle) return accumulate(nd.right(), target);

    const auto lsum = accumulate(nd.left(), target);
    const auto rsum = accumulate(nd.right(), target);
    return op_(lsum, rsum);
  }

  template <class UnaryFunc>
  void for_each(const node &nd, const range_t &target, UnaryFunc &f) {
    if (nd.begin >= target.second || nd.end <= target.first) return;
    if (nd.is_leaf()) return f(elem(nd));

    const auto lchild = nd.left();
    const auto rchild = nd.right();
    for_each(lchild, target, f);
    for_each(rchild, target, f);
    elem(nd) = op_(elem(lchild), elem(rchild));
  }

 private:  // Private data members
  size_type size_;
  BinaryOp op_;
  std::vector<T> tree_;
};

}  // namespace djp

#endif  // Header guard
