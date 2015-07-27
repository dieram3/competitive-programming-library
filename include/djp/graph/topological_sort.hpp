//          Copyright Diego Ramírez July 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DJP_GRAPH_TOPOLOGICAL_SORT_HPP
#define DJP_GRAPH_TOPOLOGICAL_SORT_HPP

#include <stdexcept> // for std::logical_error
#include <vector>    // for std::vector
#include <cstddef>   // for std::size_t
#include <cassert>   // for assert

namespace djp {

/// \brief Sorts the vertices of the given graph topologically.
///
/// \param graph The target graph.
///
/// \returns A <tt>std::vector</tt> which contains the vertex descriptors of
/// \p graph in topological order.
///
/// \throws std::logic_error if \p graph is not a DAG.
///
/// \par Complexity
/// <tt>O(V + E)</tt>, where <tt>V = graph.num_vertices()</tt> and
/// <tt>E = graph.num_edges()</tt>
///
template <typename Graph>
std::vector<size_t> topological_sort(const Graph &graph) {

  const size_t num_vertices = graph.num_vertices();
  std::vector<size_t> sorted_list;
  sorted_list.reserve(num_vertices);

  std::vector<size_t> in_degree(num_vertices);

  for (size_t v = 0; v != num_vertices; ++v) {
    in_degree[v] = graph.in_degree(v);
    if (in_degree[v] == 0)
      sorted_list.push_back(v);
  }

  size_t num_processed = 0;
  while (num_processed < sorted_list.size()) {
    // Note: If sorted_list.size() - num_processed > 1 in some cycle, there are
    // multiple topological sorts.

    const size_t root = sorted_list[num_processed++];

    for (const auto *edge : graph.out_edges(root)) {
      const size_t child = edge->target;
      --in_degree[child];
      if (in_degree[child] == 0)
        sorted_list.push_back(child);
    }
  }

  if (num_processed < num_vertices)
    throw std::logic_error("topological_sort: Not a DAG");

  return sorted_list;
}

} // end namespace djp

#endif // Header guard
