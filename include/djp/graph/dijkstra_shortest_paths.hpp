//          Copyright Diego Ramírez July 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DJP_GRAPH_DIJKSTRA_SHORTEST_PATHS_HPP
#define DJP_GRAPH_DIJKSTRA_SHORTEST_PATHS_HPP

#include <vector>
#include <queue>
#include <utility>
#include <functional>
#include <cstddef>

namespace djp {

template <class Graph>
std::vector<size_t> dijkstra_shortest_paths(const Graph& g, size_t source) {
  using pq_value_t = std::pair<size_t, size_t>;  // (dist[v], v)
  using pq_type = std::priority_queue<void, std::vector<pq_value_t>,
                                      std::greater<pq_value_t>>;

  std::vector<size_t> dist(g.num_vertices(), -1);
  dist[source] = 0;

  pq_type pq;
  pq.emplace(dist[source], source);

  while (!pq.empty()) {
    const size_t u = pq.top().second;
    pq.pop();

    for (auto edge : g.out_edges(u)) {
      const size_t v = edge->target;
      const size_t weight_uv = edge->weight;
      if (dist[v] > dist[u] + weight_uv) {
        dist[v] = dist[u] + weight_uv;
        pq.emplace(dist[v], v);
      }
    }
  }
  return dist;
}

}  // namespace djp

#endif  // Header guard
