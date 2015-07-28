//          Copyright Diego Ramírez July 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <djp/graph/connected_components.hpp>
#include <gtest/gtest.h>

#include <djp/graph/adjacency_list.hpp>
#include <algorithm> // for std::max_element
#include <vector>    // for std::vector
using namespace djp;

template <typename Graph>
void check_labels(const Graph &graph, const std::vector<size_t> &expected) {
  const size_t num_components =
      expected.empty()
          ? 0
          : *std::max_element(expected.begin(), expected.end()) + 1;

  EXPECT_EQ(num_components, connected_components(graph));

  std::vector<size_t> labels;
  EXPECT_EQ(num_components, connected_components(graph, &labels));

  ASSERT_EQ(expected.size(), labels.size());

  EXPECT_EQ(expected, labels);
}

TEST(connected_components, WorksOnEmptyGraphs) {
  adjacency_list<> graph(0);
  check_labels(graph, {});
}

TEST(connected_components, WorksOnSingleVertexGraphs) {
  adjacency_list<> graph(1);
  check_labels(graph, {0});
}

TEST(connected_components, WorksOnTotallyDisconnectedGraphs) {
  adjacency_list<> graph(10);
  check_labels(graph, {0, 1, 2, 3, 4, 5, 6, 7, 8, 9});
}

TEST(connected_components, WorksOnSingleComponentGraphs) {
  adjacency_list<> graph(6);
  graph.add_bidir_edge(0, 1);
  graph.add_bidir_edge(1, 2);
  graph.add_bidir_edge(3, 4);
  graph.add_bidir_edge(4, 5);
  graph.add_bidir_edge(1, 4);
  graph.add_bidir_edge(2, 5);

  check_labels(graph, {0, 0, 0, 0, 0, 0});
}

TEST(connected_components, WorksOnMultilpleComponentGraphs) {
  adjacency_list<> graph(14);
  // Component 0
  graph.add_bidir_edge(4, 0);
  graph.add_bidir_edge(4, 9);
  graph.add_bidir_edge(4, 8);

  // Component 1
  graph.add_bidir_edge(1, 2);
  graph.add_bidir_edge(2, 3);
  graph.add_bidir_edge(3, 1);

  // Component 2
  graph.add_bidir_edge(5, 12);
  graph.add_bidir_edge(5, 7);
  graph.add_bidir_edge(5, 10);
  graph.add_bidir_edge(5, 11);
  graph.add_bidir_edge(10, 12);
  graph.add_bidir_edge(12, 11);
  graph.add_bidir_edge(11, 7);
  graph.add_bidir_edge(7, 10);

  // Component 3
  // 6 is alone and has no edge.

  // Component 4
  graph.add_bidir_edge(13, 13);
  graph.add_bidir_edge(13, 13);

  check_labels(graph, {0, 1, 1, 1, 0, 2, 3, 2, 0, 0, 2, 2, 2, 4});
}
