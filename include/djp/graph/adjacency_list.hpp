//          Copyright Diego Ramírez July 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DJP_GRAPH_ADJACENCY_LIST_HPP
#define DJP_GRAPH_ADJACENCY_LIST_HPP

#include <vector>  // for std::vector
#include <map>     // for std::set

namespace djp {

/// Represents a directed graph with weighted edges.
class adjacency_list {
 public:
  using size_type = std::size_t;

  struct vertex_info {
    std::map<size_type, size_type> out_edges_;
  };

  adjacency_list(size_type nvertices) : vertices(nvertices) {}

  void add_edge(size_type from, size_type to, size_type weight = 1) {
    vertices[from].out_edges_.emplace(to, weight);
  }

  void add_bidir_edge(size_type v1, size_type v2, size_type weight = 1) {
    add_edge(v1, v2, weight);
    add_edge(v2, v1, weight);
  }

  size_type num_vertices() const noexcept { return vertices.size(); }

  const std::map<size_type, size_type>& out_edges(size_type vertex_id) const {
    return vertices[vertex_id].out_edges_;
  }

  size_type out_degree(size_type vertex_id) const noexcept {
    return out_edges(vertex_id).size();
  }

  static size_type null_vertex() noexcept {
      return -1;
  }

 private:
  std::vector<vertex_info> vertices;
};

}  // namespace djp

#endif  // Header guard
