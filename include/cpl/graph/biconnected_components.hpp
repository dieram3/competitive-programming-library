//          Copyright Diego Ramirez 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef CPL_GRAPH_BICONNECTED_COMPONENTS_HPP
#define CPL_GRAPH_BICONNECTED_COMPONENTS_HPP

#include <algorithm>  // min
#include <cstddef>    // size_t
#include <cstdint>    // SIZE_MAX
#include <functional> // function
#include <stack>      // stack
#include <utility>    // pair
#include <vector>     // vector

namespace cpl {

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
/// \param[out] bicomp The map used to record the component label of each edge.
/// \param[out] is_articulation The map used to record whether a vertex is an
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
size_t biconnected_components(const Graph& g, std::vector<size_t>& bicomp,
                              std::vector<bool>& is_articulation) {

  enum colors { white, gray, black };
  const size_t num_vertices = g.num_vertices();
  size_t time = 0;
  size_t children_of_root = 0;
  size_t comp_cnt = 0;
  std::stack<std::pair<size_t, size_t>> stack; // edge, source of edge
  std::vector<size_t> pred(num_vertices, SIZE_MAX);
  std::vector<size_t> dtm(num_vertices);
  std::vector<size_t> low(num_vertices);
  std::vector<colors> color(num_vertices, white);
  is_articulation.assign(num_vertices, false);
  bicomp.resize(g.num_edges());

  std::function<void(size_t)> dfs_visit;
  dfs_visit = [&](const size_t src) {
    color[src] = gray;
    low[src] = dtm[src] = ++time;
    for (const auto e : g.out_edges(src)) {
      const size_t tgt = (src == g.source(e)) ? g.target(e) : g.source(e);
      if (tgt == pred[src])
        continue;
      if (color[tgt] == white) {
        stack.emplace(e, src);
        pred[tgt] = src;
        if (pred[src] == SIZE_MAX)
          ++children_of_root;
        dfs_visit(tgt);
        low[src] = std::min(low[src], low[tgt]);
      } else if (color[tgt] == gray) {
        stack.emplace(e, src);
        low[src] = std::min(low[src], dtm[tgt]);
      }
    }
    color[src] = black;
    const auto parent = pred[src];
    if (parent == SIZE_MAX) {
      is_articulation[src] = children_of_root > 1;
      return;
    }
    if (low[src] < dtm[parent])
      return;
    is_articulation[parent] = true;
    while (dtm[stack.top().second] >= dtm[src]) {
      bicomp[stack.top().first] = comp_cnt;
      stack.pop();
    }
    bicomp[stack.top().first] = comp_cnt++;
    stack.pop();
  };

  for (size_t v = 0; v != num_vertices; ++v) {
    if (color[v] == white) {
      children_of_root = 0;
      dfs_visit(v);
    }
  }
  return comp_cnt;
}

/// \brief Uses the Tarjan's algorithm to find the articulation points and
/// bridges of a simple graph.
///
/// \param g The target graph.
///
/// \param output_articulation_point Unary function. Each time an articulation
/// point \c v is found, it will be notified by invoking
/// <tt>output_articulation_point(v)</tt>. Articulation points are notified in
/// increasing order.
///
/// \param output_bridge Unary function. Each time a bridge \c e is found, it
/// will be notified by invoking <tt>output_bridge(e)</tt>. The order in which
/// bridges are notified is undetermined.
///
/// \pre
/// The graph \p g must be a simple graph i.e it must be undirected, have no
/// loops and have no parallel edges.
///
/// \par Complexity
/// <tt>O(V + E)</tt>
///
template <typename Graph, typename Out1, typename Out2>
void articulation_points_and_bridges(const Graph& g,
                                     Out1 output_articulation_point,
                                     Out2 output_bridge) {

  const size_t num_v = g.num_vertices();
  size_t children_of_root = 0;
  size_t time = 0;
  std::vector<size_t> pred(num_v, SIZE_MAX);
  std::vector<size_t> dtm(num_v);
  std::vector<size_t> low(num_v);
  std::vector<bool> is_articulation(num_v);

  std::function<void(size_t)> dfs_visit;
  dfs_visit = [&](const size_t src) {
    low[src] = dtm[src] = ++time;
    for (const auto e : g.out_edges(src)) {
      const size_t tgt = (src == g.source(e)) ? g.target(e) : g.source(e);
      if (!dtm[tgt]) {
        pred[tgt] = src;
        if (pred[src] == SIZE_MAX)
          ++children_of_root;
        dfs_visit(tgt);
        if (low[tgt] >= dtm[src])
          is_articulation[src] = true;
        if (low[tgt] > dtm[src])
          output_bridge(e);
        low[src] = std::min(low[src], low[tgt]);
      } else if (tgt != pred[src])
        low[src] = std::min(low[src], dtm[tgt]);
    }
  };

  for (size_t v = 0; v != num_v; ++v) {
    if (!dtm[v]) {
      children_of_root = 0;
      dfs_visit(v);
      is_articulation[v] = children_of_root > 1; // Fix root flag.
    }
    if (is_articulation[v])
      output_articulation_point(v);
  }
}

} // end namespace cpl

#endif // Header guard
