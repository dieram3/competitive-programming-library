//          Copyright Diego Ramírez July 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DJP_GRAPH_ADJACENCY_LIST_HPP
#define DJP_GRAPH_ADJACENCY_LIST_HPP

#include <vector> // for std::vector
#include <list>   // for std::list

namespace djp {

struct no_data {};

template <class EdgeData = no_data, class VertexData = no_data>
class adjacency_list {

public: // Types
  using vertex_id = std::size_t;

  struct edge : public EdgeData {
    edge(const edge &) = delete; // Prevent accidental copy.
    edge(vertex_id src, vertex_id tgt) : source{src}, target{tgt} {}
    vertex_id source;
    vertex_id target;
  };

  using edge_list = std::list<edge>;
  using edge_iterator = typename edge_list::const_iterator;
  using conn_edge_list = std::vector<edge_iterator>; // connected edges

  struct vertex : public VertexData {
    vertex() = default;
    vertex(const vertex &) = delete; // Prevent accidental copy.
    conn_edge_list out_edges;
    conn_edge_list in_edges;
  };

  using vertex_list = std::vector<vertex>;

public: // Essential Member functions
  adjacency_list(std::size_t num_vertices) : vertices_(num_vertices) {}

  EdgeData &add_edge(vertex_id source, vertex_id target) {
    edges_.emplace_front(source, target);
    vertices_[source].out_edges.push_back(edges_.cbegin());
    vertices_[target].in_edges.push_back(edges_.cbegin());
    return edges_.front();
  }

  void add_bidir_edge(vertex_id v1, vertex_id v2, const EdgeData &ed = {}) {
    add_edge(v1, v2) = ed;
    add_edge(v2, v1) = ed;
  }

  std::size_t num_vertices() const { return vertices_.size(); }
  std::size_t num_edges() const { return edges_.size(); }

  const vertex_list &vertices() const { return vertices_; }
  const edge_list &edges() const { return edges_; }

  static constexpr vertex_id null_vertex() noexcept { return -1; }

public: // Helper functions
  const conn_edge_list &out_edges(vertex_id v) const {
    return vertices_[v].out_edges;
  }

  const conn_edge_list &in_edges(vertex_id v) const {
    return vertices_[v].in_edges;
  }

  std::size_t out_degree(vertex_id v) const { return out_edges(v).size(); }
  std::size_t in_degree(vertex_id v) const { return in_edges(v).size(); }

private:
  vertex_list vertices_;
  edge_list edges_;
};

} // namespace djp

#endif // Header guard
