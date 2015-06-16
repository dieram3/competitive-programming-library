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
#include <utility>     // For std::declval
#include <vector>      // For std::vector
#include <cstddef>     // For std::size_t

namespace djp {

/// Solves the maximum flow problem using the Edmonds-Karp algorithm.
///
/// The member type \c edge of \c Graph must contain the following fields:
/// \li \c capacity: The maximum flow allowed through the edge.
/// \li \c flow: Output field where the final flow of each edge will be put on.
/// This field should be mutable.
/// \li \c rev_edge: A pointer to the reversed edge. Note that if the reverse
/// edge was not intended to be part of the modeled graph, it should have a
/// capacity of 0.
///
/// \tparam Graph Must be a valid graph type.
/// \tparam FlowType The type of the flow. It shall be either a signed integer
/// or a floating point type.
/// \param graph The graph that represents the flow network.
/// \param source The source vertex.
/// \param target The target vertex.
/// \returns The maximum possible flow from \p source to \p target.
/// \pre \p source shall not be equal to \p target.
/// \par Complexity
/// At most O(V * E^2) memory accesses, where <tt>V = graph.num_vertices()</tt>
/// and <tt>E = graph.num_edges()</tt>.
/// \todo Add example.
template <typename Graph, typename FlowType = decltype(
                              std::declval<typename Graph::edge>().flow)>
FlowType edmonds_karp_max_flow(Graph &graph, size_t source, size_t target) {
  static_assert(std::is_signed<FlowType>::value ||
                    std::is_floating_point<FlowType>::value,
                "The flow type must be signed or floating point.");

  for (auto &edge : graph.edges())
    edge.flow = 0;

  std::vector<typename Graph::edge *> parent(graph.num_vertices());
  std::vector<unsigned> last_queue(graph.num_vertices());

  auto find_path = [source, target, &parent, &last_queue, &graph] {
    std::queue<size_t> queue;
    queue.push(source);
    const auto current_queue = ++last_queue[source];

    while (!queue.empty()) {
      const size_t curr = queue.front();
      queue.pop();
      for (auto *edge : graph.out_edges(curr)) {
        const size_t child = edge->target;
        if (last_queue[child] == current_queue)
          continue;
        if (edge->flow >= edge->capacity)
          continue;
        parent[child] = edge;
        if (child == target)
          return true;
        queue.push(child);
        last_queue[child] = current_queue;
      }
    }
    return false;
  };

  FlowType max_flow = 0;

  while (find_path()) {
    FlowType path_flow = std::numeric_limits<FlowType>::max();
    for (auto *edge = parent[target]; edge; edge = parent[edge->source]) {
      path_flow = std::min(path_flow, edge->capacity - edge->flow);
    }
    for (auto *edge = parent[target]; edge; edge = parent[edge->source]) {
      edge->flow += path_flow;
      edge->rev_edge->flow -= path_flow;
    }
    max_flow += path_flow;
  }

  return max_flow;
}

} // end namespace djp

#endif // Header guard
