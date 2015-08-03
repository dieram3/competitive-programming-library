//          Copyright Diego Ramírez July 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <djp/graph/bellman_ford_shortest_paths.hpp>
#include <gtest/gtest.h>

#include <djp/graph/directed_graph.hpp> // for djp::directed_graph
#include <vector>                       // for std::vector
using namespace djp;

namespace {
struct edge_data {
  long weight;
};
} // end anonymous namespace

using digraph_t = directed_graph<edge_data>;
using dist_t = std::vector<long>;

TEST(BellmanFordShortestPaths, WorksOnSingleVertexGraphs) {
  dist_t dist;
  digraph_t graph(1);

  EXPECT_TRUE(bellman_ford_shortest_paths(graph, 0, dist));

  graph.add_edge(0, 0).weight = 0;
  graph.add_edge(0, 0).weight = 1;
  EXPECT_TRUE(bellman_ford_shortest_paths(graph, 0, dist));
  EXPECT_EQ(dist_t{0}, dist);

  graph.add_edge(0, 0).weight = -1;
  EXPECT_FALSE(bellman_ford_shortest_paths(graph, 0, dist));
}

TEST(BellmanFordShortestPaths, WorksOnPositiveEdgeWeightGraphs) {
  digraph_t graph(11);

  auto add_bidir_edge = [&graph](size_t u, size_t v, long weight) {
    graph.add_edge(u, v).weight = weight;
    graph.add_edge(v, u).weight = weight;
  };

  add_bidir_edge(0, 1, 3);

  add_bidir_edge(1, 2, 3);
  add_bidir_edge(1, 4, 2);

  add_bidir_edge(2, 3, 4);
  add_bidir_edge(2, 8, 5);

  add_bidir_edge(4, 5, 2);
  add_bidir_edge(4, 7, 4);

  add_bidir_edge(5, 10, 3);

  add_bidir_edge(6, 9, 5);
  add_bidir_edge(6, 10, 3);

  add_bidir_edge(7, 8, 2);
  add_bidir_edge(7, 9, 2);

  add_bidir_edge(8, 9, 2);
  add_bidir_edge(9, 10, 2);

  dist_t dist;
  EXPECT_TRUE(bellman_ford_shortest_paths(graph, 3, dist));
  EXPECT_EQ(dist_t({10, 7, 4, 0, 9, 11, 16, 11, 9, 11, 13}), dist);
}

TEST(BellmanFordShortestPaths, DetectsNegativeCyclesOnTrivialCases) {
  digraph_t graph(5);
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
  digraph_t graph(7);
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
