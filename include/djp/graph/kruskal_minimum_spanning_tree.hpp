//          Copyright Diego Ramírez July 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DJP_GRAPH_KRUSKAL_MINIMUM_SPANNING_TREE_HPP
#define DJP_GRAPH_KRUSKAL_MINIMUM_SPANNING_TREE_HPP

#include <algorithm>                           // for std::sort
#include <numeric>                             // for std::iota
#include <vector>                              // for std::vector
#include <djp/data_structure/disjoint_set.hpp> // for djp::disjoint_set

namespace djp {

/// \brief Finds a minimum spanning tree (MST) in an undirected graph with
/// weighted edges.
///
/// \param g The target graph.
/// \param weight The weight map of edges.
///
/// \returns A <tt>std::vector</tt> containing the edge descriptors of the MST
/// sorted by weight.
///
/// \par Complexity
/// <tt>O(E * log(E))</tt>.
///
template <typename Graph, typename Weight>
std::vector<size_t>
kruskal_minimum_spanning_tree(const Graph &g,
                              const std::vector<Weight> &weight) {
  const size_t num_vertices = g.num_vertices();
  if (num_vertices == 0)
    return {};

  std::vector<size_t> edges(g.num_edges());
  std::iota(edges.begin(), edges.end(), size_t{0});

  std::sort(edges.begin(), edges.end(),
            [&](size_t lhs, size_t rhs) { return weight[lhs] < weight[rhs]; });

  disjoint_set dset(num_vertices);

  const size_t max_tree_edges = num_vertices - 1;
  std::vector<size_t> tree_edges;
  tree_edges.reserve(max_tree_edges);

  for (const auto e : edges) {
    if (dset.union_set(g.source(e), g.target(e))) {
      tree_edges.push_back(e);
      if (tree_edges.size() == max_tree_edges)
        break;
    }
  }

  return tree_edges;
}

} // end namespace djp

#endif // Header guard
