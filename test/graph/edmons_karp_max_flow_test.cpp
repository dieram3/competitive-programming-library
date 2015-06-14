//          Copyright Diego Ramírez June 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <djp/graph/edmons_karp_max_flow.hpp>
#include <gtest/gtest.h>

#include <djp/graph/adjacency_list.hpp>

using namespace djp;

namespace {
struct edge_t {
  long capacity;
};

} // end anonymous namespace

using graph_t = adjacency_list<edge_t>;

TEST(edmons_karp_max_flow, WorksWell) {
  graph_t g(4);
  g.add_edge(0, 1).capacity = 20;
  g.add_edge(1, 0).capacity = 20;
  g.add_edge(0, 2).capacity = 10;
  g.add_edge(1, 2).capacity = 5;
  g.add_edge(1, 3).capacity = 10;
  g.add_edge(2, 3).capacity = 20;
  g.add_edge(3, 2).capacity = 15;

  EXPECT_EQ(20, edmonds_karp_max_flow(g, 0, 1));
  EXPECT_EQ(25, edmonds_karp_max_flow(g, 0, 2));
  EXPECT_EQ(25, edmonds_karp_max_flow(g, 0, 3));

  EXPECT_EQ(20, edmonds_karp_max_flow(g, 1, 0));
  EXPECT_EQ(25, edmonds_karp_max_flow(g, 1, 2));
  EXPECT_EQ(25, edmonds_karp_max_flow(g, 1, 3));

  EXPECT_EQ(0, edmonds_karp_max_flow(g, 2, 0));
  EXPECT_EQ(0, edmonds_karp_max_flow(g, 2, 1));
  EXPECT_EQ(20, edmonds_karp_max_flow(g, 2, 3));

  EXPECT_EQ(0, edmonds_karp_max_flow(g, 3, 0));
  EXPECT_EQ(0, edmonds_karp_max_flow(g, 3, 1));
  EXPECT_EQ(15, edmonds_karp_max_flow(g, 3, 2));
}
