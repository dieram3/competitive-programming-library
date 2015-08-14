//          Copyright Diego Ramírez July 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DJP_GRAPH_TOPOLOGICAL_SORT_HPP
#define DJP_GRAPH_TOPOLOGICAL_SORT_HPP

#include <functional> // for std::function
#include <stdexcept>  // for std::logic_error
#include <vector>     // for std::vector
#include <cstddef>    // for std::size_t

namespace djp {

/// \brief Sorts the vertices of the given graph topologically.
///
/// \param g The target graph.
///
/// \returns A <tt>std::vector</tt> which contains the vertex descriptors of
/// \p graph in topological order.
///
/// \throws std::logic_error if \p graph is not a DAG.
///
/// \par Complexity
/// <tt>O(V + E)</tt>
///
template <typename Graph>
std::vector<size_t> topological_sort(const Graph &g) {
  enum colors { white, gray, black };
  const size_t num_v = g.num_vertices();
  std::vector<size_t> list(num_v);
  std::vector<colors> color(num_v, white);
  size_t cur_pos = num_v;

  std::function<void(size_t)> dfs_visit;
  dfs_visit = [&](const size_t src) {
    color[src] = gray;
    for (const auto e : g.out_edges(src)) {
      const size_t tgt = g.target(e);
      if (color[tgt] == white)
        dfs_visit(tgt);
      else if (color[tgt] == gray)
        throw std::logic_error("Not a DAG");
    }
    color[src] = black;
    list[--cur_pos] = src;
  };

  for (size_t v = 0; v != num_v; ++v)
    if (color[v] == white)
      dfs_visit(v);

  return list;
}

} // end namespace djp

#endif // Header guard
