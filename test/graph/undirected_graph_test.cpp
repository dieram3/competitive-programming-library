//          Copyright Diego Ramírez August 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <djp/graph/undirected_graph.hpp>
#include <gtest/gtest.h>

#include <algorithm> // for std::any_of
#include <iterator>  // for std::begin, std::end
using namespace djp;
using std::size_t;

TEST(UndirectedGraphTest, ConstructWell) {
  undirected_graph graph(26);
  EXPECT_EQ(26, graph.num_vertices());
  EXPECT_EQ(0, graph.num_edges());

  for (size_t v = 0; v != graph.num_vertices(); ++v) {
    EXPECT_EQ(0, graph.out_degree(v));
    EXPECT_EQ(0, graph.in_degree(v));
  }
}

TEST(UndirectedGraphTest, LinksWell) {

  undirected_graph g(5);

  auto connects_to = [&g](size_t u, size_t v) {
    auto &u_edges = g.out_edges(u);
    return std::any_of(begin(u_edges), end(u_edges), [&g, v](size_t e) {
      return g.source(e) == v || g.target(e) == v;
    });
  };

  g.add_edge(3, 4);
  g.add_edge(2, 1);
  g.add_edge(0, 1);
  g.add_edge(4, 3);
  g.add_edge(0, 2);

  EXPECT_TRUE(connects_to(3, 4));
  EXPECT_TRUE(connects_to(4, 3));

  EXPECT_TRUE(connects_to(2, 1));
  EXPECT_TRUE(connects_to(1, 2));

  EXPECT_TRUE(connects_to(0, 1));
  EXPECT_TRUE(connects_to(1, 0));

  EXPECT_TRUE(connects_to(4, 3));
  EXPECT_TRUE(connects_to(3, 4));

  EXPECT_TRUE(connects_to(2, 0));
  EXPECT_TRUE(connects_to(0, 2));
}
