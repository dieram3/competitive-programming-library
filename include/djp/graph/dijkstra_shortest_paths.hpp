//          Copyright Diego Ramírez July 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DJP_GRAPH_DIJKSTRA_SHORTEST_PATHS_HPP
#define DJP_GRAPH_DIJKSTRA_SHORTEST_PATHS_HPP

#include <vector>
#include <queue>

namespace djp {

// Yes, this interface is poor.
template <class Graph>
std::vector<std::size_t> dijkstra_shortest_paths(const Graph& g,
                                                 std::size_t source) {
  using std::size_t;
  using pq_value_t = std::pair<size_t, size_t>;  // (dist[v], v)
  using pq_type = std::priority_queue<void, std::vector<pq_value_t>,
                                      std::greater<pq_value_t>>;

  std::vector<size_t> dist(g.num_vertices(), -1);
  dist[source] = 0;

  pq_type pq;
  pq.emplace(dist[source], source);

  while (!pq.empty()) {
    size_t u = pq.top().second;
    pq.pop();

    // Iterate pairs (dest_vertex, cost)
    for (auto pair : g.out_edges(u)) {
      const auto v = pair.first;
      const auto weight_uv = pair.second;
      if (dist[v] > dist[u] + weight_uv) {
        dist[v] = dist[u] + weight_uv;
        pq.emplace(dist[v], v);
      }
    }
  }

  return dist;
}

}  // namespace djp

#endif
