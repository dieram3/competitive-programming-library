//          Copyright Diego Ramírez July 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <djp/graph/dijkstra_shortest_paths.hpp>
#include <gtest/gtest.h>
#include <djp/graph/adjacency_list.hpp>
#include <vector>
#include <random>
#include <numeric>
#include <functional>
#include <cassert>

namespace {

// Complexity: O(V*avg_degree)
// It can generate parallel edges.
// You can visualize the generated graph on http://g.ivank.net/

djp::adjacency_list make_random_graph(size_t num_vertices,
                                      double out_degree_mean) {
  assert(num_vertices != 0);

  djp::adjacency_list graph(num_vertices);

  std::random_device rd;
  std::default_random_engine gen1(rd());
  std::mt19937 gen2(rd());
  std::default_random_engine gen3(rd());

  std::poisson_distribution<size_t> degree_dist(out_degree_mean);
  std::uniform_int_distribution<size_t> dst_dist(0, num_vertices - 1);
  std::poisson_distribution<size_t> weight_dist(5.0);

  auto gen_degree = std::bind(degree_dist, std::ref(gen1));
  auto gen_dst = std::bind(dst_dist, std::ref(gen2));
  auto gen_weight = std::bind(weight_dist, std::ref(gen3));

  for (size_t src = 0; src < num_vertices; ++src) {
    size_t degree = gen_degree();
    while (degree--)
      graph.add_edge(src, gen_dst(), gen_weight());
  }

  return graph;
}

} // Anonymous namesapce

TEST(dijkstra_shortest_paths, WorksWithAdjacencyList) {
  djp::adjacency_list graph(6);

  graph.add_bidir_edge(0, 1, 7);
  graph.add_bidir_edge(0, 2, 9);
  graph.add_bidir_edge(0, 5, 14);
  graph.add_bidir_edge(1, 2, 10);
  graph.add_bidir_edge(1, 3, 15);
  graph.add_bidir_edge(2, 3, 11);
  graph.add_bidir_edge(2, 5, 2);
  graph.add_bidir_edge(3, 4, 6);
  graph.add_bidir_edge(4, 5, 9);

  auto spaths = djp::dijkstra_shortest_paths(graph, 0);

  std::vector<size_t> expected = {0, 7, 9, 20, 20, 11};

  EXPECT_EQ(expected, spaths);
}

TEST(dijkstra_shortest_paths, WorksOnBigGraphs) {
  const size_t num_vertices = 10000;
  const double out_degree_mean = 5.0;
  const auto graph = make_random_graph(num_vertices, out_degree_mean);

  auto dist_v1 = djp::v1::dijkstra_shortest_paths(graph, 0);
  auto dist_v2 = djp::v2::dijkstra_shortest_paths(graph, 0);

  EXPECT_EQ(dist_v1, dist_v2);
}
