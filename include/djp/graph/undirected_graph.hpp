//          Copyright Diego Ramírez August 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DJP_GRAPH_UNDIRECTED_GRAPH_HPP
#define DJP_GRAPH_UNDIRECTED_GRAPH_HPP

#include <deque>       // for std::deque
#include <type_traits> // for std::conditional, std::is_clas
#include <vector>      // for std::vector
#include <cstddef>     // for std::size_t

namespace djp {

template <class EdgeData = void, class VertexData = void>
class undirected_graph {

  struct no_data {};

  template <typename T>
  using class_or_nothing =
      typename std::conditional<std::is_class<T>::value, T, no_data>::type;

public: // Types
  using vertex_id = std::size_t;
  using edge_data = class_or_nothing<EdgeData>;
  using vertex_data = class_or_nothing<VertexData>;

  struct edge : public edge_data {
    edge(const edge &) = delete; // Prevent accidental copy.
    edge(vertex_id src, vertex_id tgt) : source{src}, target{tgt} {}
    vertex_id source;
    vertex_id target;

    vertex_id get_neighbor(vertex_id my_endpoint) const {
      return my_endpoint == source ? target : source;
    }
  };

  struct vertex : public vertex_data {
    vertex() = default;
    vertex(const vertex &) = delete; // Prevent accidental copy.
    std::vector<edge *> adj_edges;
  };

public: // Essential Member functions
  undirected_graph(std::size_t num_vertices) : vertex_list(num_vertices) {}

  edge_data &add_edge(vertex_id source, vertex_id target) {
    edge_list.emplace_back(source, target);
    vertex_list[source].adj_edges.push_back(&edge_list.back());
    vertex_list[target].adj_edges.push_back(&edge_list.back());
    return edge_list.back();
  }

  std::size_t num_vertices() const { return vertex_list.size(); }
  std::size_t num_edges() const { return edge_list.size(); }

  const std::vector<vertex> &vertices() const { return vertex_list; }
  const std::deque<edge> &edges() const { return edge_list; }

  static constexpr vertex_id null_vertex() noexcept { return -1; }

public: // Helper functions
  const std::vector<edge *> &out_edges(vertex_id v) const {
    return vertex_list[v].adj_edges;
  }
  const std::vector<edge *> &in_edges(vertex_id v) const {
    return out_edges(v);
  }
  std::size_t out_degree(vertex_id v) const { return out_edges(v).size(); }
  std::size_t in_degree(vertex_id v) const { return out_degree(v); }

private:
  std::vector<vertex> vertex_list;
  std::deque<edge> edge_list;
};

} // end namespace djp

#endif // Header guard
