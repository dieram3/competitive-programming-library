//          Copyright Diego Ramírez July 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <djp/graph/kruskal_minimum_spanning_tree.hpp>
#include <gtest/gtest.h>

#include <djp/graph/undirected_graph.hpp>
using namespace djp;
using std::vector;

TEST(KruskalMinimumSpanningTreeTest, WorksOnEmptyGraph) {
  undirected_graph graph(0);
  vector<unsigned> weight;
  EXPECT_EQ(0, kruskal_minimum_spanning_tree(graph, weight).size());
}

TEST(KruskalMinimumSpanningTreeTest, WorksOnSingleVertexGraph) {
  undirected_graph graph(1);
  vector<unsigned> weight;
  EXPECT_EQ(0, kruskal_minimum_spanning_tree(graph, weight).size());
}

TEST(KruskalMinimumSpanningTreeTest, WorksOnTrees) {
  undirected_graph graph(6);
  vector<int> weight_of;
  auto add_edge = [&](size_t u, size_t v, int weight) {
    graph.add_edge(u, v);
    weight_of.push_back(weight);
  };
  add_edge(0, 1, -1); // 0
  add_edge(3, 1, 5);  // 1
  add_edge(1, 2, 10); // 2
  add_edge(5, 4, -4); // 3
  add_edge(4, 2, 0);  // 4

  const auto mst_edges = kruskal_minimum_spanning_tree(graph, weight_of);

  EXPECT_EQ(5, mst_edges.size());
  EXPECT_EQ(vector<size_t>({3, 0, 4, 1, 2}), mst_edges);
}

TEST(KruskalMinimumSpanningTreeTest, WorksOnCyclicGraphs) {
  undirected_graph graph(5);
  vector<int> weight_of;
  auto add_edge = [&](size_t u, size_t v, int weight) {
    graph.add_edge(u, v);
    weight_of.push_back(weight);
  };
  add_edge(0, 2, 8); // 0
  add_edge(2, 3, 5); // 1
  add_edge(3, 0, 6); // 2
  add_edge(1, 3, 3); // 3
  add_edge(1, 2, 2); // 4
  add_edge(2, 4, 7); // 5
  add_edge(1, 4, 4); // 6

  const auto mst_edges = kruskal_minimum_spanning_tree(graph, weight_of);

  ASSERT_EQ(4, mst_edges.size());

  EXPECT_EQ(vector<size_t>({4, 3, 6, 2}), mst_edges);
}
