//          Copyright Diego Ramírez August 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <djp/graph/bipartite.hpp>
#include <gtest/gtest.h>

#include <djp/graph/undirected_graph.hpp>

#include <cstddef> // For std::size_t
#include <vector>  // For std::vector

using namespace djp;
using std::vector;

static void check_colors(const undirected_graph &g, const vector<bool> &color) {

  EXPECT_EQ(g.num_vertices(), color.size());
  for (size_t e = 0; e != g.num_edges(); ++e) {
    const size_t u = g.source(e);
    const size_t v = g.target(e);
    EXPECT_NE(color[u], color[v]) << "Neighbor vertices " << u << " and " << v
                                  << " should have different colors";
  }
}

static void check_bipartite(const undirected_graph &g) {
  vector<bool> color;
  EXPECT_TRUE(is_bipartite(g, color));
  check_colors(g, color);
}

TEST(IsBipartiteTest, EmptyGraphTest) {
  undirected_graph g(0);
  check_bipartite(g);
}

TEST(IsBipartiteTest, AloneVerticesTest) {
  undirected_graph g(6);
  check_bipartite(g);
}

TEST(IsBipartiteTest, ConnectedBipartiteGraphTest) {
  undirected_graph g(7);
  g.add_edge(0, 1);
  g.add_edge(0, 4);
  g.add_edge(1, 2);
  g.add_edge(1, 5);
  g.add_edge(2, 6);
  g.add_edge(3, 6);
  g.add_edge(4, 5);

  ASSERT_EQ(7, g.num_edges());
  check_bipartite(g);
}

TEST(IsBipartiteTest, DisconnectedBipartiteGraphTest) {
  undirected_graph g(8);
  g.add_edge(0, 1);
  g.add_edge(0, 7);
  g.add_edge(1, 6);
  g.add_edge(2, 4);
  g.add_edge(3, 4);
  g.add_edge(6, 7);

  ASSERT_EQ(6, g.num_edges());
  check_bipartite(g);
}

TEST(IsBipartiteTest, ConnectedNonBipartiteGraphTest) {
  undirected_graph g(6);
  g.add_edge(0, 1);
  g.add_edge(0, 2);
  g.add_edge(1, 3);
  g.add_edge(2, 3);
  g.add_edge(2, 4);
  g.add_edge(3, 4);
  g.add_edge(4, 5);

  ASSERT_EQ(7, g.num_edges());
  vector<bool> color;
  EXPECT_FALSE(is_bipartite(g, color));
}

TEST(IsBipartiteTest, DisconnectedNonBipartiteGraphTest) {
  undirected_graph g(11);
  g.add_edge(0, 1);
  g.add_edge(0, 3);
  g.add_edge(1, 2);
  g.add_edge(2, 3);
  g.add_edge(4, 7);
  g.add_edge(5, 6);
  g.add_edge(5, 8);
  g.add_edge(6, 8);

  ASSERT_EQ(8, g.num_edges());
  vector<bool> color;
  EXPECT_FALSE(is_bipartite(g, color));
}
