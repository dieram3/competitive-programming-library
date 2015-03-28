//          Copyright Diego Ramírez July 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DJP_GRAPH_ADJACENCY_LIST_HPP
#define DJP_GRAPH_ADJACENCY_LIST_HPP

#include <vector>  // for std::vector
#include <list>    // for std::list

namespace djp {

class adjacency_list {
 public:
  using size_type = std::size_t;

  struct edge {
    size_type source;
    size_type target;
    long weight;
  };

  using edge_list = std::list<edge>;
  using edge_iterator = edge_list::iterator;
  using out_edges_list = std::vector<edge_iterator>;

  struct vertex {
    out_edges_list out_edges;
  };

  using vertex_list = std::vector<vertex>;

  adjacency_list(size_type nvertices) : vertices(nvertices) {}

  void add_edge(size_type source, size_type target, long weight = 1) {
    edges.push_front({source, target, weight});
    vertices[source].out_edges.push_back(edges.begin());
  }

  void add_bidir_edge(size_type v1, size_type v2, long weight = 1) {
    add_edge(v1, v2, weight);
    add_edge(v2, v1, weight);
  }

  size_type num_vertices() const noexcept { return vertices.size(); }

  const out_edges_list& out_edges(size_type vertex_id) const {
    return vertices[vertex_id].out_edges;
  }

  size_type out_degree(size_type vertex_id) const noexcept {
    return out_edges(vertex_id).size();
  }

  static size_type null_vertex() noexcept { return -1; }

 private:
  vertex_list vertices;
  edge_list edges;
};

}  // namespace djp

#endif  // Header guard
