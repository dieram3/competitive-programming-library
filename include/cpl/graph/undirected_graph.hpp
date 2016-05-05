//          Copyright Diego Ramirez 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef CPL_GRAPH_UNDIRECTED_GRAPH_HPP
#define CPL_GRAPH_UNDIRECTED_GRAPH_HPP

#include <cstddef> // size_t
#include <utility> // pair
#include <vector>  // vector

namespace cpl {

/// \brief Adjacency list which represents undirected graphs.
///
class undirected_graph {
  std::vector<std::vector<size_t>> adj_edges;
  std::vector<std::pair<size_t, size_t>> edge_list;

public:
  explicit undirected_graph(size_t num_vertices) : adj_edges(num_vertices) {}

  size_t add_edge(size_t u, size_t v) {
    edge_list.emplace_back(u, v);
    const size_t edge_id = edge_list.size() - 1;
    adj_edges[u].push_back(edge_id);
    adj_edges[v].push_back(edge_id);
    return edge_id;
  }

  size_t num_vertices() const {
    return adj_edges.size();
  }
  size_t num_edges() const {
    return edge_list.size();
  }

  size_t source(size_t e) const {
    return edge_list[e].first;
  }
  size_t target(size_t e) const {
    return edge_list[e].second;
  }

  const std::vector<size_t>& out_edges(size_t v) const {
    return adj_edges[v];
  }
  const std::vector<size_t>& in_edges(size_t v) const {
    return adj_edges[v];
  }

  size_t degree(size_t v) const {
    return adj_edges[v].size();
  }
  size_t out_degree(size_t v) const {
    return degree(v);
  }
  size_t in_degree(size_t v) const {
    return degree(v);
  }
};

} // end namespace cpl

#endif // Header guard
