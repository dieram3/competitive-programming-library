//          Copyright Diego Ramirez 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef CPL_DATA_STRUCTURE_FENWICK_TREE_HPP
#define CPL_DATA_STRUCTURE_FENWICK_TREE_HPP

#include <cstddef> // size_t
#include <vector>  // vector

namespace cpl {

template <class T>
class fenwick_tree {
public:
  using size_type = std::size_t;

  // Constructs the fenwick tree with sz elements initialized as 0.
  // Complexity: O(N) where N == sz
  explicit fenwick_tree(size_type sz) : tree_(sz) {}

  // Increases the element at position 'i' by 'delta'.
  // Complexity: O(log(N)) where N == size()
  void increase(size_type i, const T& delta) {
    if (!delta)
      return;
    for (; i < size(); i |= i + 1)
      tree_[i] += delta;
  }

  // Returns the sum of elements positioned in the range [left,right]
  // Complexity: O(log(N)) where N == size()
  T sum(size_type left, size_type right) const {
    return sum(right) - sum(left - 1);
  }

  // Returns the sum of the elements positioned at [0, ind]
  T sum(size_type ind) const {
    T sum = 0;
    while ((~ind) != 0) {
      sum += tree_[ind];
      ind &= ind + 1;
      --ind;
    }
    return sum;
  }

  // Returns the number of elements stored in the tree.
  // Complexity: Constant
  size_type size() const {
    return tree_.size();
  }

private:
  std::vector<T> tree_;
};

template <class T>
class prefix_adder {
public:
  using size_type = std::size_t;

  explicit prefix_adder(size_type sz) : ftree_(sz), elems_(sz) {}

  void increase(size_type i, const T& delta) {
    elems_[i] += delta;
    ftree_.increase(i, delta);
  }

  void replace(size_type i, const T& value) {
    increase(i, value - elems_[i]);
  }

  T sum(size_type l, size_type r) const {
    return ftree_.sum(l, r);
  }
  T sum(size_type ind) const {
    return ftree_.sum(ind);
  }

  size_type size() const {
    return elems_.size();
  }
  const T& at(size_type i) const {
    return elems_[i];
  }

private:
  fenwick_tree<T> ftree_;
  std::vector<T> elems_;
};

} // end namespace cpl

#endif // Header guard
