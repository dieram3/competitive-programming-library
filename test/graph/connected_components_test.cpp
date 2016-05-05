//          Copyright Diego Ramirez 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cpl/graph/connected_components.hpp>
#include <gtest/gtest.h>

#include <cpl/graph/undirected_graph.hpp> // undirected_graph
#include <algorithm>                      // max_element
#include <cstddef>                        // size_t
#include <vector>                         // vector

using cpl::connected_components;
using cpl::undirected_graph;
using std::size_t;

static void check_labels(const undirected_graph& graph,
                         const std::vector<size_t>& expected) {
  const size_t num_components =
      expected.empty()
          ? 0
          : *std::max_element(expected.begin(), expected.end()) + 1;

  std::vector<size_t> component_of;

  EXPECT_EQ(num_components, connected_components(graph, component_of));
  EXPECT_EQ(expected.size(), component_of.size());
  EXPECT_EQ(expected, component_of);
}

TEST(ConnectedComponentsTest, WorksOnEmptyGraphs) {
  undirected_graph graph(0);
  check_labels(graph, {});
}

TEST(ConnectedComponentsTest, WorksOnSingleVertexGraphs) {
  undirected_graph graph(1);
  check_labels(graph, {0});
}

TEST(ConnectedComponentsTest, WorksOnTotallyDisconnectedGraphs) {
  undirected_graph graph(10);
  check_labels(graph, {0, 1, 2, 3, 4, 5, 6, 7, 8, 9});
}

TEST(ConnectedComponentsTest, WorksOnSingleComponentGraphs) {
  undirected_graph graph(6);
  graph.add_edge(0, 1);
  graph.add_edge(1, 2);
  graph.add_edge(3, 4);
  graph.add_edge(4, 5);
  graph.add_edge(1, 4);
  graph.add_edge(2, 5);

  check_labels(graph, {0, 0, 0, 0, 0, 0});
}

TEST(ConnectedComponentsTest, WorksOnMultilpleComponentGraphs) {
  undirected_graph graph(14);
  // Component 0
  graph.add_edge(4, 0);
  graph.add_edge(4, 9);
  graph.add_edge(4, 8);

  // Component 1
  graph.add_edge(1, 2);
  graph.add_edge(2, 3);
  graph.add_edge(3, 1);

  // Component 2
  graph.add_edge(5, 12);
  graph.add_edge(5, 7);
  graph.add_edge(5, 10);
  graph.add_edge(5, 11);
  graph.add_edge(10, 12);
  graph.add_edge(12, 11);
  graph.add_edge(11, 7);
  graph.add_edge(7, 10);

  // Component 3
  // 6 is alone and has no edge.

  // Component 4
  graph.add_edge(13, 13);
  graph.add_edge(13, 13);

  check_labels(graph, {0, 1, 1, 1, 0, 2, 3, 2, 0, 0, 2, 2, 2, 4});
}
