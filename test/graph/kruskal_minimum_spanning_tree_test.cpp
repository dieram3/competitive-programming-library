//          Copyright Diego Ramírez July 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <djp/graph/kruskal_minimum_spanning_tree.hpp>
#include <gtest/gtest.h>

#include <djp/graph/adjacency_list.hpp>
using namespace djp;

struct edge_data {
  int weight;
};

/// \todo Use a real undirected graph.
using undirected_graph = adjacency_list<edge_data>;

TEST(kruskal_minimum_spanning_tree, WorksOnEmptyGraph) {
  undirected_graph graph(0);
  EXPECT_EQ(0, kruskal_minimum_spanning_tree(graph).size());
}

TEST(kruskal_minimum_spanning_tree, WorksOnSingleVertexGraph) {
  undirected_graph graph(1);
  EXPECT_EQ(0, kruskal_minimum_spanning_tree(graph).size());
}

TEST(kruskal_minimum_spanning_tree, WorksOnTrees) {
  undirected_graph graph(6);
  graph.add_edge(0, 1).weight = -1; // 0
  graph.add_edge(3, 1).weight = 5;  // 1
  graph.add_edge(1, 2).weight = 10; // 2
  graph.add_edge(5, 4).weight = -4; // 3
  graph.add_edge(4, 2).weight = 0;  // 4

  const auto mst_edges = kruskal_minimum_spanning_tree(graph);

  ASSERT_EQ(5, mst_edges.size());

  EXPECT_EQ(-4, mst_edges[0]->weight);
  EXPECT_EQ(5, mst_edges[0]->source);
  EXPECT_EQ(4, mst_edges[0]->target);

  EXPECT_EQ(-1, mst_edges[1]->weight);
  EXPECT_EQ(0, mst_edges[1]->source);
  EXPECT_EQ(1, mst_edges[1]->target);

  EXPECT_EQ(0, mst_edges[2]->weight);
  EXPECT_EQ(4, mst_edges[2]->source);
  EXPECT_EQ(2, mst_edges[2]->target);

  EXPECT_EQ(5, mst_edges[3]->weight);
  EXPECT_EQ(3, mst_edges[3]->source);
  EXPECT_EQ(1, mst_edges[3]->target);

  EXPECT_EQ(10, mst_edges[4]->weight);
  EXPECT_EQ(1, mst_edges[4]->source);
  EXPECT_EQ(2, mst_edges[4]->target);
}

TEST(kruskal_minimum_spanning_tree, WorksWell) {
  undirected_graph graph(5);
  graph.add_edge(0, 2).weight = 8;
  graph.add_edge(2, 3).weight = 5;
  graph.add_edge(3, 0).weight = 6;
  graph.add_edge(1, 3).weight = 3;
  graph.add_edge(1, 2).weight = 2;
  graph.add_edge(2, 4).weight = 7;
  graph.add_edge(1, 4).weight = 4;

  const auto mst_edges = kruskal_minimum_spanning_tree(graph);

  ASSERT_EQ(4, mst_edges.size());

  EXPECT_EQ(2, mst_edges[0]->weight);
  EXPECT_EQ(1, mst_edges[0]->source);
  EXPECT_EQ(2, mst_edges[0]->target);

  EXPECT_EQ(3, mst_edges[1]->weight);
  EXPECT_EQ(1, mst_edges[1]->source);
  EXPECT_EQ(3, mst_edges[1]->target);

  EXPECT_EQ(4, mst_edges[2]->weight);
  EXPECT_EQ(1, mst_edges[2]->source);
  EXPECT_EQ(4, mst_edges[2]->target);

  EXPECT_EQ(6, mst_edges[3]->weight);
  EXPECT_EQ(3, mst_edges[3]->source);
  EXPECT_EQ(0, mst_edges[3]->target);
}
