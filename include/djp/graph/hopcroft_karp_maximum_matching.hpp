//          Copyright Diego Ramírez July 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DJP_GRAPH_HOPCROFT_KARP_MAXIMUM_MATCHING_HPP
#define DJP_GRAPH_HOPCROFT_KARP_MAXIMUM_MATCHING_HPP

#include <queue>     // for std::queue
#include <stack>     // for std::stack
#include <stdexcept> // for std::logic_error
#include <vector>    // for std::vector
#include <cstddef>   // for std::size_t
#include <cstdint>   // for SIZE_MAX

namespace djp {

template <typename Graph>
class hopcroft_karp {
  const Graph &g;
  const size_t num_vertices;
  const size_t null_vertex;
  std::vector<size_t> pair_of; // This can be used to query the selected pairs.
  std::vector<size_t> dist;
  std::vector<size_t> red_vertices;
  std::vector<size_t> green_vertices;
  size_t matching;

private:
  // Separates the graph in red vertices and green vertices
  // Throws std::logic_error if the graph is not bipartite.
  // Take into account that this function would not be necessary if red and
  // green vertices were known in advance.
  void make_partition() {
    std::vector<int> color_of(num_vertices); // 1 red, -1 green, 0 unknown
    std::stack<size_t> dfs_stack;

    auto coloring_component = [&, this](const size_t source) {
      color_of[source] = 1;
      dfs_stack.push(source);
      while (!dfs_stack.empty()) {
        const size_t u = dfs_stack.top();
        dfs_stack.pop();
        for (const auto e : g.out_edges(u)) {
          const size_t v = (u == g.source(e)) ? g.target(e) : g.source(e);
          if (color_of[v] == 0) {
            color_of[v] = -color_of[u];
            dfs_stack.push(v);
          } else if (color_of[v] == color_of[u])
            throw std::logic_error("Non bipartite graph");
        }
      }
    };

    for (size_t v = 0; v != num_vertices; ++v) {
      if (color_of[v] == 0)
        coloring_component(v);
      color_of[v] == 1 ? red_vertices.push_back(v)
                       : green_vertices.push_back(v);
    }
  }

  bool bfs() {
    std::queue<size_t> bfs_queue;
    for (const size_t rv : red_vertices) {
      if (pair_of[rv] == null_vertex) {
        dist[rv] = 0;
        bfs_queue.push(rv);
      } else {
        dist[rv] = SIZE_MAX;
      }
    }
    dist[null_vertex] = SIZE_MAX;
    while (!bfs_queue.empty()) {
      const size_t rv = bfs_queue.front();
      bfs_queue.pop();
      if (dist[rv] >= dist[null_vertex])
        continue;
      for (const auto e : g.out_edges(rv)) {
        const size_t gv = (rv == g.source(e)) ? g.target(e) : g.source(e);
        if (dist[pair_of[gv]] != SIZE_MAX)
          continue;
        dist[pair_of[gv]] = dist[rv] + 1;
        bfs_queue.push(pair_of[gv]);
      }
    }
    return dist[null_vertex] != SIZE_MAX;
  }

  bool dfs(const size_t rv) {
    if (rv == null_vertex)
      return true;
    for (const auto e : g.out_edges(rv)) {
      const auto gv = (rv == g.source(e)) ? g.target(e) : g.source(e);
      if (dist[pair_of[gv]] != dist[rv] + 1)
        continue;
      if (!dfs(pair_of[gv]))
        continue;
      pair_of[gv] = rv;
      pair_of[rv] = gv;
      return true;
    }
    dist[rv] = SIZE_MAX;
    return false;
  }

public:
  hopcroft_karp(const Graph &graph)
      : g{graph}, num_vertices{g.num_vertices()}, null_vertex{num_vertices} {

    pair_of.resize(num_vertices, null_vertex);
    dist.resize(num_vertices + 1);
    make_partition();
    matching = 0;
    while (bfs()) {
      for (const size_t rv : red_vertices) {
        if (pair_of[rv] != null_vertex)
          continue;
        if (dfs(rv))
          ++matching;
      }
    }
  }

  std::size_t maximum_matching() const { return matching; }
};

/// \brief Finds the maximum cardinality matching in an undirected bipartite
/// graph.
///
/// \param graph The target graph.
///
/// Finds the maximum cardinality matching in the given graph i.e the maximum
/// number of edges in a set such that no two edges share a common endpoint.
///
/// \returns The maximum cardinality matching.
///
/// \throws std::logic_error if \p graph is not bipartite.
///
/// \par Complexity
/// <tt>O(E * sqrt(V))</tt>, where <tt>V = graph.num_vertices()</tt> and
/// <tt>E = graph.num_edges()</tt>.
///
template <typename Graph>
std::size_t hopcroft_karp_maximum_matching(const Graph &graph) {
  const hopcroft_karp<Graph> hk(graph);
  return hk.maximum_matching();
}

} // end namespace djp

#endif // Header guard
