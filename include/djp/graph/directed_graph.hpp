//          Copyright Diego Ramírez July 2014, July-August 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DJP_GRAPH_DIRECTED_GRAPH_HPP
#define DJP_GRAPH_DIRECTED_GRAPH_HPP

#include <vector>  // for std::vector
#include <utility> // for std::pair
#include <cstddef> // for std::size_t

namespace djp {

/// \brief Adjacency list which represents directed graphs.
///
class directed_graph {
  std::vector<std::vector<size_t>> outedges, inedges;
  std::vector<std::pair<size_t, size_t>> edge_list;

public:
  directed_graph(size_t n_verts) : outedges(n_verts), inedges(n_verts) {}

  size_t add_edge(size_t src, size_t tgt) {
    edge_list.emplace_back(src, tgt);
    const size_t edge_id = edge_list.size() - 1;
    outedges[src].push_back(edge_id);
    inedges[tgt].push_back(edge_id);
    return edge_id;
  }

  size_t num_vertices() const { return outedges.size(); }
  size_t num_edges() const { return edge_list.size(); }

  size_t source(size_t e) const { return edge_list[e].first; }
  size_t target(size_t e) const { return edge_list[e].second; }

  const std::vector<size_t> &out_edges(size_t v) const { return outedges[v]; }
  const std::vector<size_t> &in_edges(size_t v) const { return inedges[v]; }

  size_t out_degree(size_t v) const { return outedges[v].size(); }
  size_t in_degree(size_t v) const { return inedges[v].size(); }
};

} // namespace djp

#endif // Header guard
