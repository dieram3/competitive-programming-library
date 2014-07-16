//          Copyright Diego Ramírez July 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DJP_CONTAINER_SEGMENT_TREE_HPP
#define DJP_CONTAINER_SEGMENT_TREE_HPP

#include <vector>
#include <iterator>

namespace djp {

template <class T, class BinaryOp>
class segment_tree {
 public:
  using value_type = T;
  using size_type = std::size_t;
  using operator_type = BinaryOp;

 private:
  using tree_type = std::vector<value_type>;
  using node_idx = typename tree_type::size_type;  // node index

 public:
  template <class ForwardIt>
  segment_tree(ForwardIt first, ForwardIt last,
               operator_type Oper = operator_type())
      : RangeSize(std::distance(first, last)), Op(Oper) {
    if (size()) {
      Tree.resize(tree_size(size()));
      initialize(root(), first, last);
    }
  }

  value_type accumulate(size_type Beg, size_type End) const {
    return accumulate(Beg, End, root(), 0, size());
  }

  size_type size() const { return RangeSize; }

  template <class UnaryFunc>
  void for_each(size_type Beg, size_type End, UnaryFunc f) {
    for_each(Beg, End, root(), 0, size(), f);
  }

 private:
  template <class ForwardIt>
  void initialize(node_idx idx, ForwardIt first, ForwardIt last);

  value_type accumulate(size_type TargetBeg, size_type TargetEnd,
                        node_idx CurIdx, size_type CurBeg,
                        size_type CurEnd) const;

  template <class UnaryFunc>
  void for_each(size_type TargetBeg, size_type TargetEnd, node_idx CurIdx,
                size_type CurBeg, size_type CurEnd, UnaryFunc& f);

  // Requires: Size != 0 ,  has complexity O(log(Size))
  static size_type tree_size(size_type Size) {
    node_idx idx = root();
    size_type Beg = 0, End = Size;

    while (Beg + 1 != End) {
      Beg = middle_bound(Beg, End);
      idx = right_child(idx);
    }
    return idx + 1;
    // idx will be the last index.
    // the returned value (idx+1) has the form (2^n -1),
    // e.g (1,3,7,15,31,63,127...) if root() == 0
  }

  static node_idx root() {
    return 0;
  }  // We don't need worry up if this is 0 or 1.

  static node_idx left_child(node_idx idx) { return 2 * idx + 1; }

  static node_idx right_child(node_idx idx) { return 2 * idx + 2; }

  static size_type middle_size(size_type sz) { return sz / 2; }

  static size_type middle_bound(size_type Beg, size_type End) {
    return Beg + middle_size(End - Beg);
  }

 private:  // Private data members
  size_type RangeSize;
  operator_type Op;
  tree_type Tree;
};

template <class T, class BinaryOp>
template <class ForwardIt>
void segment_tree<T, BinaryOp>::initialize(node_idx idx, ForwardIt first,
                                           ForwardIt last) {
  size_type SegmentSize = std::distance(first, last);

  if (SegmentSize == 1) {
    Tree[idx] = *first;
    return;
  }

  ForwardIt middle = std::next(first, middle_size(SegmentSize));
  node_idx lchild = left_child(idx);
  node_idx rchild = right_child(idx);

  initialize(lchild, first, middle);
  initialize(rchild, middle, last);
  Tree[idx] = Op(Tree[lchild], Tree[rchild]);
}

template <class T, class BinaryOp>
typename segment_tree<T, BinaryOp>::value_type
segment_tree<T, BinaryOp>::accumulate(size_type TargetBeg, size_type TargetEnd,
                                      node_idx CurIdx, size_type CurBeg,
                                      size_type CurEnd) const {
  if (TargetBeg == CurBeg and TargetEnd == CurEnd) {
    return Tree[CurIdx];
  }
  // else it can't be a leaf.

  size_type CurMiddle = middle_bound(CurBeg, CurEnd);

  if (TargetEnd <= CurMiddle) {
    // Search left child
    return accumulate(TargetBeg, TargetEnd, left_child(CurIdx), CurBeg,
                      CurMiddle);
  }
  if (TargetBeg >= CurMiddle) {
    // Search right child
    return accumulate(TargetBeg, TargetEnd, right_child(CurIdx), CurMiddle,
                      CurEnd);
  }

  // else search both of them

  value_type ResL =
      accumulate(TargetBeg, CurMiddle, left_child(CurIdx), CurBeg, CurMiddle);

  value_type ResR =
      accumulate(CurMiddle, TargetEnd, right_child(CurIdx), CurMiddle, CurEnd);

  return Op(ResL, ResR);
}

template <class T, class BinaryOp>
template <class UnaryFunc>
void segment_tree<T, BinaryOp>::for_each(size_type TargetBeg,
                                         size_type TargetEnd, node_idx CurIdx,
                                         size_type CurBeg, size_type CurEnd,
                                         UnaryFunc& f) {
  if (CurBeg + 1 == CurEnd)
    f(Tree[CurIdx]);
  else {
    size_type CurMiddle = middle_bound(CurBeg, CurEnd);
    node_idx lchild = left_child(CurIdx);
    node_idx rchild = right_child(CurIdx);

    if (TargetEnd <= CurMiddle) {
      for_each(TargetBeg, TargetEnd, lchild, CurBeg, CurMiddle, f);
    } else if (TargetBeg >= CurMiddle) {
      for_each(TargetBeg, TargetEnd, rchild, CurMiddle, CurEnd, f);
    } else {
      for_each(TargetBeg, CurMiddle, lchild, CurBeg, CurMiddle, f);
      for_each(CurMiddle, TargetEnd, rchild, CurMiddle, CurEnd, f);
    }

    Tree[CurIdx] = Op(Tree[lchild], Tree[rchild]);
  }
}

}  // namespace djp

#endif  // Header guard
