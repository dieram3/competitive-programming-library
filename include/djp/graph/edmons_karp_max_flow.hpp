//          Copyright Diego Ramírez June 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
/// \file
/// \brief Implements the Edmonds-Karp algorithm.

#ifndef DJP_GRAPH_EDMONS_KARP_MAX_FLOW_HPP
#define DJP_GRAPH_EDMONS_KARP_MAX_FLOW_HPP

#include <algorithm>   // For std::min
#include <limits>      // For std::numeric_limits
#include <queue>       // For std::queue
#include <type_traits> // For std::is_signed and std::is_floating_point
#include <vector>      // For std::vector
#include <cstddef>     // For std::size_t

namespace djp {

/// \brief Solves the maximum flow problem using the Edmons-Karp algorithm.
/// \param g The graph (E, V) that represents the flow network.
/// \param source The source vertex.
/// \param target The target vertex.
/// \returns The maximum possible flow from \p source to \p target.
/// \pre \p g shall not have parallel edges.
/// \pre \p source shall not be equal to \p target.
/// \pre For each edge <em>(u, v)</em> in \c g.edges() must exist an edge
/// <em>(v, u)</em>. Note that if no edge <em>(v, u)</em> was intended, it
/// should have a capacity of 0.
/// \par Complexity
/// O(V * E^2)
/// \todo Make this function more user-friendly.
/// \todo Try not to allocate a matrix for the residual graph.
/// \todo Provide a way to return the flow of each edge. A possible solution
/// is requiring a 'flow' data member for edges. It also would solve the matrix
/// allocation problem.
template <class Graph>
auto edmonds_karp_max_flow(const Graph &g, size_t source, size_t target)
    -> decltype(g.edges().begin()->capacity) {
  using flow_type = decltype(g.edges().begin()->capacity);
  static_assert(std::is_signed<flow_type>::value ||
                    std::is_floating_point<flow_type>::value,
                "The flow type must be signed or floating point.");

  const size_t num_nodes = g.num_vertices();
  auto at = [num_nodes](size_t i, size_t j) { return i * num_nodes + j; };
  std::vector<flow_type> residual(num_nodes * num_nodes);

  for (auto &edge : g.edges())
    residual[at(edge.source, edge.target)] = edge.capacity;

  std::vector<size_t> parent(num_nodes);
  auto find_path = [at, num_nodes, source, target, &parent, &g, &residual] {
    std::vector<bool> visited(num_nodes);
    std::queue<size_t> queue;
    queue.push(source);
    visited[source] = true;
    parent[source] = g.null_vertex();

    while (!queue.empty()) {
      const size_t u = queue.front();
      queue.pop();

      if (u == target)
        return true;

      for (auto edge : g.out_edges(u)) {
        const size_t v = edge->target;
        if (visited[v] || residual[at(u, v)] <= 0)
          continue;
        queue.push(v);
        visited[v] = true;
        parent[v] = u;
      }
    }

    return false;
  };

  flow_type max_flow = 0;

  while (find_path()) {
    auto path_flow = std::numeric_limits<flow_type>::max();
    for (size_t v = target; v != source; v = parent[v]) {
      size_t u = parent[v];
      path_flow = std::min(path_flow, residual[at(u, v)]);
    }
    for (size_t v = target; v != source; v = parent[v]) {
      size_t u = parent[v];
      residual[at(u, v)] -= path_flow;
      residual[at(v, u)] += path_flow;
    }
    max_flow += path_flow;
  }

  return max_flow;
}

} // end namespace djp

#endif // Header guard
