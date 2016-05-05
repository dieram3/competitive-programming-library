//          Copyright Diego Ramirez 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cpl/graph/hopcroft_karp_maximum_matching.hpp>
#include <gtest/gtest.h>

#include <cpl/graph/undirected_graph.hpp> // undirected_graph

using cpl::undirected_graph;
using cpl::hopcroft_karp_maximum_matching;

TEST(HopcroftKarpMaximumMatchingTest, WorksOnDisconnectedGraphs) {
  undirected_graph graph(20);
  EXPECT_EQ(0, hopcroft_karp_maximum_matching(graph));
}

TEST(HopcroftKarpMaximumMatchingTest, WorksOnTrivialCases) {
  undirected_graph graph(15);
  graph.add_edge(0, 7);
  graph.add_edge(1, 5);
  graph.add_edge(2, 8);
  graph.add_edge(3, 9);
  graph.add_edge(4, 6);
  graph.add_edge(12, 13);
  EXPECT_EQ(6, hopcroft_karp_maximum_matching(graph));
}

TEST(HopcroftKarpMaximumMatchingTest, WorksOnEasyCases) {
  undirected_graph graph(9);
  // Component 1
  graph.add_edge(0, 5);
  graph.add_edge(1, 5);
  graph.add_edge(1, 6);
  graph.add_edge(2, 6);

  // Component 2
  graph.add_edge(3, 7);
  graph.add_edge(3, 8);
  graph.add_edge(4, 7);
  graph.add_edge(4, 8);

  EXPECT_EQ(4, hopcroft_karp_maximum_matching(graph));
}

TEST(HopcroftKarpMaximumMatchingTest, WorksOnComplexCases) {
  undirected_graph graph(10);

  graph.add_edge(0, 5);
  graph.add_edge(0, 6);
  graph.add_edge(1, 5);
  graph.add_edge(1, 9);
  graph.add_edge(2, 7);
  graph.add_edge(2, 8);
  graph.add_edge(3, 5);
  graph.add_edge(3, 9);
  graph.add_edge(4, 6);
  graph.add_edge(4, 8);

  EXPECT_EQ(5, hopcroft_karp_maximum_matching(graph));
}

TEST(HopcroftKarpMaximumMatchingTest, WorksOnLinearGraphs) {
  undirected_graph graph(9);
  graph.add_edge(0, 1);
  graph.add_edge(1, 2);
  graph.add_edge(2, 3);
  graph.add_edge(3, 4);
  graph.add_edge(4, 5);
  graph.add_edge(5, 6);
  graph.add_edge(6, 7);
  graph.add_edge(7, 8);

  EXPECT_EQ(4, hopcroft_karp_maximum_matching(graph));
}

TEST(HopcroftKarpMaximumMatchingTest, WorksForSingleOptionPairs) {
  undirected_graph graph(10);
  graph.add_edge(0, 5);
  graph.add_edge(0, 6);
  graph.add_edge(0, 7);
  graph.add_edge(1, 5);
  graph.add_edge(1, 6);
  graph.add_edge(1, 7);
  graph.add_edge(1, 8);
  graph.add_edge(1, 9);
  graph.add_edge(2, 6);
  graph.add_edge(3, 7);
  graph.add_edge(4, 5);
  graph.add_edge(4, 8);
  graph.add_edge(4, 9);
  EXPECT_EQ(5, hopcroft_karp_maximum_matching(graph));
}
