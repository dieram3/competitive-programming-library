//          Copyright Diego Ramírez August 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DJP_GRAPH_GUSFIELD_ALL_PAIRS_MIN_CUT_HPP
#define DJP_GRAPH_GUSFIELD_ALL_PAIRS_MIN_CUT_HPP

#include <djp/graph/min_st_cut.hpp>
#include <djp/utility/matrix.hpp>

#include <algorithm> // for std::min
#include <limits>    // for std::numeric_limits
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
/// requires the input graph be a bidirectional Flow Network. A Flow Network is
/// bidirectional iff the capacity of each edge is equal to the capacity of its
/// reversed edge.
///
/// \param g The target graph.
/// \param rev_edge The reverse edge map.
/// \param capacity The capacity (or weight) map.
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
template <typename Graph, typename Flow>
matrix<Flow> gusfield_all_pairs_min_cut(const Graph &g,
                                        const std::vector<size_t> &rev_edge,
                                        const std::vector<Flow> &capacity) {

  const size_t num_vertices = g.num_vertices();
  std::vector<size_t> parent(num_vertices);
  matrix<Flow> cut({num_vertices, num_vertices},
                   std::numeric_limits<Flow>::max());

  std::vector<bool> source_side;
  for (size_t i = 1; i != num_vertices; ++i) {
    const Flow min_cut =
        min_st_cut(g, i, parent[i], rev_edge, capacity, source_side);
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
