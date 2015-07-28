//          Copyright Diego Ramírez July 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DJP_GRAPH_CONNECTED_COMPONENTS_HPP
#define DJP_GRAPH_CONNECTED_COMPONENTS_HPP

#include <stack>   // for std::stack
#include <vector>  // for std::vector
#include <cassert> // for assert
#include <cstddef> // for std::size_t

namespace djp {

/// \brief Computes the connected components of an undirected graph using a
/// DFS-based approach.
///
/// \param graph The target graph.
/// \param[out] labels Optional parameter where the label of each vertex is put.
///
/// If \p labels is not null a <tt>std::vector</tt> of size
/// <tt>graph.num_vertices()</tt> will be asigned
/// to it so labels[v] returns the label of the component which \c v belongs
/// to. A label is a value in the range <tt>[0, N)</tt> where <tt>N =
/// Total-number-of-components</tt>, which identifies a component in the graph.
///
/// The way in which labels are assigned is the incremental way i.e
/// <tt>label[i]</tt> can have value L iff all labels in the range
/// <tt>[0, L)</tt> have appeared in the range <tt>label[0 .. i - 1]</tt>.
///
/// \returns The total number of components.
///
/// \par Complexity
/// <tt>O(V + E)</tt>, where <tt>V = graph.num_vertices()</tt> and
/// <tt>E = graph.num_edges()</tt>.
///
template <typename Graph>
std::size_t connected_components(const Graph &graph,
                                 std::vector<size_t> *labels = nullptr) {
  const size_t num_vertices = graph.num_vertices();
  const size_t invalid_component = num_vertices;

  std::vector<size_t> component_of(num_vertices, invalid_component);
  std::stack<size_t> dfs_stack;

  size_t current_component = 0;
  for (size_t source = 0; source != num_vertices; ++source) {
    if (component_of[source] != invalid_component)
      continue;

    component_of[source] = current_component;
    dfs_stack.push(source);
    while (!dfs_stack.empty()) {
      const size_t current = dfs_stack.top();
      dfs_stack.pop();

      for (const auto *edge : graph.out_edges(current)) {
        const size_t neighbor = edge->target;
        if (component_of[neighbor] != invalid_component) {
          assert(component_of[neighbor] == current_component);
          continue;
        }

        component_of[neighbor] = current_component;
        dfs_stack.push(neighbor);
      }
    }
    ++current_component;
  }

  if (labels)
    labels->swap(component_of);

  return current_component;
}

} // end namespace djp

#endif // Header guard
