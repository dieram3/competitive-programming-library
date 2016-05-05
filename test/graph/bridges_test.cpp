//          Copyright Diego Ramirez 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cpl/graph/bridges.hpp>
#include <gtest/gtest.h>

#include <cpl/graph/undirected_graph.hpp> // undirected_graph
#include <algorithm>                      // sort
#include <cstddef>                        // size_t
#include <vector>                         // vector

using cpl::find_bridges;
using cpl::undirected_graph;
using std::vector;
using std::size_t;

static vector<size_t> sorted_bridges(const undirected_graph& g) {
  vector<size_t> bridges;
  auto output_bridge = [&](const size_t e) { bridges.push_back(e); };
  find_bridges(g, output_bridge);
  std::sort(bridges.begin(), bridges.end());
  return bridges;
}

TEST(BridgesTest, EmptyGraphTest) {
  undirected_graph g(0);
  EXPECT_EQ(vector<size_t>(), sorted_bridges(g));
}

TEST(BridgesTest, SingleVertexGraphTest) {
  undirected_graph g(1);
  EXPECT_EQ(vector<size_t>(), sorted_bridges(g));
}

TEST(BridgesTest, AloneVerticesTest) {
  undirected_graph g(10);
  EXPECT_EQ(vector<size_t>(), sorted_bridges(g));
}

TEST(BridgesTest, TriangularGraphTest) {
  undirected_graph g(3);
  g.add_edge(0, 1);
  g.add_edge(0, 2);
  g.add_edge(1, 2);
  EXPECT_EQ(vector<size_t>(), sorted_bridges(g));
}

TEST(BridgesTest, LinearGraphTest) {
  undirected_graph g(4);
  g.add_edge(0, 1);
  g.add_edge(1, 2);
  g.add_edge(2, 3);
  EXPECT_EQ(vector<size_t>({0, 1, 2}), sorted_bridges(g));
}

TEST(BridgesTest, OneBridgeTest) {
  undirected_graph g(7);
  g.add_edge(0, 1);
  g.add_edge(0, 2);
  g.add_edge(1, 2);
  g.add_edge(1, 3);
  g.add_edge(1, 4);
  g.add_edge(1, 6); // edge 5 is the only bridge.
  g.add_edge(3, 5);
  g.add_edge(4, 5);

  ASSERT_EQ(8, g.num_edges());
  EXPECT_EQ(vector<size_t>({5}), sorted_bridges(g));
}

TEST(BridgesTest, TwoBridgesTest) {
  undirected_graph g(5);
  g.add_edge(0, 1);
  g.add_edge(0, 2);
  g.add_edge(0, 3); // edge 2 is bridge
  g.add_edge(1, 2);
  g.add_edge(3, 4); // edge 4 is bridge

  ASSERT_EQ(5, g.num_edges());
  EXPECT_EQ(vector<size_t>({2, 4}), sorted_bridges(g));
}

TEST(BridgesTest, SixBridgesTest) {
  undirected_graph g(16);
  g.add_edge(0, 1); // edge 0 is bridge
  g.add_edge(2, 3); // edge 1 is bridge
  g.add_edge(2, 6); // edge 2 is bridge
  g.add_edge(4, 8);
  g.add_edge(4, 9);
  g.add_edge(6, 7); // edge 5 is bridge
  g.add_edge(8, 9);
  g.add_edge(8, 13);
  g.add_edge(9, 10); // edge 8 bridge
  g.add_edge(9, 13);
  g.add_edge(10, 11);
  g.add_edge(10, 14);
  g.add_edge(11, 15);
  g.add_edge(12, 13); // edge 13 bridge
  g.add_edge(14, 15);

  ASSERT_EQ(15, g.num_edges());
  EXPECT_EQ(vector<size_t>({0, 1, 2, 5, 8, 13}), sorted_bridges(g));
}

TEST(BridgesTest, ForestTest) {
  undirected_graph g(20);
  // First tree
  g.add_edge(0, 1);
  g.add_edge(1, 2);
  g.add_edge(1, 3);
  g.add_edge(2, 4);
  // Second tree
  g.add_edge(5, 6);
  // Third tree is only 7
  // Fourth tree
  g.add_edge(8, 9);
  g.add_edge(8, 10);
  g.add_edge(10, 11);
  // Fifth tree
  g.add_edge(12, 13);
  g.add_edge(13, 14);
  g.add_edge(13, 15);
  g.add_edge(15, 16);
  g.add_edge(16, 17);
  g.add_edge(16, 18);
  g.add_edge(18, 19);

  ASSERT_EQ(15, g.num_edges());

  const vector<size_t> expected = {0, 1, 2,  3,  4,  5,  6, 7,
                                   8, 9, 10, 11, 12, 13, 14};

  EXPECT_EQ(expected, sorted_bridges(g));
}

TEST(BridgesTest, NoBridgeTest) {
  undirected_graph g(9);
  g.add_edge(0, 1);
  g.add_edge(0, 4);
  g.add_edge(1, 2);
  g.add_edge(1, 6);
  g.add_edge(2, 3);
  g.add_edge(3, 4);
  g.add_edge(3, 8);
  g.add_edge(5, 6);
  g.add_edge(5, 7);
  g.add_edge(5, 8);
  g.add_edge(6, 7);
  g.add_edge(7, 8);

  ASSERT_EQ(12, g.num_edges());
  EXPECT_EQ(vector<size_t>(), sorted_bridges(g));
}
