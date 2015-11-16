//          Copyright Diego Ramírez November 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DJP_DATA_STRUCTURE_EQSM_SEGTREE_HPP
#define DJP_DATA_STRUCTURE_EQSM_SEGTREE_HPP

#include <algorithm> // For std::copy
#include <iterator>  // For std::distance
#include <vector>    // For std::vector
#include <cstddef>   // For std::size_t

namespace djp {

/// \brief Segment tree with element query and segment modification.
///
/// The combiner is required to be commutative.
///
template <typename T, typename Combine>
class eqsm_segtree {
public:
  /// \brief Constructs a segment tree with the given size.
  ///
  /// The elements of the segment tree are initialized to the identity value.
  ///
  /// \param count The number of elements to store.
  /// \param idem The identity value for the combiner.
  /// \param comb The combiner to use.
  ///
  /// \pre <tt>comb(idem, x) = comb(x, idem) = x</tt> for all <tt>x</tt>.
  /// \pre <tt>comb(x,y) = comb(y,x)</tt> for all <tt>x,y</tt>.
  ///
  /// \par Complexity
  /// Linear in <tt>count</tt>.
  ///
  explicit eqsm_segtree(size_t count, const T &idem, Combine comb = Combine())
      : combine(comb), identity(idem), num_elems{count},
        tree(2 * num_elems, identity) {}

  /// \brief Replaces the contents of the tree with the those in given range.
  ///
  /// \param first Iterator to the beginning of the range to store.
  /// \param last Iterator to the end of the range to store.
  ///
  /// \par Complexity
  /// Exactly <tt>(count - 1)</tt> applications of the combiner.
  ///
  template <typename ForwardIt>
  void store(ForwardIt first, ForwardIt last) {
    num_elems = std::distance(first, last);
    tree.assign(2 * num_elems, identity);
    std::copy(first, last, tree.begin() + num_elems);
  }

  /// \brief Combines each element of the specified segment with a given value.
  ///
  /// \param value The value to combine with.
  /// \param l Position of the first element in the segment.
  /// \param r Position of the element after the last element in the segment.
  ///
  /// \pre <tt>l <= r && r <= size()</tt>
  ///
  /// \par Complexity
  /// <tt>O(log(n))</tt> applications of the stored combiner, where
  /// <tt>n = size()</tt>.
  ///
  void combine_with(const T &value, size_t l, size_t r) {
    for (l += num_elems, r += num_elems; l < r; l >>= 1, r >>= 1) {
      if (l & 1)
        tree[l] = combine(tree[l], value), ++l;
      if (r & 1)
        --r, tree[r] = combine(tree[r], value);
    }
  }

  /// \brief Gets the value of the specified element.
  ///
  /// \param p Position of the element to get the value.
  ///
  /// \pre <tt>p < size()</tt>
  ///
  /// \par Complexity
  /// <tt>O(log(n))</tt> applications of the stored combiner, where
  /// <tt>n = size()</tt>.
  ///
  T get(size_t p) {
    T res = identity;
    for (p += num_elems; p > 0; p >>= 1)
      res = combine(res, tree[p]);
    return res;
  }

  /// \brief Returns the number of elements in the tree.
  ///
  /// \returns The number of stored elements.
  ///
  /// \par Complexity
  /// Constant.
  ///
  size_t size() const { return num_elems; }

private:
  Combine combine{};
  T identity{};
  size_t num_elems{};
  std::vector<T> tree{};
};

} // end namespace djp

#endif // Header guard
