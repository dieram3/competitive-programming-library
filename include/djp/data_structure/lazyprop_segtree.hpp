//          Copyright Diego Ramírez November 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DJP_DATA_STRUCTURE_LAZYPROP_SEGTREE_HPP
#define DJP_DATA_STRUCTURE_LAZYPROP_SEGTREE_HPP

#include <functional> // For std::function
#include <iterator>   // For std::distance
#include <vector>     // For std::vector
#include <cassert>    // For assert
#include <cstddef>    // For std::size_t

namespace djp {

/// \brief Implements a generic segment tree with lazy propagation.
///
/// \tparam T The value type.
/// \tparam Combiner Binary functor which combines a pair of values.
/// \tparam OpList The type to represent a list of modify-operations.
///
/// The Binary function is required to be associative, but not commutative.
///
/// <tt>OpList</tt> must have the following member functions.
///
/// <tt>void push(const OpList& ops)</tt>:
/// Appends the operations enqueued in <tt>ops</tt> to <tt>*this</tt>.
///
/// <tt>T apply(size_t range_size, const T& reduced_value)</tt>:
/// Returns the value of applying the enqueued operations to a range of
/// <tt>range_size</tt> elements whose reduced value is <tt>reduced_value</tt>.
///
/// <tt>void clear()</tt>:
/// Clears the list of operations, so <tt>apply(N, x)</tt> returns <tt>x</tt>.
///
/// <tt>bool empty()</tt>:
/// Returns \c true if the queue is empty.
///
template <typename T, typename Combiner, typename OpList>
class lazyprop_segtree {
  struct node_t {
    node_t(size_t b, size_t e, size_t pos_ = 1)
        : beg{b}, end{e}, mid{(b + e) / 2}, pos{pos_} {}

    size_t beg, end, mid, pos;

    size_t size() const { return end - beg; }
    size_t lpos() const { return 2 * pos; }
    size_t rpos() const { return 2 * pos + 1; }
    node_t left() const { return node_t(beg, mid, lpos()); }
    node_t right() const { return node_t(mid, end, rpos()); }
    bool leaf() const { return size() == 1; }
  };

  node_t root() const { return node_t(0, num_elems); }

public:
  // TODO: Document member functions

  template <typename ForwardIt>
  lazyprop_segtree(ForwardIt first, ForwardIt last, Combiner c = Combiner())
      : num_elems(std::distance(first, last)), comb(c) {
    assert(first != last);
    {
      // Find optimal size
      node_t nd = root();
      while (!nd.leaf())
        nd = nd.right();
      values.resize(nd.pos + 1); // size is 4*(N-1) in the worst case
      ops.resize(nd.pos + 1);    // 128 requires 256, 129 requires 512
    }
    // Recurse the tree
    std::function<void(const node_t &)> recurse;
    recurse = [&](const node_t &nd) {
      if (nd.leaf()) {
        values[nd.pos] = *first++;
        return;
      }
      recurse(nd.left());
      recurse(nd.right());
      values[nd.pos] = comb(values[nd.lpos()], values[nd.rpos()]);
    };
    recurse(root());
    assert(first == last);
  }

  void apply(size_t beg, size_t end, const OpList &op) {
    assert(beg < end);
    std::function<void(const node_t &)> recurse;
    recurse = [&](const node_t &nd) {
      if (nd.beg >= beg && nd.end <= end) {
        ops[nd.pos].push(op);
        return;
      }
      push_down_ops(nd); // Needed because children need updated values.
      const auto lhs = nd.left();
      const auto rhs = nd.right();
      if (lhs.end > beg)
        recurse(lhs);
      if (rhs.beg < end)
        recurse(rhs);
      push_down_ops(lhs);
      push_down_ops(rhs);
      values[nd.pos] = comb(values[lhs.pos], values[rhs.pos]);
    };
    recurse(root());
  }

  T reduce(const size_t beg, const size_t end) {
    assert(beg < end);
    std::function<T(const node_t &)> recurse;
    recurse = [&](const node_t &nd) {
      push_down_ops(nd);

      if (nd.beg >= beg && nd.end <= end)
        return values[nd.pos];

      const auto lhs = nd.left();
      const auto rhs = nd.right();
      if (end <= lhs.end)
        return recurse(lhs);
      if (beg >= rhs.beg)
        return recurse(rhs);
      return comb(recurse(lhs), recurse(rhs));
    };
    return recurse(root());
  }

private:
  void push_down_ops(const node_t &nd) {
    if (ops[nd.pos].empty())
      return;
    values[nd.pos] = ops[nd.pos].apply(nd.size(), values[nd.pos]);
    if (!nd.leaf()) {
      ops[nd.lpos()].push(ops[nd.pos]);
      ops[nd.rpos()].push(ops[nd.pos]);
    }
    ops[nd.pos].clear();
  }

private:
  size_t num_elems;
  Combiner comb;
  std::vector<T> values;
  std::vector<OpList> ops;
};

} // end namespace djp

#endif // Header guard
