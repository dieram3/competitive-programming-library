//          Copyright Diego Ramírez November 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DJP_DATA_STRUCTURE_LAZYPROP_SEGTREE_HPP
#define DJP_DATA_STRUCTURE_LAZYPROP_SEGTREE_HPP

#include <iterator> // For std::distance
#include <vector>   // For std::vector
#include <cassert>  // For assert
#include <cstddef>  // For std::size_t

namespace djp {

/// \brief Generic segment tree with lazy propagation.
///
/// The memory complexity is linear in the number of elements to be stored.
///
/// \tparam T The value type.
/// \tparam Combine The type of the combiner, a binary functor.
/// \tparam OpList The type to represent a list of modify-operations.
///
/// The combiner is required to be associative, but not commutative.
///
/// <tt><b>OpList</b></tt> must be copy assignable and default constructible;
/// the default constructor must create an empty list. Additionaly, \c OpList
/// must define the following member functions:
///
/// <tt><b>void push(const OpList& ops)</b></tt>:
/// Appends the operations enqueued in <tt>ops</tt> to <tt>*this</tt>.
///
/// <tt><b>T apply(size_t range_size, const T& reduced_value)</b></tt>:
/// Returns the result of applying the enqueued operations in \c *this to an
/// arbitrary range of size <tt>range_size</tt> whose reduced value after
/// applying the combiner is <tt>reduced_value</tt>.
///
/// <tt><b>bool empty()</b></tt>:
/// Returns \c true if the queue is empty. It guarantees that
/// <tt>apply(rsize, x)</tt> has no effect and returns <tt>x</tt>.
///
template <typename T, typename Combine, typename OpList>
class lazyprop_segtree {
  struct node_t {
    node_t(size_t b, size_t e, size_t pos_ = 1) : pos{pos_}, beg{b}, end{e} {}

    size_t pos, beg, end;

    size_t size() const { return end - beg; }
    size_t lpos() const { return 2 * pos; }
    size_t rpos() const { return 2 * pos + 1; }
    node_t left() const { return node_t(beg, beg + size() / 2, lpos()); }
    node_t right() const { return node_t(beg + size() / 2, end, rpos()); }
    bool leaf() const { return size() == 1; }
  };

  node_t root() const { return node_t(0, num_elems); }

public:
  /// \brief Constructs a segment tree initialized with the identity value.
  ///
  /// \param count The number of elements to be stored.
  /// \param identity The identity value for <tt>comb</tt>.
  /// \param comb The combiner to use.
  ///
  /// \pre <tt>identity</tt> must satisfy the identity property for
  /// <tt>comb</tt>. It means that <tt>comb(x, identity) = comb(identity, x) =
  /// x</tt> will be true for any valid <tt>x</tt>.
  ///
  /// \par Complexity
  /// Linear in <tt>count</tt>.
  ///
  lazyprop_segtree(size_t count, const T &identity,
                   const Combine &comb = Combine())
      : num_elems{count}, combine(comb) {
    node_t nd = root(); // Node to find optimal size.
    while (!nd.leaf())
      nd = nd.right();
    values.resize(nd.pos + 1, identity);
    ops.resize(nd.pos + 1); // size is 4*(N-1) in the worst case
  }

  /// \brief Constructs a segment tree initialized with the given range.
  ///
  /// \param first The beginning of the range to copy the elements from.
  /// \param last The end of the range to copy the elements from.
  /// \param comb The combiner to use.
  ///
  /// \par Complexity
  /// <tt>O(n)</tt> applications of <tt>comb</tt>, where
  /// <tt>n = std::distance(first, last)</tt>.
  ///
  template <typename ForwardIt>
  lazyprop_segtree(ForwardIt first, ForwardIt last,
                   const Combine &comb = Combine())
      : lazyprop_segtree(std::distance(first, last), T(), comb) {
    assert(first != last);
    copy_range(first, root());
    assert(first == last);
  }

  /// \brief Returns the number of elements stored in the tree.
  ///
  /// \returns The number of elements stored in the tree.
  ///
  /// \par Complexity
  /// Constant.
  ///
  size_t size() const { return num_elems; }

  /// \brief Applies a list of operations to the specified segment.
  ///
  /// \param beg Position of the first element to include.
  /// \param end Position of the element after the last element to include.
  /// \param op The list of operations to be applied.
  ///
  /// \pre <tt>beg < end && end <= size()</tt>.
  ///
  /// \par Complexity
  /// <tt>O(log(n))</tt> applications of the stored combiner and
  /// <tt>O(log(n))</tt> applications of the stored OpLists,
  /// where <tt>n = size()</tt>.
  ///
  void apply(size_t beg, size_t end, const OpList &op) {
    assert(beg < end);
    query_beg = beg, query_end = end;
    apply(op, root());
  }

  /// \brief Reduces the specified segment to a single value by using the stored
  /// combiner.
  ///
  /// \param beg Position of the first element to include.
  /// \param end Position of the element after the last element to include.
  ///
  /// \pre <tt>beg < end && end <= size()</tt>.
  ///
  /// \returns The reduced value of the segment <tt>[beg, end)</tt>.
  ///
  /// \par Complexity
  /// <tt>O(log(n))</tt> applications of the stored combiner and
  /// <tt>O(log(n))</tt> applications of the stored OpLists,
  /// where <tt>n = size()</tt>.
  ///
  T reduce(size_t beg, size_t end) {
    assert(beg < end);
    query_beg = beg, query_end = end;
    return reduce(root());
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
    ops[nd.pos] = OpList(); // clear the queue
  }

  T reduce(const node_t &nd) {
    push_down_ops(nd);
    if (nd.beg >= query_beg && nd.end <= query_end)
      return values[nd.pos];
    const auto lhs = nd.left();
    const auto rhs = nd.right();
    if (query_end <= lhs.end)
      return reduce(lhs);
    if (query_beg >= rhs.beg)
      return reduce(rhs);
    return combine(reduce(lhs), reduce(rhs));
  }

  void apply(const OpList &op, const node_t &nd) {
    if (nd.beg >= query_beg && nd.end <= query_end) {
      ops[nd.pos].push(op);
      return;
    }
    push_down_ops(nd);
    const auto lhs = nd.left();
    const auto rhs = nd.right();
    if (lhs.end > query_beg)
      apply(op, lhs);
    if (rhs.beg < query_end)
      apply(op, rhs);
    push_down_ops(lhs);
    push_down_ops(rhs);
    values[nd.pos] = combine(values[lhs.pos], values[rhs.pos]);
  }

  // This function is optional.
  template <typename ForwardIt>
  void copy_range(ForwardIt &iter, const node_t &nd) {
    if (nd.leaf()) {
      values[nd.pos] = *iter++;
      return;
    }
    copy_range(iter, nd.left());
    copy_range(iter, nd.right());
    values[nd.pos] = combine(values[nd.lpos()], values[nd.rpos()]);
  }

private:
  // Data variables
  size_t num_elems;
  Combine combine;
  std::vector<T> values;
  std::vector<OpList> ops;
  // Cache variables
  size_t query_beg{};
  size_t query_end{};
};

} // end namespace djp

#endif // Header guard
