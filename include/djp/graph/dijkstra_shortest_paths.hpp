//          Copyright Diego Ramírez July 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
/// \file
/// \brief Implements Dijkstra's algorithm.

#ifndef DJP_GRAPH_DIJKSTRA_SHORTEST_PATHS_HPP
#define DJP_GRAPH_DIJKSTRA_SHORTEST_PATHS_HPP

#include <vector>  // for std::vector
#include <queue>   // for std::priority_queue
#include <limits>  // for std::numeric_limits
#include <cstddef> // for std::size_t

namespace djp {

/// \brief Solves the single-source shortest paths problem.
///
/// \tparam Graph Directed graph type.
/// \tparam Distance Distance type.
///
/// \param g The target graph.
/// \param source The source vertex.
/// \param weight Weight map of edges.
///
/// \returns A vector \c dist such that  <tt>dist[v]</tt> contains the shortest
/// distance from the vertex \p source to the vertex \c v. Note that
/// <tt>dist[source] == 0</tt>.
///
/// \par Complexity
/// O(V * log(V) + E), where <tt>V = g.num_vertices()</tt> and
/// <tt>E = g.num_edges()</tt>
///
template <class Graph, class Distance>
std::vector<Distance>
dijkstra_shortest_paths(const Graph &g, size_t source,
                        const std::vector<Distance> &weight) {

  struct pq_elem {
    Distance dist;
    size_t vertex;
    bool operator<(const pq_elem &that) const { return dist > that.dist; }
  };

  std::vector<bool> visited(g.num_vertices(), false);
  std::vector<Distance> dist(g.num_vertices(),
                             std::numeric_limits<Distance>::max());

  std::priority_queue<pq_elem> pq;
  pq.push({dist[source] = 0, source});

  while (!pq.empty()) {
    const size_t u = pq.top().vertex;
    pq.pop();
    if (visited[u])
      continue;

    visited[u] = true;
    for (const auto edge : g.out_edges(u)) {
      const size_t v = g.target(edge);
      const Distance alt = dist[u] + weight[edge]; // alternative
      if (alt < dist[v])
        pq.push({dist[v] = alt, v});
    }
  }
  return dist;
}

} // namespace djp

#endif // Header guard
