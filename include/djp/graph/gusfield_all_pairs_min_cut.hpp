//          Copyright Diego Ramírez August 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DJP_GRAPH_GUSFIELD_ALL_PAIRS_MIN_CUT_HPP
#define DJP_GRAPH_GUSFIELD_ALL_PAIRS_MIN_CUT_HPP

#include <djp/graph/min_st_cut.hpp>
#include <djp/utility/matrix.hpp>

#include <algorithm> // for std::min
#include <vector>    // for std::vector
#include <cstddef>   // for std::size_t

namespace djp {

/// \brief Computes all min-cut pairs for an undirected graph by constructing
/// the Gomory-Hu tree.
///
/// This function implements the Gusfield Algorithm. It constructs the Gomory-Hu
/// tree without any vertex contraction so it is very efficient in practice.
///
/// The Gomory-Hu tree is a structure used to compute min-cut pairs in an
/// undirected graph, but for the sake of simplicity, this implementation
/// requires the input graph be a bidirectional FlowNetwork. A bidirectional
/// FlowNetwork is a FlowNetwork where the capacity of each edge is equal to the
/// capacity of its reverse edge.
///
/// \param graph The target graph.
///
/// \returns A matrix of flows \c cut where <tt>cut[{s, t}]</tt> evaluates to
/// the min s-t cut between the vertices \c s and \c t (being \c s != \c t).
///
/// \par Complexity
/// Exactly <tt>V - 1</tt> min s-t cut operations, where
/// <tt>V = graph.num_vertices()</tt>. In the current implementation, the
/// underlying min s-t cut uses the Edmonds-Karp max flow algorithm whichs has
/// complexity O(V * E^2) so the overall complexity of this function in the
/// worst case is <tt>O(V^2 * E^2)</tt>
///
template <typename Graph>
auto gusfield_all_pairs_min_cut(const Graph &graph)
    -> matrix<decltype(graph.edges().begin()->flow)> {

  using flow_t = decltype(graph.edges().begin()->flow);
  const size_t num_vertices = graph.num_vertices();
  std::vector<size_t> parent(num_vertices);
  matrix<flow_t> cut({num_vertices, num_vertices},
                     std::numeric_limits<flow_t>::max());

  std::vector<bool> source_side;
  for (size_t i = 1; i != num_vertices; ++i) {
    const flow_t min_cut = min_st_cut(graph, i, parent[i], source_side);
    for (size_t j = i + 1; j != num_vertices; ++j)
      if (source_side[j] && parent[j] == parent[i])
        parent[j] = i;
    cut[{i, parent[i]}] = cut[{parent[i], i}] = min_cut;
    for (size_t j = 0; j != i; ++j)
      cut[{i, j}] = cut[{j, i}] = std::min(min_cut, cut[{parent[i], j}]);
  }
  return cut;
}

} // end namespace djp

#endif // Header guard
