//          Copyright Diego Ramírez August 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DJP_GRAPH_BICONNECTED_COMPONENTS_HPP
#define DJP_GRAPH_BICONNECTED_COMPONENTS_HPP

#include <algorithm>  // for std::min
#include <functional> // for std::function
#include <stack>      // for std::stack
#include <vector>     // for std::vector
#include <utility>    // for std::pair
#include <cstddef>    // for std::size_t

namespace djp {

/// \brief Finds the biconnected components of an undirected graph.
///
/// Uses the Tarjan's algorithm to find the biconnected components and
/// (additionally) the articulation points of the given graph.
///
/// The vector map \p comp will be resized and modified in such a way that
/// <tt>comp[e]</tt> will contain the label of the biconnected component whichs
/// the edge \c e belongs to. Labels are integer values into the range
/// <tt>[0, C)</tt> being \c C the total number of biconnected components.
///
/// Similarly, the vector map \p is_cut will be resized and modified in such a
/// way that <tt>is_cut[v]</tt> evaluates to \c true if \c v is an articulation
/// point (or cut vertex).
///
/// \param g The input graph.
/// \param[out] comp The map used to record the component label of each edge.
/// \param[out] is_cut The map used to record whether a vertex is an
/// articulation point or not.
///
/// \returns The total number of biconnected components.
///
/// \pre The input graph \p g shall be a simple graph.
///
/// \par Complexity
/// <tt>O(V + E)</tt>
///
/// \note This function can  also be used to find bridges. If and edge \c e is
/// the unique member of its biconnected component, then \c e is a bridge.
///
template <typename Graph>
std::size_t biconnected_components(const Graph &g, std::vector<size_t> &comp,
                                   std::vector<bool> &is_cut) {

  enum colors { white, gray, black };
  const size_t num_vertices = g.num_vertices();
  size_t time = 0;
  size_t children_of_root = 0;
  size_t comp_cnt = 0;
  std::stack<std::pair<size_t, size_t>> s; // edge, source of edge
  std::vector<size_t> pred(num_vertices);
  std::vector<size_t> dtm(num_vertices);
  std::vector<size_t> low(num_vertices);
  std::vector<colors> color(num_vertices, white);
  is_cut.resize(num_vertices);
  comp.resize(g.num_edges());

  std::function<void(size_t)> dfs_visit;
  dfs_visit = [&](const size_t src) {
    color[src] = gray;
    low[src] = dtm[src] = ++time;

    for (const auto e : g.out_edges(src)) {
      const size_t tgt = (src == g.source(e)) ? g.target(e) : g.source(e);
      if (color[tgt] == white) {
        s.emplace(e, src);
        pred[tgt] = src;
        if (pred[src] == src)
          ++children_of_root;
        dfs_visit(tgt);
      } else if (color[tgt] == gray) {
        if (tgt != pred[src]) {
          s.emplace(e, src);
          low[src] = std::min(low[src], dtm[tgt]);
        }
      }
    }
    color[src] = black;
    const auto parent = pred[src];
    if (parent == src) {
      is_cut[src] = children_of_root > 1;
    } else {
      low[parent] = std::min(low[parent], low[src]);
      if (low[src] >= dtm[parent]) {
        is_cut[parent] = true;
        while (dtm[s.top().second] >= dtm[src])
          comp[s.top().first] = comp_cnt, s.pop();
        comp[s.top().first] = comp_cnt++, s.pop();
      }
    }
  };

  for (size_t v = 0; v != num_vertices; ++v)
    pred[v] = v;

  for (size_t v = 0; v != num_vertices; ++v)
    if (color[v] == white) {
      children_of_root = 0;
      dfs_visit(v);
    }
  return comp_cnt;
}

} // end namespace djp

#endif // Header guard
