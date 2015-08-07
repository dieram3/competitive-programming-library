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
#include <cstdint>     // for SIZE_MAX

namespace djp {

/// \brief Solves the maximum flow problem using the Edmonds-Karp algorithm.
///
/// This algorithm requires that G has a reversed edge for each edge in the
/// graph. A reverse edge must exist even though it was not intended to be part
/// of the modeled graph, in such a case, the reverse edge should have a
/// capacity of 0. In case of existence of parallel edges, each edge <tt>(u,
/// v)</tt> must have its own counterpart <tt>(v, u)</tt>.
///
/// \tparam Graph The directed graph type. Must model a FlowNetwork.
///
/// \param g The target graph.
/// \param source The source vertex.
/// \param target The target vertex.
/// \param rev_edge The reverse edge map.
/// \param capacity The capacity map
/// \param[out] flow The final flow of each edge. It is guaranteed that
/// <tt>flow[e] <= capacity[e]</tt> for each edge \c e in the graph.
///
/// \returns The maximum possible flow from \p source to \p target.
///
/// \pre <tt>source != target</tt>
///
/// \par Complexity
/// At most O(V * E^2) memory accesses.
///
template <typename Graph, typename Flow>
Flow edmonds_karp_max_flow(const Graph &g, const size_t source,
                           const size_t target,
                           const std::vector<size_t> &rev_edge,
                           const std::vector<Flow> &capacity,
                           std::vector<Flow> &flow) {

  static_assert(std::is_signed<Flow>::value ||
                    std::is_floating_point<Flow>::value,
                "The flow type must be signed or floating point.");

  using edge_id = size_t;
  flow.assign(g.num_edges(), 0);

  // last_bfs[v] stores the the last BFS tree that vertex v was part of.  Note
  // that the source vertex is present in all BFS trees as it is the root.
  std::vector<unsigned> last_bfs(g.num_vertices());
  std::vector<edge_id> parents(g.num_vertices(), SIZE_MAX);

  auto find_path = [&, source, target] {
    std::queue<size_t> bfs_queue;
    bfs_queue.push(source);
    const auto current_bfs = ++last_bfs[source];

    while (!bfs_queue.empty()) {
      const size_t curr = bfs_queue.front();
      bfs_queue.pop();
      for (const auto edge : g.out_edges(curr)) {
        const size_t child = g.target(edge);
        if (last_bfs[child] == current_bfs)
          continue; // Already in the tree.
        if (flow[edge] == capacity[edge])
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

  Flow max_flow = 0;
  while (find_path()) {
    Flow path_flow = std::numeric_limits<Flow>::max();
    for (auto edge = parents[target]; edge != SIZE_MAX;
         edge = parents[g.source(edge)]) {
      path_flow = std::min(path_flow, capacity[edge] - flow[edge]);
    }
    for (auto edge = parents[target]; edge != SIZE_MAX;
         edge = parents[g.source(edge)]) {
      flow[edge] += path_flow;
      flow[rev_edge[edge]] -= path_flow;
    }
    max_flow += path_flow;
  }
  return max_flow;
}

} // end namespace djp

#endif // Header guard
