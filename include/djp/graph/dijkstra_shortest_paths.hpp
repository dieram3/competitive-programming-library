//          Copyright Diego Ramírez July 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DJP_GRAPH_DIJKSTRA_SHORTEST_PATHS_HPP
#define DJP_GRAPH_DIJKSTRA_SHORTEST_PATHS_HPP

#include <vector>  // for std::vector
#include <queue>   // for std::priority_queue
#include <limits>  // for std::numeric_limits
#include <cstddef> // for std::size_t

namespace djp {

/// \brief Solves the Single-Source Shortest Paths problem.
/// \returns A vector containing the shortest distances to each vertex.
/// Complexity: O(V*log(V) + E)
/// where V == g.num_vertices() && E == g.num_edges()
template <class Graph, class Distance = std::size_t>
std::vector<Distance> dijkstra_shortest_paths(const Graph &g, size_t source) {

  using dist_t = Distance;
  struct pq_elem {
    dist_t dist;
    size_t vertex;
    bool operator<(const pq_elem &that) const { return dist > that.dist; }
  };
  constexpr dist_t inf = std::numeric_limits<dist_t>::max();

  std::vector<bool> visited(g.num_vertices(), false);
  std::vector<dist_t> dist(g.num_vertices(), inf);

  std::priority_queue<pq_elem> pq;
  pq.push({dist[source] = 0, source});

  while (!pq.empty()) {
    const size_t u = pq.top().vertex;
    pq.pop();
    if (visited[u])
      continue;

    visited[u] = true;
    for (auto edge : g.out_edges(u)) {
      const size_t v = edge->target;
      const dist_t alt = dist[u] + edge->weight; // alternative
      if (alt < dist[v])
        pq.push({dist[v] = alt, v});
    }
  }
  return dist;
}

} // namespace djp

#endif // Header guard
