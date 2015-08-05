//          Copyright Diego Ramírez August 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DJP_FLOW_NETWORK_HPP
#define DJP_FLOW_NETWORK_HPP

#include <deque>       // for std::deque
#include <type_traits> // for std::is_signed, std::is_floating_point
#include <vector>      // for std::vector
#include <cstddef>     // for std::size_t

namespace djp {

/// \brief Convenience class to represent flow networks.
///
/// This class is directed graph with the right methods to model a FlowNetwork.
///
template <typename Flow>
class flow_network {
  static_assert(std::is_signed<Flow>::value ||
                    std::is_floating_point<Flow>::value,
                "The flow type must be signed or floating point.");

public: // Types
  struct edge {
    size_t source;
    size_t target;
    edge *rev_edge;
    Flow capacity;
    mutable Flow flow;
  };
  struct vertex {
    std::vector<edge *> out_edges;
  };

public:
  flow_network(size_t num_vertices) : vertex_list(num_vertices) {}

  size_t num_vertices() const { return vertex_list.size(); }
  const std::deque<edge> &edges() const { return edge_list; }

  const std::vector<edge *> &out_edges(size_t v) const {
    return vertex_list[v].out_edges;
  }

  void add_edge(size_t source, size_t target, Flow cap, Flow rev_cap = 0) {
    auto &edge1 = add_raw_edge(source, target, cap);
    auto &edge2 = add_raw_edge(target, source, rev_cap);
    edge1.rev_edge = &edge2;
    edge2.rev_edge = &edge1;
  }
  void add_bidir_edge(size_t u, size_t v, Flow cap) {
    add_edge(u, v, cap, cap);
  }

private:
  edge &add_raw_edge(size_t source, size_t target, Flow cap) {
    edge_list.push_back({source, target, nullptr, cap, 0});
    vertex_list[source].out_edges.push_back(&edge_list.back());
    return edge_list.back();
  }

private:
  std::vector<vertex> vertex_list;
  std::deque<edge> edge_list;
};

} // end namespace djp

#endif // Header guard
