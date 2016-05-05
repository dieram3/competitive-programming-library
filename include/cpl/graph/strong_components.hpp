//          Copyright Diego Ramirez 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef CPL_GRAPH_STRONG_COMPONENTS_HPP
#define CPL_GRAPH_STRONG_COMPONENTS_HPP

#include <algorithm>  // min
#include <cstddef>    // size_t
#include <cstdint>    // SIZE_MAX
#include <functional> // function
#include <stack>      // stack
#include <vector>     // vector

namespace cpl {

/// \brief Finds the strongly connected components (SCC) of a directed graph.
///
/// Uses the Tarjan's algorithm based to find the SCC of the input graph. The
/// component labels are recorded in the \p comp map. Label values will be
/// bounded into the range <tt>[0, C)</tt>, being \c C the total number of
/// strongly connected components.
///
/// \param g The target graph.
/// \param[out] comp The component map.
///
/// \returns The total number of strongly connected components.
///
/// \par Complexity
/// <tt>(V + E)</tt>
///
/// \note As a byproduct, the sequence <tt>[C - 1, C - 2, ..., 2, 1, 0]</tt>
/// will form a valid topological sorting of the condensation of \p g (where \c
/// C = the total number of SCC).
///
template <typename Graph>
size_t strong_components(const Graph& g, std::vector<size_t>& comp) {

  const size_t num_vertices = g.num_vertices();
  size_t time = 0;
  size_t comp_cnt = 0;
  std::stack<size_t> stack;
  std::vector<size_t> low(num_vertices);
  std::vector<size_t> dtm(num_vertices);
  comp.resize(num_vertices);

  std::function<void(size_t)> dfs_visit;
  dfs_visit = [&](const size_t v) {
    low[v] = dtm[v] = ++time;
    comp[v] = SIZE_MAX;
    stack.push(v);
    for (const auto edge : g.out_edges(v)) {
      const size_t w = g.target(edge);
      if (!dtm[w]) {
        dfs_visit(w);
        low[v] = std::min(low[v], low[w]);
      } else if (comp[w] == SIZE_MAX)
        low[v] = std::min(low[v], dtm[w]);
    }
    if (dtm[v] != low[v])
      return;
    while (true) {
      const size_t w = stack.top();
      stack.pop();
      comp[w] = comp_cnt;
      if (w == v)
        break;
    }
    ++comp_cnt;
  };

  for (size_t v = 0; v != num_vertices; ++v)
    if (!dtm[v])
      dfs_visit(v);
  return comp_cnt;
}

} // end namespace cpl

#endif // Header guard
