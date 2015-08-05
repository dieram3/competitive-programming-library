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
/// \li \c rev_edge: A pointer to the reverse edge. Note that a reverse edge
/// must exist even though it was not intended to be part of the modeled graph.
/// In such a case, the reverse edge should have a capacity of 0. In case of
/// existence of parallel edges, each edge <tt>(u, v)</tt> must have its own
/// counterpart <tt>(v, u)</tt>.
///
/// \tparam Graph Must be a valid graph type.
/// \tparam FlowType The type of the flow. It shall be either a signed integer
/// or a floating point type.
/// \param graph The directed graph that represents the flow network.
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
FlowType edmonds_karp_max_flow(const Graph &graph, const size_t source,
                               const size_t target) {
  static_assert(std::is_signed<FlowType>::value ||
                    std::is_floating_point<FlowType>::value,
                "The flow type must be signed or floating point.");

  for (auto &edge : graph.edges())
    edge.flow = 0;

  // last_queue_ids[v] represents the the last BFS queue in which the vertex v
  // was inserted. Note that the source vertex is present in all BFS queues.
  std::vector<unsigned> last_queue_ids(graph.num_vertices());
  std::vector<const typename Graph::edge *> parents(graph.num_vertices());

  auto find_path = [source, target, &parents, &last_queue_ids, &graph] {
    std::queue<size_t> queue;
    queue.push(source);
    const auto current_queue_id = ++last_queue_ids[source];

    while (!queue.empty()) {
      const size_t curr = queue.front();
      queue.pop();
      for (auto *edge : graph.out_edges(curr)) {
        const size_t child = edge->target;
        if (last_queue_ids[child] == current_queue_id)
          continue;
        if (edge->flow >= edge->capacity)
          continue;
        parents[child] = edge;
        if (child == target)
          return true;
        queue.push(child);
        last_queue_ids[child] = current_queue_id;
      }
    }
    return false;
  };

  FlowType max_flow = 0;

  while (find_path()) {
    FlowType path_flow = std::numeric_limits<FlowType>::max();
    for (auto *edge = parents[target]; edge; edge = parents[edge->source]) {
      path_flow = std::min(path_flow, edge->capacity - edge->flow);
    }
    for (auto *edge = parents[target]; edge; edge = parents[edge->source]) {
      edge->flow += path_flow;
      edge->rev_edge->flow -= path_flow;
    }
    max_flow += path_flow;
  }

  return max_flow;
}

/// \brief Convenience function to add an edge correctly in a graph so it is
/// usable in the Edmonds-Karp algorithm.
template <typename Graph, typename Flow>
void edmonds_karp_add_edge(Graph &graph, size_t s, size_t t, Flow cap,
                           Flow rev_cap = 0) {
  auto &edge1 = graph.add_edge(s, t);
  auto &edge2 = graph.add_edge(t, s);
  edge1.capacity = cap;
  edge2.capacity = rev_cap;
  edge1.rev_edge = &edge2;
  edge2.rev_edge = &edge1;
}

} // end namespace djp

#endif // Header guard
