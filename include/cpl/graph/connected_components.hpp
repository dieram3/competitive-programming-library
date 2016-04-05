//          Copyright Diego Ramírez July-August 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef CPL_GRAPH_CONNECTED_COMPONENTS_HPP
#define CPL_GRAPH_CONNECTED_COMPONENTS_HPP

#include <vector>  // for std::vector
#include <cstddef> // for std::size_t
#include <cstdint> // for SIZE_MAX

namespace cpl {

/// \brief Computes the connected components of an undirected graph using a
/// DFS-based approach.
///
/// \param g The target graph.
/// \param[out] component_of Map vector where the component label of each vertex
/// is recorded.
///
/// A label is defined as an integer value in the range <tt>[0, N)</tt> (where
/// <tt>N = Total-number-of-components</tt>) which uniquely identifies one of
/// the \c N components in the graph.
///
/// The \p component_of \c vector will be resized to the number of vertices in
/// the graph and modified in such a way that \c component_of[v] maps to the
/// label of the component which \c v belongs to.
///
/// Labels are assigned incrementally, i.e <tt>component_of[v]</tt> can have
/// value L iff all labels in the range <tt>[0, L)</tt> have been used for
/// preceding vertices (vertices indexed with values less than \c v).
///
/// \returns The total number of components.
///
/// \par Complexity
/// <tt>O(V + E)</tt>.
///
template <typename Graph>
size_t connected_components(const Graph &g, std::vector<size_t> &component_of) {
  const size_t num_vertices = g.num_vertices();
  component_of.assign(num_vertices, SIZE_MAX);

  std::vector<size_t> stack;
  auto explore = [&](const size_t source, const size_t comp_label) {
    component_of[source] = comp_label;
    stack.push_back(source);
    while (!stack.empty()) {
      const auto u = stack.back();
      stack.pop_back();
      for (const auto e : g.out_edges(u)) {
        const auto v = (u == g.source(e)) ? g.target(e) : g.source(e);
        if (component_of[v] != SIZE_MAX)
          continue; // It has been discovered before.
        component_of[v] = comp_label;
        stack.push_back(v);
      }
    }
  };

  size_t num_components = 0;
  for (size_t v = 0; v != num_vertices; ++v)
    if (component_of[v] == SIZE_MAX)
      explore(v, num_components++);

  return num_components;
}

} // end namespace cpl

#endif // Header guard
