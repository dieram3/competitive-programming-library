//          Copyright Diego Ramírez August 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <djp/graph/flow_network.hpp>
#include <gtest/gtest.h>

#include <tuple> // for std::tuple

using namespace djp;

using flow_t = long;
using digraph_t = flow_network<flow_t>;
using tuple_edge = std::tuple<size_t, size_t, flow_t>;

static tuple_edge as_tuple(const digraph_t::edge &edge) {
  return tuple_edge(edge.source, edge.target, edge.capacity);
}

TEST(FlowNetworkTest, NumVerticesTest) {
  digraph_t graph1(5);
  EXPECT_EQ(5, graph1.num_vertices());
  digraph_t graph2(10);
  graph2.add_bidir_edge(2, 4, 10);
  EXPECT_EQ(10, graph2.num_vertices());
}

TEST(FlowNetworkTest, AddEdgeTest) {
  digraph_t graph(5);
  graph.add_edge(0, 1, 3);
  graph.add_edge(0, 2, 5);
  graph.add_edge(1, 3, 6);
  graph.add_edge(2, 3, 3);
  graph.add_edge(2, 4, 4, 5);
  graph.add_edge(3, 4, 2, 3);

  const auto &edges = graph.edges();
  ASSERT_EQ(2 * 6, edges.size());

  for (size_t i = 0; i != edges.size(); i += 2) {
    EXPECT_TRUE(edges[i].rev_edge == &edges[i + 1]) << "for i " << i;
    EXPECT_TRUE(edges[i + 1].rev_edge == &edges[i]) << "for i " << i;
  }

  EXPECT_EQ(tuple_edge(0, 1, 3), as_tuple(edges[0]));
  EXPECT_EQ(tuple_edge(1, 0, 0), as_tuple(edges[1]));
  EXPECT_EQ(tuple_edge(0, 2, 5), as_tuple(edges[2]));
  EXPECT_EQ(tuple_edge(2, 0, 0), as_tuple(edges[3]));
  EXPECT_EQ(tuple_edge(1, 3, 6), as_tuple(edges[4]));
  EXPECT_EQ(tuple_edge(3, 1, 0), as_tuple(edges[5]));
  EXPECT_EQ(tuple_edge(2, 3, 3), as_tuple(edges[6]));
  EXPECT_EQ(tuple_edge(3, 2, 0), as_tuple(edges[7]));
  EXPECT_EQ(tuple_edge(2, 4, 4), as_tuple(edges[8]));
  EXPECT_EQ(tuple_edge(4, 2, 5), as_tuple(edges[9]));
  EXPECT_EQ(tuple_edge(3, 4, 2), as_tuple(edges[10]));
  EXPECT_EQ(tuple_edge(4, 3, 3), as_tuple(edges[11]));
}

TEST(FlowNetworkTest, AddBidirEdgeTest) {
  digraph_t graph(5);
  graph.add_bidir_edge(0, 1, 9);
  graph.add_bidir_edge(0, 2, 5);
  graph.add_bidir_edge(0, 3, 1);
  graph.add_bidir_edge(1, 3, 8);
  graph.add_bidir_edge(2, 3, 2);
  graph.add_bidir_edge(2, 4, 7);
  graph.add_bidir_edge(3, 4, 4);

  const auto &edges = graph.edges();
  ASSERT_EQ(2 * 7, edges.size());

  for (size_t i = 0; i != edges.size(); i += 2) {
    EXPECT_TRUE(edges[i].rev_edge == &edges[i + 1]) << "for i " << i;
    EXPECT_TRUE(edges[i + 1].rev_edge == &edges[i]) << "for i " << i;
  }

  EXPECT_EQ(tuple_edge(0, 1, 9), as_tuple(edges[0]));
  EXPECT_EQ(tuple_edge(1, 0, 9), as_tuple(edges[1]));
  EXPECT_EQ(tuple_edge(0, 2, 5), as_tuple(edges[2]));
  EXPECT_EQ(tuple_edge(2, 0, 5), as_tuple(edges[3]));
  EXPECT_EQ(tuple_edge(0, 3, 1), as_tuple(edges[4]));
  EXPECT_EQ(tuple_edge(3, 0, 1), as_tuple(edges[5]));
  EXPECT_EQ(tuple_edge(1, 3, 8), as_tuple(edges[6]));
  EXPECT_EQ(tuple_edge(3, 1, 8), as_tuple(edges[7]));
  EXPECT_EQ(tuple_edge(2, 3, 2), as_tuple(edges[8]));
  EXPECT_EQ(tuple_edge(3, 2, 2), as_tuple(edges[9]));
  EXPECT_EQ(tuple_edge(2, 4, 7), as_tuple(edges[10]));
  EXPECT_EQ(tuple_edge(4, 2, 7), as_tuple(edges[11]));
  EXPECT_EQ(tuple_edge(3, 4, 4), as_tuple(edges[12]));
  EXPECT_EQ(tuple_edge(4, 3, 4), as_tuple(edges[13]));
}

TEST(FlowNetworkTest, OutEdgesTest) {
  digraph_t graph(4);
  graph.add_edge(0, 1, 1);
  graph.add_edge(0, 2, 1);
  graph.add_edge(1, 3, 1);
  graph.add_edge(2, 3, 1);

  const auto &edges = graph.edges();

  const auto &out_edges0 = graph.out_edges(0);
  const auto &out_edges1 = graph.out_edges(1);
  const auto &out_edges2 = graph.out_edges(2);
  const auto &out_edges3 = graph.out_edges(3);

  ASSERT_EQ(2, out_edges0.size());
  ASSERT_EQ(2, out_edges1.size());
  ASSERT_EQ(2, out_edges2.size());
  ASSERT_EQ(2, out_edges2.size());

  EXPECT_TRUE(out_edges0[0] == &edges[0]);
  EXPECT_TRUE(out_edges1[0] == &edges[1]);
  EXPECT_TRUE(out_edges0[1] == &edges[2]);
  EXPECT_TRUE(out_edges2[0] == &edges[3]);
  EXPECT_TRUE(out_edges1[1] == &edges[4]);
  EXPECT_TRUE(out_edges3[0] == &edges[5]);
  EXPECT_TRUE(out_edges2[1] == &edges[6]);
  EXPECT_TRUE(out_edges3[1] == &edges[7]);
}
