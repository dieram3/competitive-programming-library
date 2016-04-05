//          Copyright Diego Ramírez July 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cpl/graph/bellman_ford_shortest_paths.hpp>
#include <gtest/gtest.h>

#include <cpl/graph/directed_graph.hpp> // for cpl::directed_graph
#include <vector>                       // for std::vector
using namespace cpl;

using dist_vec = std::vector<long>;
using weight_vec = std::vector<long>;

TEST(BellmanFordShortestPathsTest, WorksOnSingleVertexGraphs) {
  directed_graph graph(1);

  dist_vec dist;
  weight_vec weight;

  auto add_edge = [&](size_t s, size_t t, long w) {
    graph.add_edge(s, t);
    weight.push_back(w);
  };

  EXPECT_TRUE(bellman_ford_shortest_paths(graph, 0, weight, dist));

  add_edge(0, 0, 0);
  add_edge(0, 0, 1);
  EXPECT_TRUE(bellman_ford_shortest_paths(graph, 0, weight, dist));
  EXPECT_EQ(dist_vec{0}, dist);

  add_edge(0, 0, -1);
  EXPECT_FALSE(bellman_ford_shortest_paths(graph, 0, weight, dist));
}

TEST(BellmanFordShortestPathsTest, WorksOnPositiveEdgeWeightGraphs) {
  directed_graph graph(11);
  weight_vec weight_of;

  auto add_bidir_edge = [&](size_t u, size_t v, long weight) {
    graph.add_edge(u, v);
    weight_of.push_back(weight);
    graph.add_edge(v, u);
    weight_of.push_back(weight);
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

  dist_vec dist;
  EXPECT_TRUE(bellman_ford_shortest_paths(graph, 3, weight_of, dist));
  EXPECT_EQ(dist_vec({10, 7, 4, 0, 9, 11, 16, 11, 9, 11, 13}), dist);
}

TEST(BellmanFordShortestPathsTest, DetectsNegativeCyclesOnTrivialCases) {
  directed_graph graph(5);
  graph.add_edge(0, 1);
  graph.add_edge(1, 2);
  graph.add_edge(2, 3);
  graph.add_edge(3, 4);
  const size_t last_edge = graph.add_edge(4, 0);
  weight_vec weight_of = {2, 4, 3, 1, -11};

  dist_vec dist;
  EXPECT_FALSE(bellman_ford_shortest_paths(graph, 1, weight_of, dist));

  weight_of[last_edge] += 1;
  EXPECT_TRUE(bellman_ford_shortest_paths(graph, 1, weight_of, dist));
  EXPECT_EQ(dist_vec({-2, 0, 4, 7, 8}), dist);
}

TEST(BellmanFordShortestPathsTest, DetectsNegativeCyclesOnComplexCases) {
  directed_graph graph(7);
  weight_vec weight_of;
  auto add_edge = [&](size_t s, size_t t, long w) {
    graph.add_edge(s, t);
    weight_of.push_back(w);
    return weight_of.size() - 1; // Edge id
  };
  add_edge(0, 1, 2);
  add_edge(1, 2, 4);
  add_edge(2, 3, 3);
  add_edge(2, 5, -4);
  add_edge(3, 4, 1);
  add_edge(3, 6, -20);
  add_edge(4, 0, -10);
  add_edge(6, 4, 21);

  dist_vec dist;
  EXPECT_TRUE(bellman_ford_shortest_paths(graph, 1, weight_of, dist));
  EXPECT_EQ(dist_vec({-2, 0, 4, 7, 8, 0, -13}), dist);

  const size_t edge13 = add_edge(1, 3, 7);
  EXPECT_TRUE(bellman_ford_shortest_paths(graph, 1, weight_of, dist));
  weight_of[edge13] -= 1;
  EXPECT_FALSE(bellman_ford_shortest_paths(graph, 1, weight_of, dist));

  EXPECT_TRUE(bellman_ford_shortest_paths(graph, 5, weight_of, dist));
  const auto inf = std::numeric_limits<long>::max();
  EXPECT_EQ(dist_vec({inf, inf, inf, inf, inf, 0, inf}), dist);
}
