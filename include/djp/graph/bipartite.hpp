//          Copyright Diego Ramírez August 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DJP_GRAPH_BIPARTITE_HPP
#define DJP_GRAPH_BIPARTITE_HPP

#include <functional> // For std::function
#include <stdexcept>  // For std::logic_error
#include <vector>     // For std::vector
#include <cstddef>    // For std::size_t

namespace djp {

/// \brief Checks if an undirected graph is bipartite.
///
/// An undirected graph is bipartite if it can be partitioned into two groups of
/// vertices such that no two neighboring vertices belong to the same side. If
/// \p g is bipartite, this function returns \c true and records in the \p color
/// map the chosen side of each vertex ('true' side or 'false' side).
/// Conversely, if \p g is not bipartite, this function returns \c false and the
/// \p color map is left undefined.
///
/// \param g The target graph.
/// \param[out] color The color (or side) map.
///
/// \returns \c true if \p g is bipartite, \c false otherwise.
///
/// \par Complexity
/// <tt>O(V + E)</tt>
///
template <typename Graph>
bool is_bipartite(const Graph &g, std::vector<bool> &color) {
  const size_t num_v = g.num_vertices();
  std::vector<bool> visited(num_v);
  color.resize(num_v);

  std::function<void(size_t)> dfs_visit;
  dfs_visit = [&](const size_t src) {
    visited[src] = true;
    for (const auto e : g.out_edges(src)) {
      const size_t tgt = (src == g.source(e)) ? g.target(e) : g.source(e);
      if (!visited[tgt]) {
        color[tgt] = !color[src];
        dfs_visit(tgt);
      } else if (color[src] == color[tgt])
        throw std::logic_error("Not bipartite");
    }
  };

  for (size_t v = 0; v != num_v; ++v)
    if (!visited[v])
      try {
        dfs_visit(v); // The color of 'v' is left as is.
      } catch (std::logic_error &) {
        return false;
      }
  return true;
}

} // end namespace djp

#endif // header guard
