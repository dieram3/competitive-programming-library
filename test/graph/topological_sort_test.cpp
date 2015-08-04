//          Copyright Diego Ramírez July 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <djp/graph/topological_sort.hpp>
#include <gtest/gtest.h>

#include <djp/graph/directed_graph.hpp> // for djp::directed_graph

#include <algorithm> // for std::find
#include <stdexcept> // for std::logic_error
#include <vector>    // for std::vector

using namespace djp;

using digraph_t = directed_graph<>;

static void check_toposort(const digraph_t &graph) {
  std::vector<bool> processed(graph.num_vertices());

  const auto sorted_list = topological_sort(graph);

  for (const size_t v : sorted_list) {
    EXPECT_FALSE(processed[v]);
    processed[v] = true;

    for (const auto *edge : graph.in_edges(v)) {
      const auto source = edge->source;
      EXPECT_TRUE(processed[source]);
    }
  }

  EXPECT_EQ(processed.end(),
            std::find(processed.begin(), processed.end(), false));
}

TEST(TopologicalSortTest, WorksOnMultipleSolution) {
  digraph_t graph(8);
  graph.add_edge(3, 7);
  graph.add_edge(3, 4);
  graph.add_edge(2, 7);
  graph.add_edge(1, 4);
  graph.add_edge(1, 6);
  graph.add_edge(7, 0);
  graph.add_edge(7, 5);
  graph.add_edge(4, 5);

  check_toposort(graph);
}

TEST(TopologicalSortTest, WorksOnSeparatedComponents) {
  digraph_t graph(7);
  graph.add_edge(4, 0);
  graph.add_edge(1, 0);
  graph.add_edge(6, 5);
  graph.add_edge(6, 2);

  check_toposort(graph);
}

TEST(TopologicalSortTest, WorksOnUniqueSolution) {
  digraph_t graph(4);
  graph.add_edge(3, 0);
  graph.add_edge(0, 2);
  graph.add_edge(2, 1);
  const auto sorted_list = topological_sort(graph);
  EXPECT_EQ(3, sorted_list[0]);
  EXPECT_EQ(0, sorted_list[1]);
  EXPECT_EQ(2, sorted_list[2]);
  EXPECT_EQ(1, sorted_list[3]);
}

TEST(TopologicalSortTest, DetectsBidirectionalEdges) {
  digraph_t graph(4);
  graph.add_edge(0, 1);
  graph.add_edge(1, 2);
  graph.add_edge(2, 1);
  graph.add_edge(2, 3);

  EXPECT_THROW(topological_sort(graph), std::logic_error);
}

TEST(TopologicalSortTest, ThrowsIfNotADag) {
  digraph_t graph(4);
  graph.add_edge(3, 0);
  graph.add_edge(0, 1);
  graph.add_edge(1, 2);
  graph.add_edge(2, 0);
  EXPECT_THROW(topological_sort(graph), std::logic_error);
}
