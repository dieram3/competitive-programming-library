//          Copyright Diego Ramírez August 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <djp/graph/min_st_cut.hpp>
#include <gtest/gtest.h>

#include <djp/graph/directed_graph.hpp>

#include <algorithm> // for std::sort
#include <tuple>     // for std::tuple
#include <vector>    // for std::vector
#include <cstddef>   // for std::size_t

using namespace djp;

namespace {
struct edge_data {
  mutable long flow;
  long capacity;
  edge_data *rev_edge;
};
}

using digraph_t = directed_graph<edge_data>;
using bool_vec = std::vector<bool>;
using edge_tuple = std::tuple<size_t, size_t, long>;

static std::vector<edge_tuple> get_cut_set(const digraph_t &graph,
                                           bool_vec &reachable) {
  std::vector<edge_tuple> cut_set;
  for (const auto &edge : graph.edges()) {
    if (edge.capacity == 0)
      continue; // It is a reversed edge.
    if (reachable[edge.source] && !reachable[edge.target])
      cut_set.emplace_back(edge.source, edge.target, edge.capacity);
  }
  std::sort(cut_set.begin(), cut_set.end());
  return cut_set;
}

TEST(MinSTCutTest, WorksForSmallCases) {
  digraph_t graph(6);
  auto add_edge = [&graph](size_t s, size_t t, long cap) {
    edmonds_karp_add_edge(graph, s, t, cap);
  };
  add_edge(0, 1, 16);
  add_edge(0, 2, 13);
  add_edge(1, 2, 10);
  add_edge(1, 3, 12); // cut 1
  add_edge(2, 1, 4);
  add_edge(2, 4, 14);
  add_edge(3, 2, 9);
  add_edge(3, 5, 20);
  add_edge(4, 3, 7); // cut 2
  add_edge(4, 5, 4); // cut 3

  bool_vec reachable;
  EXPECT_EQ(23, min_st_cut(graph, 0, 5, reachable));
  ASSERT_EQ(bool_vec({1, 1, 1, 0, 1, 0}), reachable);

  const auto cut_set = get_cut_set(graph, reachable);

  ASSERT_EQ(3, cut_set.size());

  EXPECT_EQ(edge_tuple(1, 3, 12), cut_set[0]);
  EXPECT_EQ(edge_tuple(4, 3, 7), cut_set[1]);
  EXPECT_EQ(edge_tuple(4, 5, 4), cut_set[2]);
}

TEST(MinSTCutTest, BidirGraphTest) {
  digraph_t graph(13);
  auto add_edge = [&graph](size_t u, size_t v, long cap) {
    edmonds_karp_add_edge(graph, u, v, cap, cap);
  };
  add_edge(0, 1, 2);
  add_edge(0, 2, 2);
  add_edge(1, 3, 1);
  add_edge(1, 4, 1);
  add_edge(2, 3, 1);
  add_edge(2, 5, 1);
  add_edge(2, 6, 2);
  add_edge(3, 4, 1);
  add_edge(3, 6, 1);
  add_edge(4, 7, 1);
  add_edge(5, 8, 1);
  add_edge(6, 9, 1);
  add_edge(7, 10, 2);
  add_edge(7, 12, 2);
  add_edge(8, 9, 2);
  add_edge(9, 10, 1);
  add_edge(10, 11, 2);
  add_edge(11, 12, 2);

  ASSERT_EQ(18 * 2, graph.num_edges());

  bool_vec reachable;
  EXPECT_EQ(2, min_st_cut(graph, 0, 11, reachable));
  EXPECT_EQ(bool_vec({1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 0, 0}), reachable);

  const auto cut_set = get_cut_set(graph, reachable);

  ASSERT_EQ(2, cut_set.size());
  EXPECT_EQ(edge_tuple(4, 7, 1), cut_set[0]);
  EXPECT_EQ(edge_tuple(9, 10, 1), cut_set[1]);
}
