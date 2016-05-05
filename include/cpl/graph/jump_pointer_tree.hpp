//          Copyright Diego Ramirez 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef CPL_GRAPH_JUMP_POINTER_TREE_HPP
#define CPL_GRAPH_JUMP_POINTER_TREE_HPP

#include <cassert> // assert
#include <cstddef> // size_t
#include <cstdint> // SIZE_MAX
#include <stack>   // stack
#include <vector>  // vector

namespace cpl {

/// \brief Data structure which solves the level ancestor problem.
///
/// It uses \c N*log(N) space and can find ancestors at the required distance in
/// \c log(N) time (where \c N is the number of vertices of the tree).
///
class jump_pointer_tree {
  std::vector<std::vector<size_t>> parents; // (2^i)th parent
  std::vector<size_t> depth;

private:
  static size_t parents_size(size_t depth) {
    size_t result = 0;
    while (depth > 0)
      ++result, depth >>= 1;
    return result;
  }
  void add_leaf(const size_t leaf, const size_t parent) {
    depth[leaf] = depth[parent] + 1;
    parents[leaf].resize(parents_size(depth[leaf]));
    parents[leaf][0] = parent;
    for (size_t i = 0; i + 1 < parents[leaf].size(); ++i)
      parents[leaf][i + 1] = parents[parents[leaf][i]][i];

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
  jump_pointer_tree(const Graph& g, const size_t root)
      : parents(g.num_vertices()), depth(parents.size(), SIZE_MAX) {
    depth[root] = 0;
    std::stack<size_t> stack;
    stack.push(root);
    while (!stack.empty()) {
      const size_t curr = stack.top();
      stack.pop();
      for (const size_t e : g.out_edges(curr)) {
        const size_t child = (curr == g.source(e) ? g.target(e) : g.source(e));
        if (depth[child] != SIZE_MAX)
          continue;
        add_leaf(child, curr);
        stack.push(child);
      }
    } // end while
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
  size_t depth_of(const size_t v) const {
    return depth[v];
  }

  /// \brief Finds the k-th ancestor of \p v according to the used root.
  ///
  /// \param v The vertex to be queried.
  /// \param k Distance of ancestor to be searched.
  ///
  /// \returns The k-th ancestor
  ///
  /// \pre <tt>k <= depth_of(v)</tt>
  ///
  /// \par Complexity
  /// Logarithmic in \c k.
  ///
  size_t kth_ancestor(size_t v, size_t k) const {
    for (size_t i = 0; k > 0; k >>= 1, ++i) {
      if ((k & 1) != 0)
        v = parents[v][i];
    }
    return v;
  }

  /// \brief Finds the ancestor of vertex \p v at depth \p d.
  ///
  /// \param v The vertex to be queried.
  /// \param d Depth of the ancestor to be searched.
  ///
  /// \return The ancestor in the required depth (or level).
  ///
  /// \pre <tt>d <= depth_of(v)</tt>
  ///
  /// \par Complexity
  /// Logarithmic in <tt>(depth_of(v) - d)</tt>.
  ///
  size_t level_ancestor(const size_t v, const size_t d) const {
    return kth_ancestor(v, depth[v] - d);
  }
};

} // end namespace cpl

#endif // Header guard
