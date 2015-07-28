//          Copyright Diego Ramírez July 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DJP_GRAPH_KRUSKAL_MINIMUM_SPANNING_TREE_HPP
#define DJP_GRAPH_KRUSKAL_MINIMUM_SPANNING_TREE_HPP

#include <algorithm>                           // for std::sort
#include <vector>                              // for std::vector
#include <djp/data_structure/disjoint_set.hpp> // for djp::disjoint_set

namespace djp {

/// \brief Finds a minimum spanning tree (MST) in an undirected graph with
/// weighted edges.
///
/// \param graph The target undirected graph.
///
/// \returns A <tt>std::vector</tt> containing the edge descriptors of the MST
/// sorted by weight.
///
template <typename Graph>
std::vector<const typename Graph::edge *>
kruskal_minimum_spanning_tree(const Graph &graph) {
  using edge_pointer = const typename Graph::edge *;

  if (graph.num_vertices() == 0)
    return {};

  std::vector<edge_pointer> sorted_edges;
  sorted_edges.reserve(graph.num_edges());

  for (const auto &edge : graph.edges())
    sorted_edges.push_back(&edge);

  std::sort(sorted_edges.begin(), sorted_edges.end(),
            [](edge_pointer lhs, edge_pointer rhs) {
              return lhs->weight < rhs->weight;
            });

  disjoint_set dset(graph.num_vertices());

  const size_t num_tree_edges = graph.num_vertices() - 1;
  std::vector<edge_pointer> tree_edges;
  tree_edges.reserve(num_tree_edges);

  for (edge_pointer edge : sorted_edges) {
    if (dset.make_union(edge->source, edge->target)) {
      tree_edges.push_back(edge);
      if (tree_edges.size() == num_tree_edges)
        break;
    }
  }

  return tree_edges;
}

} // end namespace djp

#endif // Header guard
