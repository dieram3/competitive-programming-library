#include <djp/graph/adjacency_list.hpp>
#include <gtest/gtest.h>
#include <cstddef> // for std::size_t

// This is a small test
// Who really test the adjacency list are all the graph algorithms.

static_assert(djp::adjacency_list<>::null_vertex() + 1 == 0,
              "Null vertex has corrupt value");

TEST(adjacency_list, ConstructWell) {
  djp::adjacency_list<> graph(15);
  EXPECT_EQ(15u, graph.num_vertices());
  EXPECT_EQ(0u, graph.num_edges());

  for (size_t v = 0; v < graph.num_vertices(); ++v) {
    EXPECT_EQ(0u, graph.out_degree(v));
  }
}

TEST(adjacency_list, LinksWell) {
  djp::adjacency_list<> graph(5);

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
  graph.add_bidir_edge(0, 2);

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
