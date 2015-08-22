//          Copyright Diego Ramírez May 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
/// \file
/// \brief Defines classes and functions to find the lowest common ancestor.

#ifndef DJP_GRAPH_LOWER_COMMON_ANCESTOR_HPP
#define DJP_GRAPH_LOWER_COMMON_ANCESTOR_HPP

#include <djp/data_structure/segment_tree.hpp> // Used for Range Minimum Query

#include <algorithm> // for std::minmax
#include <stack>     // for std::stack
#include <vector>    // for std::vector
#include <cstddef>   // for std::size_t

namespace djp {

/// \brief Query class to compute the lowest common ancestor.
///
/// This class uses a range minimum query data structure to find the lowest
/// common ancestor between two vertices given a predefined root.
class rmq_lca {
  struct euler_visit {
    size_t node;
    size_t depth;
  };

  struct shallower {
    const euler_visit &operator()(const euler_visit &lhs,
                                  const euler_visit &rhs) const {
      return lhs.depth < rhs.depth ? lhs : rhs;
    }
  };

public:
  /// \brief Constructs a \c rmq_lca object with the given tree in relation to
  /// the given root.
  ///
  /// \param g The target undirected graph.
  /// \param root Vertex descriptor of the root.
  ///
  /// \pre \p g must be a tree i.e it must be connected and have no cycles.
  /// \par Complexity
  /// <tt>O(N)</tt>, where <tt>N = g.num_vertices()</tt>
  ///
  template <typename Graph>
  rmq_lca(const Graph &g, const size_t root) {
    const size_t num_vertices = g.num_vertices();

    euler_pos.resize(num_vertices);
    std::vector<bool> visited(num_vertices);
    std::vector<size_t> depth_of(num_vertices);

    std::stack<size_t> dfs_stack;
    std::vector<euler_visit> euler_tour;
    euler_tour.reserve(2 * num_vertices);

    depth_of[root] = 0;
    dfs_stack.push(root);

    while (!dfs_stack.empty()) {
      const size_t curr = dfs_stack.top();
      dfs_stack.pop();

      euler_tour.push_back({curr, depth_of[curr]});

      if (visited[curr])
        continue;

      for (const auto e : g.out_edges(curr)) {
        const size_t child = (curr == g.source(e)) ? g.target(e) : g.source(e);
        if (visited[child])
          continue; // It is the parent.
        depth_of[child] = depth_of[curr] + 1;
        dfs_stack.push(curr);
        dfs_stack.push(child);
      }

      euler_pos[curr] = euler_tour.size() - 1;
      visited[curr] = true;
    }

    // Initialize the Segment Tree for Range Minimum Query.
    stree.assign(euler_tour.begin(), euler_tour.end());
  }

  /// \brief Computes the lowest common ancestor between two vertices.
  /// \param v1 Descriptor of the first vertex.
  /// \param v2 Descriptor of the second vertex.
  /// \par Complexity
  /// <tt>O(log(N))</tt>, where \c N = number of vertices in the target graph.
  ///
  size_t lca(const size_t v1, const size_t v2) const {
    const auto range = std::minmax(euler_pos[v1], euler_pos[v2]);
    return stree.accumulate(range.first, range.second + 1).node;
  }

  /// \brief Returns the stored depth (related to the used root) of the given
  /// vertex.
  /// \param v Descriptor of the vertex to get the depth of.
  /// \par Complexity
  /// Constant.
  ///
  size_t depth_of(const size_t v) const { return stree.at(euler_pos[v]).depth; }

  /// \brief Computes the distance between the given pair of vertices using the
  /// stored data.
  ///
  /// The distance between two vertices \p v1 and \p v2 is defined as the number
  /// of edges in the path from \p v1 to \p v2.
  ///
  /// \param v1 Descriptor of the first vertex.
  /// \param v2 Descriptor of the second vertex.
  ///
  /// \par Complexity
  /// <tt>O(log(N))</tt>, where \c N = number of vertices in the target graph.
  ///
  size_t distance(const size_t v1, const size_t v2) const {
    return depth_of(v1) + depth_of(v2) - 2 * depth_of(lca(v1, v2));
  }

  /// \brief Checks whether a vertice is visited in a path.
  ///
  /// Relies on LCA properties to verify if the vertice \p m is visited in the
  /// path going from \p a to \p b (inclusive).
  ///
  /// \param a The start of the path.
  /// \param b The end of the path.
  /// \param m The vertice to be checked.
  ///
  /// \returns \c true if the vertice \p m is visited. Otherwise \c false.
  ///
  /// \par Complexity
  /// <tt>O(log(N))</tt>, where \c N = number of vertices in the target graph.
  ///
  bool visits(size_t a, size_t b, size_t m) const {
    const size_t lca_ab = lca(a, b), lca_am = lca(a, m), lca_bm = lca(b, m);
    return (lca_am == m && lca_ab == lca_bm) ||
           (lca_bm == m && lca_ab == lca_am);
  }

private:
  segment_tree<euler_visit, shallower> stree; // the RMQ
  std::vector<size_t> euler_pos;              // Any valid euler pos for node i.
};

} // namespace djp

#endif // Header guard
