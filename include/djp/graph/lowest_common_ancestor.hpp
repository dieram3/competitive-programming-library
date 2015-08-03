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

#include <cassert> // for assert
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
  /// \brief Constructs a rmq_lca object with the given tree in relation to the
  /// given root.
  ///
  /// \param graph The target undirected graph.
  /// \param root Vertex descriptor of the root.
  ///
  /// \pre \p graph must be connected and have no cycles i.e it must be a tree.
  /// \par Complexity
  /// <tt>O(N)</tt>, where <tt>N = graph.num_vertices()</tt>
  ///
  template <typename Graph>
  rmq_lca(const Graph &graph, const size_t root) {
    const size_t num_vertices = graph.num_vertices();

    euler_pos.resize(num_vertices);
    std::vector<bool> visited(num_vertices);
    std::vector<size_t> depth_of(num_vertices);

    std::stack<size_t> dfs_stack;
    std::vector<euler_visit> euler_tour;
    euler_tour.reserve(2 * num_vertices);

    depth_of[root] = 0;
    dfs_stack.push(root);

    while (!dfs_stack.empty()) {
      const size_t current = dfs_stack.top();
      dfs_stack.pop();

      euler_tour.push_back({current, depth_of[current]});

      if (visited[current])
        continue;

      for (const auto *edge : graph.out_edges(current)) {
        const size_t child = edge->get_neighbor(current);
        if (visited[child])
          continue; // It is the parent.
        depth_of[child] = depth_of[current] + 1;
        dfs_stack.push(current);
        dfs_stack.push(child);
      }

      euler_pos[current] = euler_tour.size() - 1;
      visited[current] = true;
    }

    // Initialize the Segment Tree for Range Minimum Query.
    stree.assign(euler_tour.begin(), euler_tour.end());
  }

  /// \brief Computes the lowest common ancestor between two vertices.
  /// \param v1 Descriptor of the first vertex.
  /// \param v2 Descriptor of the second vertex.
  /// \par Complexity
  /// <tt>O(log(N))</tt>, where \c N = number of vertices on the target graph.
  ///
  size_t operator()(const size_t v1, const size_t v2) const {
    const auto range = std::minmax(euler_pos[v1], euler_pos[v2]);
    return stree.accumulate(range.first, range.second + 1).node;
  }

  /// \brief Returns the stored depth (related to the used root) of the given
  /// vertex.
  /// \param v Descriptor of the vertex to get the depth of.
  /// \par Complexity
  /// Constant.
  ///
  size_t depth_of(const size_t v) const {
    return stree.elem(euler_pos[v]).depth;
  }

private:
  segment_tree<euler_visit, shallower> stree;
  std::vector<size_t> euler_pos; // Any valid euler pos of node i.
};

} // namespace djp

#endif // Header guard
