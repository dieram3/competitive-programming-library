//          Copyright Diego Ramirez 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef CPL_GRAPH_BRIDGES_HPP
#define CPL_GRAPH_BRIDGES_HPP

#include <algorithm>  // min
#include <cstddef>    // size_t
#include <functional> // function
#include <vector>     // vector

namespace cpl {

/// \brief Finds the bridges of the given simple graph.
///
/// Uses the Tarjan's algorithm to find the bridges of the graph \c g. An edge
/// \c e is a bridge of \p g if its deletion increases the number of connected
/// components.
///
/// \param g The target graph.
/// \param[out] output_bridge Each time a bridge \c e is found, it will be
/// notified via this parameter by calling <tt>output_bridge(e)</tt>. The order
/// in which bridges are notified is undetermined.
///
/// \pre The graph \p g must be a simple graph, that is, it must be undirected
/// and have no loops or parallel edges.
///
/// \par Complexity
/// <tt>O(V + E)</tt>
///
/// \sa articulation_points_and_bridges
/// \sa biconnected_components
/// \sa connected_components
///
template <typename Graph, typename UnaryFunction>
void find_bridges(const Graph& g, UnaryFunction output_bridge) {

  const size_t num_v = g.num_vertices();
  size_t time = 0;
  std::vector<size_t> pred(num_v, SIZE_MAX);
  std::vector<size_t> dtm(num_v);
  std::vector<size_t> low(num_v);

  std::function<void(size_t)> dfs_visit;
  dfs_visit = [&](const size_t src) {
    low[src] = dtm[src] = ++time;
    for (const auto e : g.out_edges(src)) {
      const size_t tgt = (src == g.source(e)) ? g.target(e) : g.source(e);
      if (!dtm[tgt]) {
        pred[tgt] = src;
        dfs_visit(tgt);
        low[src] = std::min(low[src], low[tgt]);
        if (low[tgt] > dtm[src])
          output_bridge(e);
      } else if (tgt != pred[src])
        low[src] = std::min(low[src], dtm[tgt]);
    }
  };

  for (size_t v = 0; v != num_v; ++v)
    if (!dtm[v])
      dfs_visit(v);
}

} // end namespace cpl

#endif // Header guard
