//          Copyright Diego Ramírez July 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef CPL_GRAPH_HOPCROFT_KARP_MAXIMUM_MATCHING_HPP
#define CPL_GRAPH_HOPCROFT_KARP_MAXIMUM_MATCHING_HPP

#include <functional> // for std::function
#include <queue>      // for std::queue
#include <vector>     // for std::vector
#include <cstddef>    // for std::size_t
#include <cstdint>    // for SIZE_MAX

namespace cpl {

/// \brief Finds the maximum cardinality matching in an undirected bipartite
/// graph.
///
/// \param g The target graph.
///
/// Finds the maximum cardinality matching in the given graph. It is the maximum
/// number of edges in an edge subset, such that no two edges share a common
/// endpoint.
///
/// \returns The maximum cardinality matching.
///
/// \pre The graph \p g must be bipartite.
///
/// \par Complexity
/// <tt>O(E * sqrt(V))</tt>.
///
template <typename Graph>
std::size_t hopcroft_karp_maximum_matching(const Graph &g) {
  // Note: pair_of can be used to query the selected pairs.
  const size_t num_vertices = g.num_vertices();
  const size_t nil = num_vertices; // The null vertex
  std::vector<size_t> pair_of(num_vertices, nil);
  std::vector<size_t> dist(num_vertices + 1);
  std::vector<size_t> set_a, set_b;

  auto separate_vertices = [&] {
    std::vector<signed char> color(num_vertices);
    std::function<void(size_t)> dfs_visit;
    dfs_visit = [&](const size_t u) {
      color[u] == 1 ? set_a.push_back(u) : set_b.push_back(u);
      for (const auto e : g.out_edges(u)) {
        const size_t v = (u == g.source(e)) ? g.target(e) : g.source(e);
        if (color[v])
          continue;
        color[v] = -color[u];
        dfs_visit(v);
      }
    };
    for (size_t v = 0; v != num_vertices; ++v)
      if (!color[v]) {
        color[v] = 1;
        dfs_visit(v);
      }
  };
  auto bfs = [&] {
    std::queue<size_t> q;
    for (const size_t a : set_a) {
      if (pair_of[a] == nil) {
        dist[a] = 0;
        q.push(a);
      } else
        dist[a] = SIZE_MAX;
    }
    dist[nil] = SIZE_MAX;
    while (!q.empty()) {
      const size_t a = q.front();
      q.pop();
      if (dist[a] >= dist[nil])
        continue;
      for (const auto e : g.out_edges(a)) {
        const size_t b = (a == g.source(e)) ? g.target(e) : g.source(e);
        if (dist[pair_of[b]] != SIZE_MAX)
          continue;
        dist[pair_of[b]] = dist[a] + 1;
        q.push(pair_of[b]);
      }
    }
    return dist[nil] != SIZE_MAX;
  };
  std::function<bool(size_t)> dfs = [&](const size_t a) {
    if (a == nil)
      return true;
    for (const auto e : g.out_edges(a)) {
      const auto b = (a == g.source(e)) ? g.target(e) : g.source(e);
      if (dist[pair_of[b]] != dist[a] + 1)
        continue;
      if (!dfs(pair_of[b]))
        continue;
      pair_of[b] = a;
      pair_of[a] = b;
      return true;
    }
    dist[a] = SIZE_MAX;
    return false;
  };

  separate_vertices();
  size_t num_matching = 0;
  while (bfs())
    for (const size_t a : set_a)
      if (pair_of[a] == nil && dfs(a))
        ++num_matching;
  return num_matching;
}

} // end namespace cpl

#endif // Header guard
