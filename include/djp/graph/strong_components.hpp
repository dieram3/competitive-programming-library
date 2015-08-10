//          Copyright Diego Ramírez August 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DJP_GRAPH_STRONG_COMPONENTS_HPP
#define DJP_GRAPH_STRONG_COMPONENTS_HPP

#include <stack>   // for std::stack
#include <utility> // for std::pair
#include <vector>  // for std::vector
#include <cstddef> // for std::size_t
#include <cstdint> // for SIZE_MAX

#include <iostream> // TEMPORAL

namespace djp {

/// \brief Finds the strongly connected components (SCC) of a directed graph.
///
/// Uses an iterative version of Tarjan's algorithm based on DFS to find the SCC
/// of the input graph. The component labels are recorded in the \p comp map.
/// Label values will be bounded into the range <tt>[0, C)</tt>, being \c C the
/// total number of strongly connected components.
///
/// \param g The target graph.
/// \param[out] comp The component map.
///
/// \returns The total number of strongly connected components.
///
/// \par Complexity
/// <tt>(V + E)</tt>
///
template <typename Graph>
size_t strong_components(const Graph &g, std::vector<size_t> &comp) {

  const size_t num_vertices = g.num_vertices();
  std::stack<size_t> s;
  std::vector<size_t> root(num_vertices);
  std::vector<size_t> dfs_time(num_vertices);
  size_t time = 0;
  size_t comp_cnt = 0;

  auto discover = [&](const size_t v) {
    root[v] = v;
    comp[v] = SIZE_MAX;
    s.push(v);
    dfs_time[v] = ++time;
  };

  auto finish = [&](const size_t v) {
    for (const auto edge : g.out_edges(v)) {
      const auto w = g.target(edge);
      if (comp[w] == SIZE_MAX && dfs_time[root[w]] < dfs_time[root[v]])
        root[v] = root[w];
    }
    if (v != root[v])
      return;
    size_t w;
    do {
      w = s.top(), s.pop();
      comp[w] = comp_cnt;
    } while (w != v);
    ++comp_cnt;
  };

  auto dfs_visit = [&](const size_t start) {
    std::stack<std::pair<size_t, bool>> dfs_stack;
    dfs_stack.emplace(start, false);
    while (!dfs_stack.empty()) {
      const size_t u = dfs_stack.top().first;
      const bool backtrack = dfs_stack.top().second;
      dfs_stack.pop();
      if (!backtrack) {
        if (dfs_time[u])
          continue; // Non-tree edge.
        discover(u);
        dfs_stack.emplace(u, true);
        for (const auto edge : g.out_edges(u))
          dfs_stack.emplace(g.target(edge), false);
      } else
        finish(u);
    }
  };

  comp.resize(num_vertices);
  for (size_t v = 0; v != num_vertices; ++v)
    if (!dfs_time[v])
      dfs_visit(v);
  return comp_cnt;
}

} // end namespace djp

#endif // Header guard
