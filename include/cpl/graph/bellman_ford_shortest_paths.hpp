//          Copyright Diego Ramirez 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef CPL_GRAPH_BELLMAN_FORD_SHORTEST_PATHS_HPP
#define CPL_GRAPH_BELLMAN_FORD_SHORTEST_PATHS_HPP

#include <cstddef> // size_t
#include <limits>  // numeric_limits
#include <vector>  // vector

namespace cpl {

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
/// \param g The target graph.
/// \param source The source vertex.
/// \param weight Weight map of edges.
/// \param[out] dist The sequence in which the distance from \p source to each
/// other vertex will be stored.
///
/// \returns \c true if all distances were minimized i.e if \p graph had no
/// negative cycle reachable from \p source. Otherwise returns \c false.
///
/// \par Complexity
/// At most <tt>O(V * E)</tt> memory accesses.
///
template <typename Graph, typename Distance>
bool bellman_ford_shortest_paths(const Graph& g, const size_t source,
                                 const std::vector<Distance>& weight,
                                 std::vector<Distance>& dist) {
  const auto infinity = std::numeric_limits<Distance>::max();
  const size_t num_edges = g.num_edges();

  dist.assign(g.num_vertices(), infinity);
  dist[source] = 0;

  for (size_t step = 0; step != dist.size(); ++step) {
    bool updated = false;

    for (size_t e = 0; e != num_edges; ++e) {
      if (dist[g.source(e)] == infinity)
        continue;
      if (dist[g.source(e)] + weight[e] >= dist[g.target(e)])
        continue;
      // pred[g.target(e)] = g.source(e);
      dist[g.target(e)] = dist[g.source(e)] + weight[e];
      updated = true;
    }
    if (!updated)
      return true;
  }
  return false;
}

} // end namespace cpl

#endif // Header guard
