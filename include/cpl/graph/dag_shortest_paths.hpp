//          Copyright Diego Ramírez August 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef CPL_GRAPH_DAG_SHORTEST_PATHS_HPP
#define CPL_GRAPH_DAG_SHORTEST_PATHS_HPP

#include <algorithm>  // For std::for_each, std::min
#include <functional> // For std::function
#include <limits>     // For std::numeric_limits
#include <vector>     // For std::vector
#include <cstddef>    // For std::size_t

namespace cpl {

/// \brief Solves the single-source shortest-paths problem on a weighted,
/// directed acyclic graph.
///
/// Distance to each vertex from \p source is recorded in the \p dist map.
/// Distances to unreachable vertices are set to
/// <tt>std::numeric_limits<Distance>::max()</tt>.
///
/// \param g The target graph.
/// \param source Descriptor of the source vertex.
/// \param weight The edge-weight map.
/// \param[out] dist The vertex-distance map.
///
/// \pre The graph \p g must be a directed acyclic graph (DAG).
///
/// \par Complexity
/// The time complexity is <tt>O(V + E)</tt>.
///
template <typename Graph, typename Distance>
void dag_shortest_paths(const Graph &g, const size_t source,
                        const std::vector<Distance> &weight,
                        std::vector<Distance> &dist) {
  const size_t num_v = g.num_vertices();
  std::vector<bool> visited(num_v);
  std::vector<size_t> rev_topo;
  rev_topo.reserve(num_v);

  std::function<void(size_t)> dfs = [&](const size_t src) {
    visited[src] = true;
    for (const auto e : g.out_edges(src)) {
      const size_t tgt = g.target(e);
      if (!visited[tgt])
        dfs(tgt);
    }
    rev_topo.push_back(src);
  };
  dfs(source);

  // Note: If 'limits::max()' is changed by 'limits::min()' and 'std::min' is
  // changed by 'std::max' it gives as a result the longest paths.
  dist.assign(num_v, std::numeric_limits<Distance>::max());
  dist[source] = 0;
  std::for_each(rev_topo.rbegin(), rev_topo.rend(), [&](const size_t src) {
    for (const auto e : g.out_edges(src)) {
      const size_t tgt = g.target(e);
      dist[tgt] = std::min(dist[tgt], dist[src] + weight[e]);
    }
  });
}

} // End namespace cpl

#endif // Header guard
