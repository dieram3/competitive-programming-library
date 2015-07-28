//          Copyright Diego Ramírez July 2014, July 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DJP_GRAPH_ADJACENCY_LIST_HPP
#define DJP_GRAPH_ADJACENCY_LIST_HPP

#include <deque>       // for std::deque
#include <vector>      // for std::vector
#include <type_traits> // for std::conditional, std::is_class

namespace djp {

template <class EdgeData = void, class VertexData = void>
class adjacency_list {

  struct no_data {};

public: // Types
  using vertex_id = std::size_t;

  using edge_data = typename std::conditional<std::is_class<EdgeData>::value,
                                              EdgeData, no_data>::type;
  using vertex_data =
      typename std::conditional<std::is_class<VertexData>::value, VertexData,
                                no_data>::type;

  struct edge : public edge_data {
    edge(const edge &) = delete; // Prevent accidental copy.
    edge(vertex_id src, vertex_id tgt) : source{src}, target{tgt} {}
    vertex_id source;
    vertex_id target;
  };

  struct vertex : public vertex_data {
    vertex() = default;
    vertex(const vertex &) = delete; // Prevent accidental copy.
    std::vector<edge *> out_edges;
    std::vector<edge *> in_edges;
  };

  using vertex_list = std::vector<vertex>;
  using edge_list = std::deque<edge>;

public: // Essential Member functions
  adjacency_list(std::size_t num_vertices) : vertices_(num_vertices) {}

  edge_data &add_edge(vertex_id source, vertex_id target) {
    edges_.emplace_back(source, target);
    vertices_[source].out_edges.push_back(&edges_.back());
    vertices_[target].in_edges.push_back(&edges_.back());
    return edges_.back();
  }

  void add_bidir_edge(vertex_id v1, vertex_id v2, const edge_data &ed = {}) {
    add_edge(v1, v2) = ed;
    add_edge(v2, v1) = ed;
  }

  std::size_t num_vertices() const { return vertices_.size(); }
  std::size_t num_edges() const { return edges_.size(); }

  const vertex_list &vertices() const { return vertices_; }
  const edge_list &edges() const { return edges_; }

  static constexpr vertex_id null_vertex() noexcept { return -1; }

public: // Helper functions
  const std::vector<edge *> &out_edges(vertex_id v) const {
    return vertices_[v].out_edges;
  }

  const std::vector<edge *> &in_edges(vertex_id v) const {
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
