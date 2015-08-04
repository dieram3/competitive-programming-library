//          Copyright Diego Ramírez August 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <djp/graph/undirected_graph.hpp>
#include <gtest/gtest.h>

#include <algorithm> // for std::any_of
#include <iterator>  // for std::begin, std::end
using namespace djp;
using std::size_t;

static_assert(undirected_graph<>::null_vertex() + 1 == 0,
              "Null vertex has corrupt value");

TEST(UndirectedGraphTest, ConstructWell) {
  undirected_graph<> graph(26);
  EXPECT_EQ(26, graph.num_vertices());
  EXPECT_EQ(0, graph.num_edges());

  for (size_t v = 0; v != graph.num_vertices(); ++v) {
    EXPECT_EQ(0, graph.out_degree(v));
    EXPECT_EQ(0, graph.in_degree(v));
  }
}

TEST(UndirectedGraphTest, LinksWell) {
  struct edge_data {
    long weight = 5;
  };

  undirected_graph<edge_data> graph(5);
  using edge_pointer = const decltype(graph)::edge *;

  auto connects_to = [&graph](size_t u, size_t v, long w) {
    auto &u_edges = graph.out_edges(u);
    return std::any_of(begin(u_edges), end(u_edges), [u, v, w](edge_pointer e) {
      return e->get_neighbor(u) == v && e->weight == w;
    });
  };

  graph.add_edge(3, 4);
  graph.add_edge(2, 1).weight = -1;
  graph.add_edge(0, 1).weight = 2;
  graph.add_edge(4, 3).weight = 0;
  graph.add_edge(0, 2).weight = 4;

  EXPECT_TRUE(connects_to(3, 4, 5L));
  EXPECT_TRUE(connects_to(4, 3, 5L));

  EXPECT_TRUE(connects_to(2, 1, -1L));
  EXPECT_TRUE(connects_to(1, 2, -1L));

  EXPECT_TRUE(connects_to(0, 1, 2L));
  EXPECT_TRUE(connects_to(1, 0, 2L));

  EXPECT_TRUE(connects_to(4, 3, 0L));
  EXPECT_TRUE(connects_to(3, 4, 0L));

  EXPECT_TRUE(connects_to(2, 0, 4L));
  EXPECT_TRUE(connects_to(0, 2, 4L));
}
