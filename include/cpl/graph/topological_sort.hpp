//          Copyright Diego Ramirez 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef CPL_GRAPH_TOPOLOGICAL_SORT_HPP
#define CPL_GRAPH_TOPOLOGICAL_SORT_HPP

#include <cstddef>    // size_t
#include <functional> // function
#include <queue>      // priority_queue
#include <stdexcept>  // logic_error
#include <vector>     // vector

namespace cpl {

/// \brief Sorts the vertices of the given graph topologically.
///
/// Uses a DFS-based approach to find a topological sort of the given graph. If
/// multiple topological sorts exist, any of them is found.
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
/// \sa prioritized_topological_sort
///
template <typename Graph>
std::vector<size_t> topological_sort(const Graph& g) {
  enum class colors { white, gray, black };
  const size_t num_v = g.num_vertices();
  std::vector<size_t> list(num_v);
  std::vector<colors> color(num_v, colors::white);
  size_t cur_pos = num_v;

  std::function<void(size_t)> dfs_visit;
  dfs_visit = [&](const size_t src) {
    color[src] = colors::gray;
    for (const auto e : g.out_edges(src)) {
      const size_t tgt = g.target(e);
      if (color[tgt] == colors::white)
        dfs_visit(tgt);
      else if (color[tgt] == colors::gray)
        throw std::logic_error("Not a DAG");
    }
    color[src] = colors::black;
    list[--cur_pos] = src;
  };

  for (size_t v = 0; v != num_v; ++v)
    if (color[v] == colors::white)
      dfs_visit(v);

  return list;
}

/// \brief Sorts the directed graph in topological order and prioritizes
/// candidate vertices according to the given comparator.
///
/// Sorts the vertices of the graph \p g topologically. If multiple vertices can
/// be chosen at a given moment, the one with the highest priority will be
/// outputted first.
///
/// \param g The target graph.
///
/// \param comp The comparator used to prioritize vertices. Given two vertices
/// \c u and \c v, <tt>comp(u, v)</tt> must return \c true if \c u has lower
/// priority than \c v.
///
/// \param[out] output_vertex Each time a new vertex \c v is added to the
/// topological sort, it will be notified via this parameter by calling
/// <tt>output_vertex(v)</tt>. This unary function shall be called exactly once
/// for each vertex in the graph in their respective topological order. If the
/// number of calls is less than <tt>g.num_vertices()</tt>, it means that \p g
/// is not a DAG.
///
/// \par Complexity
/// <tt>O(E + C*V*log(V))</tt> where \c C is the cost of \p comp.
///
/// \sa topological_sort
///
template <typename Graph, typename Comp, typename UnaryFunction>
void prioritized_topological_sort(const Graph& g, Comp comp,
                                  UnaryFunction output_vertex) {
  const size_t num_vertices = g.num_vertices();
  const size_t num_edges = g.num_edges();
  std::priority_queue<size_t, std::vector<size_t>, Comp> queue(comp);
  std::vector<size_t> in_degree(num_vertices);

  for (size_t e = 0; e != num_edges; ++e)
    ++in_degree[g.target(e)];

  for (size_t v = 0; v != num_vertices; ++v)
    if (!in_degree[v])
      queue.push(v);

  while (!queue.empty()) {
    const size_t src = queue.top();
    queue.pop();
    output_vertex(src);

    for (const auto e : g.out_edges(src)) {
      const auto tgt = g.target(e);
      --in_degree[tgt];
      if (!in_degree[tgt])
        queue.push(tgt);
    }
  }
}

} // end namespace cpl

#endif // Header guard
