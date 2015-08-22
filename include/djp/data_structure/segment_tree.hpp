//          Copyright Diego Ramírez October 2014, August 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DJP_DATA_STRUCTURE_SEGMENT_TREE_HPP
#define DJP_DATA_STRUCTURE_SEGMENT_TREE_HPP

#include <algorithm> // for std::copy
#include <vector>    // for std::vector
#include <cstddef>   // for std::size_t

namespace djp {

/// \brief Data structure which supports 'cumulative sum' queries and element
/// modification in logarithmic time.
///
/// The segment tree can compute the result of combine a sub-range of
/// consecutive elements with a commutative binary function in <tt>log(n)</tt>
/// time. Examples of combinators are sum, multiplication, max and min. This
/// data structure also allows to modify single elements in logarithmic time.
///
template <typename T, typename BinaryOp>
class segment_tree {
  size_t num_elems{};
  BinaryOp op{};
  std::vector<T> tree{};

public:
  /// \brief Replaces the contents of the segment tree with those in the given
  /// range.
  ///
  /// \param first The beginning of the range to use as replacement.
  /// \param last The end of the range to use as replacement.
  /// \param comb Binary function to use as combinator.
  ///
  /// \par Complexity
  /// Linear in <tt>std::distance(first, last)</tt>.
  ///
  template <typename ForwardIt>
  void assign(ForwardIt first, ForwardIt last, BinaryOp comb = BinaryOp()) {
    num_elems = std::distance(first, last);
    op = comb;
    tree.resize(2 * num_elems);
    if (!num_elems)
      return;
    std::copy(first, last, tree.begin() + num_elems);
    for (size_t i = num_elems - 1; i > 0; --i)
      tree[i] = op(tree[i << 1], tree[i << 1 | 1]);
  }

  /// \brief Modifies an element with the given value.
  ///
  /// \param pos Position of element to modify.
  /// \param new_value The value to use as replacement.
  ///
  /// \par Complexity
  /// Logarithmic in <tt>size()</tt>.
  ///
  void modify(size_t pos, const T &new_value) {
    tree[pos += num_elems] = new_value;
    for (; pos > 1; pos >>= 1)
      tree[pos >> 1] = op(tree[pos], tree[pos ^ 1]);
  }

  /// \brief Accumulates the elements in the given range.
  ///
  /// Uses the combinator to 'accumulate' the elements positioned from \p
  /// l (inclusive) to \p r (exclusive).
  ///
  /// \param l The start position (inclusive).
  /// \param r The end position (exclusive).
  ///
  /// \returns The result of accumulate elements in the range <tt>[l, r)</tt>.
  ///
  /// \par Complexity
  /// Logarithmic in <tt>size()</tt>.
  ///
  T accumulate(size_t l, size_t r) const {
    T ans = at(l++);
    for (l += num_elems, r += num_elems; l < r; l >>= 1, r >>= 1) {
      if (l & 1)
        ans = op(ans, tree[l++]);
      if (r & 1)
        ans = op(ans, tree[--r]);
    }
    return ans;
  }

  /// \brief Accesses the specified element.
  ///
  /// \param pos Position of the element to return.
  ///
  /// \returns Reference to the requested element.
  ///
  /// \par Complexity
  /// Constant.
  ///
  const T &at(size_t pos) const { return tree[num_elems + pos]; }

  /// \brief Returns the number of elements in the segment tree.
  ///
  /// \returns The total number of elements.
  ///
  /// \par Complexity
  /// Constant.
  ///
  size_t size() const { return num_elems; }
};

} // end namespace djp

#endif // header guard
