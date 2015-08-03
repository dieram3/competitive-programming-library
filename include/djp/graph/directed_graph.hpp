//          Copyright Diego Ramírez July 2014, July-August 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DJP_GRAPH_DIRECTED_GRAPH_HPP
#define DJP_GRAPH_DIRECTED_GRAPH_HPP

#include <deque>       // for std::deque
#include <type_traits> // for std::conditional, std::is_class
#include <vector>      // for std::vector

namespace djp {

template <class EdgeData = void, class VertexData = void>
class directed_graph {

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
    std::vector<const edge *> out_edges;
    std::vector<const edge *> in_edges;
  };

public: // Essential Member functions
  directed_graph(std::size_t num_vertices) : vertex_list(num_vertices) {}

  edge_data &add_edge(vertex_id source, vertex_id target) {
    edge_list.emplace_back(source, target);
    vertex_list[source].out_edges.push_back(&edge_list.back());
    vertex_list[target].in_edges.push_back(&edge_list.back());
    return edge_list.back();
  }

  std::size_t num_vertices() const { return vertex_list.size(); }
  std::size_t num_edges() const { return edge_list.size(); }

  const std::vector<vertex> &vertices() const { return vertex_list; }
  const std::deque<edge> &edges() const { return edge_list; }

  static constexpr vertex_id null_vertex() noexcept { return -1; }

public: // Helper functions
  const std::vector<const edge *> &out_edges(vertex_id v) const {
    return vertex_list[v].out_edges;
  }
  const std::vector<const edge *> &in_edges(vertex_id v) const {
    return vertex_list[v].in_edges;
  }
  std::size_t out_degree(vertex_id v) const { return out_edges(v).size(); }
  std::size_t in_degree(vertex_id v) const { return in_edges(v).size(); }

private:
  std::vector<vertex> vertex_list;
  std::deque<edge> edge_list;
};

} // namespace djp

#endif // Header guard
