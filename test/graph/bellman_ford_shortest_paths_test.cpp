//          Copyright Diego Ramírez July 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <djp/graph/bellman_ford_shortest_paths.hpp>
#include <gtest/gtest.h>

#include <djp/graph/adjacency_list.hpp>
#include <vector> // for std::vector
using namespace djp;

struct edge_data {
  long weight;
};

using graph_t = adjacency_list<edge_data>;
using dist_t = std::vector<long>;

TEST(BellmanFordShortestPaths, WorksOnSingleVertexGraphs) {
  dist_t dist;
  graph_t graph(1);

  EXPECT_TRUE(bellman_ford_shortest_paths(graph, 0, dist));

  graph.add_edge(0, 0).weight = 0;
  graph.add_edge(0, 0).weight = 1;
  EXPECT_TRUE(bellman_ford_shortest_paths(graph, 0, dist));
  EXPECT_EQ(dist_t{0}, dist);

  graph.add_edge(0, 0).weight = -1;
  EXPECT_FALSE(bellman_ford_shortest_paths(graph, 0, dist));
}

TEST(BellmanFordShortestPaths, WorksOnPositiveEdgeWeightGraphs) {
  graph_t graph(11);

  graph.add_bidir_edge(0, 1, edge_data{3});

  graph.add_bidir_edge(1, 2, edge_data{3});
  graph.add_bidir_edge(1, 4, edge_data{2});

  graph.add_bidir_edge(2, 3, edge_data{4});
  graph.add_bidir_edge(2, 8, edge_data{5});

  graph.add_bidir_edge(4, 5, edge_data{2});
  graph.add_bidir_edge(4, 7, edge_data{4});

  graph.add_bidir_edge(5, 10, edge_data{3});

  graph.add_bidir_edge(6, 9, edge_data{5});
  graph.add_bidir_edge(6, 10, edge_data{3});

  graph.add_bidir_edge(7, 8, edge_data{2});
  graph.add_bidir_edge(7, 9, edge_data{2});

  graph.add_bidir_edge(8, 9, edge_data{2});
  graph.add_bidir_edge(9, 10, edge_data{2});

  dist_t dist;
  EXPECT_TRUE(bellman_ford_shortest_paths(graph, 3, dist));
  EXPECT_EQ(dist_t({10, 7, 4, 0, 9, 11, 16, 11, 9, 11, 13}), dist);
}

TEST(BellmanFordShortestPaths, DetectsNegativeCyclesOnTrivialCases) {
  graph_t graph(5);
  graph.add_edge(0, 1).weight = 2;
  graph.add_edge(1, 2).weight = 4;
  graph.add_edge(2, 3).weight = 3;
  graph.add_edge(3, 4).weight = 1;
  auto &w4_0 = graph.add_edge(4, 0).weight;
  w4_0 = -11;

  dist_t dist;
  EXPECT_FALSE(bellman_ford_shortest_paths(graph, 1, dist));

  w4_0 += 1;
  EXPECT_TRUE(bellman_ford_shortest_paths(graph, 1, dist));
  EXPECT_EQ(dist_t({-2, 0, 4, 7, 8}), dist);
}

TEST(BellmanFordShortestPaths, DetectsNegativeCyclesOnComplexCases) {
  graph_t graph(7);
  graph.add_edge(0, 1).weight = 2;
  graph.add_edge(1, 2).weight = 4;
  graph.add_edge(2, 3).weight = 3;
  graph.add_edge(2, 5).weight = -4;
  graph.add_edge(3, 4).weight = 1;
  graph.add_edge(3, 6).weight = -20;
  graph.add_edge(4, 0).weight = -10;
  graph.add_edge(6, 4).weight = 21;

  dist_t dist;
  EXPECT_TRUE(bellman_ford_shortest_paths(graph, 1, dist));
  EXPECT_EQ(dist_t({-2, 0, 4, 7, 8, 0, -13}), dist);

  auto &w1_3 = graph.add_edge(1, 3).weight;
  w1_3 = 7;
  EXPECT_TRUE(bellman_ford_shortest_paths(graph, 1, dist));
  w1_3 -= 1;
  EXPECT_FALSE(bellman_ford_shortest_paths(graph, 1, dist));

  EXPECT_TRUE(bellman_ford_shortest_paths(graph, 5, dist));
  const auto inf = dist[0];
  EXPECT_EQ(dist_t({inf, inf, inf, inf, inf, 0, inf}), dist);
}
