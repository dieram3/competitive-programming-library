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
#include <type_traits> // For std::is_signed, std::is_floating_point
#include <vector>      // For std::vector
#include <cstddef>     // For std::size_t

namespace djp {

/// \brief Solves the maximum flow problem using the Edmonds-Karp algorithm.
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
/// \tparam Graph The directed graph type. Must model a flow network.
///
/// \param graph The target graph.
/// \param source The source vertex.
/// \param target The target vertex.
///
/// \returns The maximum possible flow from \p source to \p target. The final
/// flow of each edge in the graph will be recorded in their flow field.
///
/// \pre <tt>source != target</tt>
///
/// \par Complexity
/// At most O(V * E^2) memory accesses, where <tt>V = graph.num_vertices()</tt>
/// and <tt>E = graph.num_edges()</tt>.
///
template <typename Graph>
auto edmonds_karp_max_flow(Graph &graph, const size_t source,
                           const size_t target)
    -> decltype(graph.edges().begin()->flow) {

  using flow_t = decltype(graph.edges().begin()->flow);
  static_assert(std::is_signed<flow_t>::value ||
                    std::is_floating_point<flow_t>::value,
                "The flow type must be signed or floating point.");

  for (auto &edge : graph.edges())
    edge.flow = 0;

  // last_bfs[v] stores the the last BFS tree that vertex v was part of.  Note
  // that the source vertex is present in all BFS trees as it is the root.
  std::vector<unsigned> last_bfs(graph.num_vertices());
  std::vector<const typename Graph::edge *> parents(graph.num_vertices());

  auto find_path = [source, target, &parents, &last_bfs, &graph] {
    std::queue<size_t> bfs_queue;
    bfs_queue.push(source);
    const auto current_bfs = ++last_bfs[source];

    while (!bfs_queue.empty()) {
      const size_t curr = bfs_queue.front();
      bfs_queue.pop();
      for (auto *edge : graph.out_edges(curr)) {
        const size_t child = edge->target;
        if (last_bfs[child] == current_bfs)
          continue; // Already in the tree.
        if (edge->flow == edge->capacity)
          continue; // Can't navigate through saturated edges.
        parents[child] = edge;
        if (child == target)
          return true;
        bfs_queue.push(child);
        last_bfs[child] = current_bfs;
      }
    }
    return false;
  };

  flow_t max_flow = 0;
  while (find_path()) {
    flow_t path_flow = std::numeric_limits<flow_t>::max();
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

} // end namespace djp

#endif // Header guard
