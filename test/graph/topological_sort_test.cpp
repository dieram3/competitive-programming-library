//          Copyright Diego Ramírez July 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <djp/graph/topological_sort.hpp>
#include <gtest/gtest.h>

#include <djp/graph/directed_graph.hpp> // for djp::directed_graph

#include <algorithm>  // for std::find
#include <functional> // for std::greater
#include <stdexcept>  // for std::logic_error
#include <vector>     // for std::vector

using namespace djp;
using std::vector;

static void check_toposort(const directed_graph &g) {
  vector<bool> processed(g.num_vertices());
  const auto sorted_list = topological_sort(g);

  for (const size_t v : sorted_list) {
    EXPECT_FALSE(processed[v]);
    processed[v] = true;

    for (const auto edge : g.in_edges(v))
      EXPECT_TRUE(processed[g.source(edge)]) << v << ' ' << edge;
  }

  EXPECT_EQ(processed.end(),
            std::find(processed.begin(), processed.end(), false));
}

TEST(TopologicalSortTest, WorksOnMultipleSolution) {
  directed_graph graph(8);
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
  directed_graph graph(7);
  graph.add_edge(4, 0);
  graph.add_edge(1, 0);
  graph.add_edge(6, 5);
  graph.add_edge(6, 2);

  check_toposort(graph);
}

TEST(TopologicalSortTest, WorksOnUniqueSolution) {
  directed_graph graph(4);
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
  directed_graph graph(4);
  graph.add_edge(0, 1);
  graph.add_edge(1, 2);
  graph.add_edge(2, 1);
  graph.add_edge(2, 3);

  EXPECT_THROW(topological_sort(graph), std::logic_error);
}

TEST(TopologicalSortTest, ThrowsIfNotADag) {
  directed_graph graph(4);
  graph.add_edge(3, 0);
  graph.add_edge(0, 1);
  graph.add_edge(1, 2);
  graph.add_edge(2, 0);
  EXPECT_THROW(topological_sort(graph), std::logic_error);
}

static vector<size_t>
smallest_numbered_first_toposort(const directed_graph &g) {
  vector<size_t> list;
  list.reserve(g.num_vertices());
  prioritized_topological_sort(g, std::greater<size_t>(),
                               [&](size_t v) { list.push_back(v); });
  return list;
}

TEST(PrioritizedTopologicalSortTest, EmptyGraphTest) {
  directed_graph g(0);
  EXPECT_EQ(vector<size_t>(), smallest_numbered_first_toposort(g));
}

TEST(PrioritizedTopologicalSortTest, ConnectedDAGTest) {
  directed_graph g(8);
  g.add_edge(1, 4);
  g.add_edge(1, 6);
  g.add_edge(2, 7);
  g.add_edge(3, 4);
  g.add_edge(3, 7);
  g.add_edge(4, 5);
  g.add_edge(7, 0);
  g.add_edge(7, 5);
  g.add_edge(7, 6);

  ASSERT_EQ(9, g.num_edges());
  EXPECT_EQ(vector<size_t>({1, 2, 3, 4, 7, 0, 5, 6}),
            smallest_numbered_first_toposort(g));
}

TEST(PrioritizedTopologicalSortTest, DisconnectedDAGTest) {
  directed_graph g(10);
  g.add_edge(1, 4);
  g.add_edge(1, 6);
  g.add_edge(4, 6);
  g.add_edge(4, 8);
  g.add_edge(5, 8);
  g.add_edge(7, 4);
  g.add_edge(8, 6);
  g.add_edge(9, 0);

  ASSERT_EQ(8, g.num_edges());
  EXPECT_EQ(vector<size_t>({1, 2, 3, 5, 7, 4, 8, 6, 9, 0}),
            smallest_numbered_first_toposort(g));
}

TEST(PrioritizedTopologicalSortTest, NotDagTest) {
  directed_graph g(8);
  g.add_edge(0, 1);
  g.add_edge(1, 2);
  g.add_edge(1, 3);

  g.add_edge(4, 5);
  g.add_edge(5, 6);
  g.add_edge(6, 7);
  g.add_edge(7, 4);

  EXPECT_GT(g.num_vertices(), smallest_numbered_first_toposort(g).size());
}
