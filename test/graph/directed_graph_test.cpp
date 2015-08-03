//          Copyright Diego Ramírez July 2014, August 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <djp/graph/directed_graph.hpp>
#include <gtest/gtest.h>

#include <cstddef> // for std::size_t

using namespace djp;

static_assert(directed_graph<>::null_vertex() + 1 == 0,
              "Null vertex has corrupt value");

TEST(DirectedGraph, ConstructWell) {
  directed_graph<> graph(15);
  EXPECT_EQ(15u, graph.num_vertices());
  EXPECT_EQ(0u, graph.num_edges());

  for (size_t v = 0; v < graph.num_vertices(); ++v) {
    EXPECT_EQ(0u, graph.out_degree(v));
  }
}

TEST(DirectedGraph, LinksWell) {
  directed_graph<> graph(5);

  auto check_in = [&graph](std::size_t u, std::size_t v) {
    for (const auto &edge : graph.in_edges(v)) {
      if (edge->source == u)
        return true;
    }
    return false;
  };

  auto check_out = [&graph](std::size_t u, std::size_t v) {
    for (const auto &edge : graph.out_edges(u)) {
      if (edge->target == v)
        return true;
    }
    return false;
  };

  auto connects_to = [&](std::size_t u, std::size_t v) {
    if (check_in(u, v)) {
      EXPECT_TRUE(check_out(u, v));
      return true;
    }
    EXPECT_FALSE(check_out(u, v));
    return false;
  };

  graph.add_edge(3, 4);
  graph.add_edge(2, 1);
  graph.add_edge(0, 1);
  graph.add_edge(4, 3);
  graph.add_edge(0, 2);
  graph.add_edge(2, 0);

  EXPECT_TRUE(connects_to(3, 4));
  EXPECT_TRUE(connects_to(2, 1));
  EXPECT_TRUE(connects_to(0, 1));
  EXPECT_TRUE(connects_to(4, 3));
  EXPECT_TRUE(connects_to(0, 2));
  EXPECT_TRUE(connects_to(2, 0));

  EXPECT_FALSE(connects_to(1, 2));
  EXPECT_FALSE(connects_to(1, 0));
  EXPECT_FALSE(connects_to(1, 4));
  EXPECT_FALSE(connects_to(3, 1));
  EXPECT_FALSE(connects_to(1, 3));
}
