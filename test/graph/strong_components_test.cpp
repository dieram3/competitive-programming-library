//          Copyright Diego Ramírez August 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <djp/graph/strong_components.hpp>
#include <gtest/gtest.h>

#include <djp/graph/directed_graph.hpp>

#include <unordered_map> // for std::unordered_map
#include <vector>        // for std::vector
#include <cstddef>       // for std::size_t

using namespace djp;
using std::vector;

static void normalize(vector<size_t> &vec) {
  std::unordered_map<size_t, size_t> id_map;
  for (const size_t elem : vec)
    id_map.emplace(elem, id_map.size());
  for (size_t &elem : vec)
    elem = id_map.at(elem);
}

// Requires: expected.size() >= 1
static void check_scc(const directed_graph &g, const size_t total_scc,
                      const vector<size_t> &expected) {
  vector<size_t> comp;
  EXPECT_EQ(total_scc, strong_components(g, comp));
  for (const size_t label : comp)
    EXPECT_LT(label, total_scc) << " weird label: " << label;

  normalize(comp);
  EXPECT_EQ(expected, comp);
}

TEST(StrongComponentsTest, EmptyGraphTest) {
  directed_graph g(0);
  check_scc(g, 0, {});
}

TEST(StrongComponentsTest, SingleVertexGraphTest) {
  directed_graph g(1);
  check_scc(g, 1, {0});
  g.add_edge(0, 0);
  g.add_edge(0, 0);
  check_scc(g, 1, {0});
}

TEST(StrongComponentsTest, AloneVerticesTest) {
  directed_graph g(5);
  g.add_edge(0, 0);
  g.add_edge(3, 3);
  g.add_edge(3, 3);
  check_scc(g, 5, {0, 1, 2, 3, 4});
}

TEST(StrongComponentsTest, NonStrongConnectedGraphTest) {
  directed_graph g(8);

  g.add_edge(0, 4);
  g.add_edge(1, 0);
  g.add_edge(2, 1);
  g.add_edge(2, 3);
  g.add_edge(3, 2);
  g.add_edge(4, 1);
  g.add_edge(5, 1);
  g.add_edge(5, 4);
  g.add_edge(5, 6);
  g.add_edge(6, 2);
  g.add_edge(6, 5);
  g.add_edge(7, 3);
  g.add_edge(7, 6);
  g.add_edge(7, 7);

  EXPECT_EQ(14, g.num_edges());
  check_scc(g, 4, {0, 0, 1, 1, 0, 2, 2, 3});
}

TEST(StrongComponentsTest, StrongConnectedGraphTest) {
  directed_graph g(9);
  g.add_edge(0, 1);
  g.add_edge(1, 1);
  g.add_edge(1, 3);
  g.add_edge(2, 0);
  g.add_edge(3, 6);
  g.add_edge(4, 2);
  g.add_edge(4, 3);
  g.add_edge(5, 4);
  g.add_edge(6, 5);
  g.add_edge(6, 8);
  g.add_edge(7, 6);
  g.add_edge(8, 7);

  EXPECT_EQ(12, g.num_edges());
  check_scc(g, 1, {0, 0, 0, 0, 0, 0, 0, 0, 0});
}

TEST(StrongComponentsTest, DirectedAcyclicGraphTest) {
  directed_graph g(14);
  g.add_edge(0, 2);
  g.add_edge(0, 3);
  g.add_edge(1, 4);
  g.add_edge(2, 5);
  g.add_edge(3, 5);
  g.add_edge(4, 6);
  g.add_edge(4, 7);
  g.add_edge(5, 8);
  g.add_edge(5, 9);
  g.add_edge(5, 10);
  g.add_edge(6, 10);
  g.add_edge(7, 6);
  g.add_edge(11, 12);
  g.add_edge(11, 13);

  EXPECT_EQ(14, g.num_edges());
  check_scc(g, 14, {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13});
}

TEST(StrongComponentsTest, PyramidalTest) {
  directed_graph g(7);
  g.add_edge(0, 1);
  g.add_edge(0, 6);
  g.add_edge(1, 2);
  g.add_edge(1, 5);
  g.add_edge(2, 3);
  g.add_edge(2, 4);
  g.add_edge(4, 2);
  g.add_edge(4, 3);
  g.add_edge(5, 1);
  g.add_edge(5, 4);
  g.add_edge(6, 0);
  g.add_edge(6, 5);

  EXPECT_EQ(12, g.num_edges());
  check_scc(g, 4, {0, 1, 2, 3, 2, 1, 0});
}

TEST(StrongComponentsTest, BidirectionalTreeTest) {
  directed_graph g(5);
  auto add_edge = [&](size_t u, size_t v) {
    g.add_edge(u, v);
    g.add_edge(v, u);
  };
  add_edge(0, 1);
  add_edge(1, 3);
  add_edge(2, 3);
  add_edge(3, 4);
  EXPECT_EQ(8, g.num_edges());
  check_scc(g, 1, {0, 0, 0, 0, 0});
}
