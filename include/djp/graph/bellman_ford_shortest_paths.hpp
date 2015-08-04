//          Copyright Diego Ramírez July 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DJP_GRAPH_BELLMAN_FORD_SHORTEST_PATHS_HPP
#define DJP_GRAPH_BELLMAN_FORD_SHORTEST_PATHS_HPP

#include <limits>  // for std::numeric_limits
#include <vector>  // for std::vector
#include <cstddef> // for std::size_t

namespace djp {

/// \brief Solves the single-source shortest paths problem for a digraph with
/// both positive and negative edge weights.
///
/// Distances to each vertex from \p source are stored on \p dist \c vector.
/// Distances for unreachable vertices are set to
/// <tt>std::numeric_limits<DistT>::max()</tt>.
///
/// This algorithm has the ability to detect negative weight cycles reachable
/// from \p source, in which case, the distance to some vertices would be
/// <tt>-infinity</tt>.
///
/// If the graph have no negative edge weight prefer using The Dijkstra
/// Algorithm for efficiency. If further, all edges have same weight prefer
/// using The BFS Shortest Reach Algorithm since it has linear complexity.
///
/// \param graph The target graph.
/// \param source The source vertex.
/// \param[out] dist The sequence in which the distance from \p source to each
/// other vertex will be stored.
///
/// \returns \c true if all distances were minimized i.e if \p graph had no
/// negative cycle. Otherwise returns \c false.
///
/// \par Complexity
/// At most <tt>O(V * E)</tt> memory accesses, where
/// <tt>V = graph.num_vertices()</tt> and <tt>E = graph.num_edges()</tt>.
///
template <typename EdgeListGraph, typename DistT>
bool bellman_ford_shortest_paths(const EdgeListGraph &graph,
                                 const size_t source,
                                 std::vector<DistT> &dist) {
  const auto infinity = std::numeric_limits<DistT>::max();
  dist.assign(graph.num_vertices(), infinity);
  dist[source] = 0;

  for (size_t step = 0; step != dist.size(); ++step) {
    bool updated = false;

    for (const auto &edge : graph.edges()) {
      if (dist[edge.source] == infinity)
        continue;
      if (dist[edge.source] + edge.weight >= dist[edge.target])
        continue;
      // pred[edge.target] = edge.source;
      dist[edge.target] = dist[edge.source] + edge.weight;
      updated = true;
    }
    if (!updated)
      return true;
  }
  return false;
}

} // end namespace djp

#endif // Header guard
