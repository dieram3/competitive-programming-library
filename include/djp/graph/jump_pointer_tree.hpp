//          Copyright Diego Ramírez August 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DJP_GRAPH_JUMP_POINTER_TREE_HPP
#define DJP_GRAPH_JUMP_POINTER_TREE_HPP

#include <functional> // For std::function
#include <vector>     // For std::vector
#include <cassert>    // For assert
#include <cstddef>    // For std::size_t
#include <cstdint>    // For SIZE_MAX

namespace djp {

/// \brief Data structure which solves the level ancestor problem.
///
/// It uses \c N*log(N) space and can find ancestors at the required distance in
/// \c log(N) time (where \c N is the number of vertices of the tree).
///
class jump_pointer_tree {
  std::vector<std::vector<size_t>> parents; // (2^i)th parent
  std::vector<size_t> depth;

private:
  void add_leaf(size_t leaf, size_t parent) {
    depth[leaf] = depth[parent] + 1;
    parents[leaf].push_back(parent);
    for (size_t i = 1; (size_t{1} << i) <= depth[leaf]; ++i) {
      const size_t new_parent = parents[parents[leaf][i - 1]][i - 1];
      parents[leaf].push_back(new_parent);
    }
    assert((size_t{1} << parents[leaf].size()) > depth[leaf]);
    assert((size_t{1} << (parents[leaf].size() - 1)) <= depth[leaf]);
  }

public:
  /// \brief Construct a jump-pointer tree with the given tree and root.
  ///
  /// \param g The target undirected graph.
  /// \param root The vertex to be used as root.
  ///
  /// \pre The graph \p g must be a tree, that is, must be a connected
  /// undirected graph with no cycles.
  ///
  /// \par Complexity
  /// Linearithmic in <tt>g.num_vertices()</tt>.
  ///
  template <typename Graph>
  jump_pointer_tree(const Graph &g, const size_t root)
      : parents(g.num_vertices()), depth(parents.size(), SIZE_MAX) {
    std::function<void(size_t)> dfs;
    dfs = [&](const size_t curr) {
      for (const size_t e : g.out_edges(curr)) {
        const size_t child = (curr == g.source(e) ? g.target(e) : g.source(e));
        if (depth[child] != SIZE_MAX)
          continue;
        add_leaf(child, curr);
        dfs(child);
      }
    };
    depth[root] = 0;
    dfs(root);
  }

  /// \brief Returns the depth of \p v according to the used root.
  ///
  /// \param v The vertex to be queried.
  ///
  /// \returns The depth of \p v.
  ///
  /// \par Complexity
  /// Constant.
  ///
  size_t depth_of(const size_t v) const { return depth[v]; }

  /// \brief Finds the kth-ancestor of \p v according to the used root.
  ///
  /// \param v The vertex to be queried.
  /// \param k Distance of ancestor to be searched.
  ///
  /// \returns The kth ancestor
  ///
  /// \pre <tt>k <= depth_of(v)</tt>
  ///
  /// \par Complexity
  /// Logarithmic in the number of vertices of the underlying tree.
  ///
  size_t kth_ancestor(size_t v, size_t k) const {
    for (size_t i = 0; k; k >>= 1, ++i)
      if (k & 1)
        v = parents[v][i];
    return v;
  }

  /// \brief Finds the level ancestor of a vertex, that is, the ancestor at
  /// a certain depth.
  ///
  /// \param v The vertex to be queried.
  /// \param level Depth of the ancestor to be searched.
  ///
  /// \return The ancestor in the required level (or depth).
  ///
  /// \pre <tt>level <= depth_of(v)</tt>
  ///
  /// \par Complexity
  /// Logarithmic in the number of vertices of the underlying tree.
  ///
  size_t level_ancestor(const size_t v, const size_t level) const {
    return kth_ancestor(v, depth[v] - level);
  }
};

} // end namespace djp

#endif // header guard
