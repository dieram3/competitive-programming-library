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
#include <type_traits> // For std::is_arithmetic
#include <vector>      // For std::vector
#include <cstddef>     // For std::size_t
#include <cstdint>     // for SIZE_MAX

namespace djp {

/// \brief Solves the maximum flow problem using the Edmonds-Karp algorithm.
///
/// This algorithm requires that each edge in the graph has its own reverse
/// edge. Reversed edges that were not intended to be part of the modeled graph
/// should have a capacity of 0. If \c g is a multigraph, each edge
/// <tt>(u, v)</tt> must have its own counterpart <tt>(v, u)</tt>.
///
/// \param g The target graph.
/// \param source The source vertex.
/// \param target The target vertex.
/// \param rev_edge The reverse edge map.
/// \param capacity The capacity map.
/// \param[out] residual The residual capacity map. The unused capacity of each
/// edge will be recorded in this map. The final flow of each edge \c e can be
/// obtained as <tt>capacity[e] - residual[e]</tt>.
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
                           std::vector<Flow> &residual) {

  static_assert(std::is_arithmetic<Flow>::value, "'Flow' must be arithmetic.");
  using edge_id = size_t;

  // last_bfs[v] stores the the last BFS tree that vertex v was part of.  Note
  // that the source vertex is present in all BFS trees as it is the root.
  std::vector<unsigned> last_bfs(g.num_vertices());
  std::vector<edge_id> pred(g.num_vertices(), SIZE_MAX);

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
        if (!residual[edge])
          continue; // Can't navigate through saturated edges.
        pred[child] = edge;
        if (child == target)
          return true;
        bfs_queue.push(child);
        last_bfs[child] = current_bfs;
      }
    }
    return false;
  };

  Flow total_flow = 0;
  residual = capacity;
  while (find_path()) {
    Flow path_flow = std::numeric_limits<Flow>::max();
    for (auto e = pred[target]; e != SIZE_MAX; e = pred[g.source(e)]) {
      path_flow = std::min(path_flow, residual[e]);
    }
    for (auto e = pred[target]; e != SIZE_MAX; e = pred[g.source(e)]) {
      residual[e] -= path_flow;
      residual[rev_edge[e]] += path_flow;
    }
    total_flow += path_flow;
  }
  return total_flow;
}

} // end namespace djp

#endif // Header guard
