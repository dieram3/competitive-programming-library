//          Copyright Diego Ramírez August 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <djp/graph/dag_shortest_paths.hpp>
#include <gtest/gtest.h>

#include <djp/graph/directed_graph.hpp>

#include <vector> // For std::vector

using namespace djp;
using std::vector;

static constexpr int inf = std::numeric_limits<int>::max();

TEST(DAGShortestPathsTest, SingleVertexTest) {
  directed_graph g(1);
  vector<int> weight, dist;
  dag_shortest_paths(g, 0, weight, dist);
  EXPECT_EQ(vector<int>({0}), dist);
}

TEST(DAGShortestPathsTest, AloneVerticesTest) {
  directed_graph g(6);
  vector<int> weight, dist;
  dag_shortest_paths(g, 0, weight, dist);
  EXPECT_EQ(vector<int>({0, inf, inf, inf, inf, inf}), dist);
  dag_shortest_paths(g, 3, weight, dist);
  EXPECT_EQ(vector<int>({inf, inf, inf, 0, inf, inf}), dist);
}

TEST(DAGShortestPathsTest, NonNegativeWeightedDAGTest) {
  directed_graph g(12);
  vector<int> weight;
  auto add_edge = [&](size_t src, size_t tgt, int w) {
    g.add_edge(src, tgt);
    weight.push_back(w);
  };
  add_edge(1, 6, 2);
  add_edge(2, 1, 6);
  add_edge(2, 3, 5);
  add_edge(2, 8, 3);
  add_edge(3, 6, 1);
  add_edge(4, 5, 3);
  add_edge(6, 0, 2);
  add_edge(6, 11, 3);
  add_edge(7, 5, 5);
  add_edge(8, 6, 4);
  add_edge(9, 2, 2);
  add_edge(10, 1, 4);

  ASSERT_EQ(12, g.num_edges());

  vector<int> dist = {1, 2, 3}; // Initial garbage.

  dag_shortest_paths(g, 9, weight, dist);
  EXPECT_EQ(vector<int>({10, 8, 2, 7, inf, inf, 8, inf, 5, 0, inf, 11}), dist);

  dag_shortest_paths(g, 10, weight, dist);
  EXPECT_EQ(vector<int>({8, 4, inf, inf, inf, inf, 6, inf, inf, inf, 0, 9}),
            dist);

  dag_shortest_paths(g, 4, weight, dist);
  EXPECT_EQ(
      vector<int>({inf, inf, inf, inf, 0, 3, inf, inf, inf, inf, inf, inf}),
      dist);

  dag_shortest_paths(g, 2, weight, dist);
  EXPECT_EQ(vector<int>({8, 6, 0, 5, inf, inf, 6, inf, 3, inf, inf, 9}), dist);

  dag_shortest_paths(g, 0, weight, dist);
  EXPECT_EQ(
      vector<int>({0, inf, inf, inf, inf, inf, inf, inf, inf, inf, inf, inf}),
      dist);
}

TEST(DAGShortestPathsTest, NegativeWeightedDAGTest) {
  directed_graph g(12);
  vector<int> weight;
  auto add_edge = [&](size_t src, size_t tgt, int w) {
    g.add_edge(src, tgt);
    weight.push_back(w);
  };
  add_edge(0, 2, 2);
  add_edge(3, 1, -11);
  add_edge(4, 0, -4);
  add_edge(4, 5, 5);
  add_edge(5, 0, -3);
  add_edge(6, 10, 5);
  add_edge(7, 3, 10);
  add_edge(7, 8, 3);
  add_edge(8, 4, 16);
  add_edge(8, 9, 2);
  add_edge(9, 3, 4);
  add_edge(9, 6, 6);
  add_edge(10, 2, -1);
  add_edge(11, 4, 3);

  ASSERT_EQ(14, g.num_edges());

  vector<int> dist(30, 112148); // Distance map with initial garbage.

  dag_shortest_paths(g, 11, weight, dist);

  EXPECT_EQ(vector<int>({-1, inf, 1, inf, 3, 8, inf, inf, inf, inf, inf, 0}),
            dist);

  dag_shortest_paths(g, 8, weight, dist);
  EXPECT_EQ(vector<int>({12, -5, 12, 6, 16, 21, 8, inf, 0, 2, 13, inf}), dist);

  dag_shortest_paths(g, 7, weight, dist);
  EXPECT_EQ(vector<int>({15, -2, 15, 9, 19, 24, 11, 0, 3, 5, 16, inf}), dist);
}

TEST(DAGShortestPathsTest, ParallelEdgesDAGTest) {
  directed_graph g(4);
  vector<int> weight;
  auto add_edge = [&](size_t src, size_t tgt, int w) {
    g.add_edge(src, tgt);
    weight.push_back(w);
  };
  add_edge(0, 1, 2);
  add_edge(0, 1, 4);
  add_edge(0, 2, 5);
  add_edge(0, 2, 3);
  add_edge(0, 2, 9);
  add_edge(0, 2, 7);
  add_edge(0, 2, 4);
  add_edge(1, 3, 4);
  add_edge(2, 3, 2);
  add_edge(2, 3, 3);
  add_edge(2, 3, 3);

  vector<int> dist;
  dag_shortest_paths(g, 0, weight, dist);
  EXPECT_EQ(vector<int>({0, 2, 3, 5}), dist);
}
