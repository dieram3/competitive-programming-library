//          Copyright Diego Ramírez June 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
/// \file
/// \brief Implements the Edmonds-Karp algorithm.

#ifndef DJP_GRAPH_EDMONS_KARP_MAX_FLOW_HPP
#define DJP_GRAPH_EDMONS_KARP_MAX_FLOW_HPP

#include <algorithm>   // For std::fill, std::min
#include <queue>       // For std::queue
#include <type_traits> // For std::is_signed and std::is_floating_point
#include <utility>     // For std::declval
#include <vector>      // For std::vector
#include <cstddef>     // For std::size_t

namespace djp {

/// Solves the maximum flow problem using the Edmons-Karp algorithm.
///
/// The \c edge_data of \c Graph must have the following fields:
/// \li \c capacity: The max flow allowed through the edge.
/// \li \c flow: Output field where the final flow of each edge will be put on.
/// \li \c rev_edge: A pointer to the reversed edge. Note that if the reverse
/// edge was not intended to be part of the modeled graph, it should have a
/// capacity of 0.
///
/// \param g The graph (E, V) that represents the flow network.
/// \param source The source vertex.
/// \param target The target vertex.
/// \returns The maximum possible flow from \p source to \p target.
/// \pre \p g shall not have parallel edges.
/// \pre \p source shall not be equal to \p target.
/// \par Complexity
/// O(V * E^2)
template <typename Graph, typename FlowType = decltype(
                              std::declval<typename Graph::edge>().flow)>
FlowType edmonds_karp_max_flow(Graph &g, size_t source, size_t target) {
  static_assert(std::is_signed<FlowType>::value ||
                    std::is_floating_point<FlowType>::value,
                "The flow type must be signed or floating point.");

  for (auto &edge : g.edges())
    edge.flow = 0;

  std::vector<typename Graph::edge *> parent(g.num_vertices());

  auto find_path = [source, target, &parent, &g] {
    std::queue<size_t> queue;
    queue.push(source);
    std::fill(begin(parent), end(parent), nullptr);

    while (!queue.empty()) {
      const size_t u = queue.front();
      queue.pop();

      for (auto *edge : g.out_edges(u)) {
        const size_t v = edge->target;
        if (parent[v] || edge->flow >= edge->capacity)
          continue;

        parent[v] = edge;
        if (v == target)
          return true;
        queue.push(v);
      }
    }

    return false;
  };

  FlowType max_flow = 0;

  while (find_path()) {
    size_t curr = target;
    auto *edge = parent[curr];
    FlowType path_flow = edge->capacity - edge->flow;
    for (curr = edge->source; curr != source; curr = edge->source) {
      edge = parent[curr];
      path_flow = std::min(path_flow, edge->capacity - edge->flow);
    }
    for (curr = target; curr != source; curr = edge->source) {
      edge = parent[curr];
      edge->flow += path_flow;
      edge->rev_edge->flow -= path_flow;
    }
    max_flow += path_flow;
  }

  return max_flow;
}

} // end namespace djp

#endif // Header guard
