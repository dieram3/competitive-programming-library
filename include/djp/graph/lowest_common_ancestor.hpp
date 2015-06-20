//          Copyright Diego Ramírez May 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
/// \file
/// \brief Defines classes and functions to find the lowest common ancestor.

#ifndef DJP_GRAPH_LOWER_COMMON_ANCESTOR_HPP
#define DJP_GRAPH_LOWER_COMMON_ANCESTOR_HPP

#include <djp/data_structure/segment_tree.hpp> // Used for Range Minimum Query

#include <algorithm> // for std::swap
#include <iterator>  // for std::begin, std::end
#include <stack>     // for std::stack
#include <vector>    // for std::vector

#include <cstddef> // for std::size_t

namespace djp {

/// \brief Function like class to compute the lowest common ancestor.
///
/// This class uses a range minimum query data structure to find the lowest
/// common ancestor between two vertices given a predefined root.
class rmq_lca {
  struct euler_visit {
    std::size_t node;
    std::size_t depth;
  };

  struct shallower {
    const euler_visit &operator()(const euler_visit &lhs,
                                  const euler_visit &rhs) const {
      return lhs.depth < rhs.depth ? lhs : rhs;
    }
  };

public:
  /// \brief Constructs a \c rmq_lca object.
  /// \param num_nodes Number of nodes of the target tree.
  /// \param root Vertex descriptor of the root.
  /// \param children_of Function that when invoked as <tt>children_of(v)</tt>
  /// being \c v a valid vertex descriptor, it must return an \c InputRange
  /// containing descriptors of all its children vertices.
  /// \pre The target graph shall have no cycles i.e it must be a tree.
  /// \pre The target graph shall be completely connected.
  /// \par Complexity
  /// <tt>O(N)</tt>, where <tt>N = num_nodes</tt>
  template <class ChildrenOf>
  rmq_lca(const size_t num_nodes, const size_t root, ChildrenOf children_of)
      : euler_pos_(num_nodes) {
    std::vector<bool> visited(num_nodes);
    std::vector<size_t> depths(num_nodes);

    std::stack<size_t> stack;
    std::vector<euler_visit> euler_tour;
    euler_tour.reserve(2 * num_nodes);

    depths[root] = 0;
    stack.push(root);

    while (!stack.empty()) {
      const size_t cur_node = stack.top();
      stack.pop();
      const size_t cur_depth = depths[cur_node];

      euler_pos_[cur_node] = euler_tour.size();
      euler_tour.push_back({cur_node, cur_depth});

      if (visited[cur_node])
        continue;

      for (size_t child : children_of(cur_node)) {
        depths[child] = cur_depth + 1;
        stack.push(cur_node);
        stack.push(child);
      }

      visited[cur_node] = true;
    }

    // Initialize the Range Minimum Query.
    euler_tour_.assign(euler_tour.begin(), euler_tour.end());
  }

  /// \brief Computes the lowest common ancestor between two vertices.
  /// \param v1 Descriptor of the first vertex.
  /// \param v2 Descriptor of the second vertex.
  /// \par Complexity
  /// <tt>O(log(N))</tt>, where \c N = number of vertices of the target
  /// graph.
  size_t operator()(size_t v1, size_t v2) const {
    size_t begin = euler_pos_[v1];
    size_t end = euler_pos_[v2];
    if (end < begin)
      std::swap(begin, end);
    ++end;
    return euler_tour_.accumulate(begin, end).node;
  }

private:
  segment_tree<euler_visit, shallower> euler_tour_;
  std::vector<size_t> euler_pos_; // any euler pos of node i in euler tour.
};

} // namespace djp

#endif // Header guard
