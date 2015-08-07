//          Copyright Diego Ramírez August 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <djp/graph/min_st_cut.hpp>
#include <gtest/gtest.h>

#include <djp/graph/directed_graph.hpp>

#include <vector>  // for std::vector
#include <cstddef> // for std::size_t

using namespace djp;

using bool_vec = std::vector<bool>;

static std::vector<size_t> get_cut_set(const directed_graph &g,
                                       bool_vec &source_side) {
  std::vector<size_t> cut_set;
  const size_t num_edges = g.num_edges();
  // even edges are originals, odd edges are the transposed.
  for (size_t edge = 0; edge != num_edges; edge += 2) {
    if (source_side[g.source(edge)] && !source_side[g.target(edge)])
      cut_set.emplace_back(edge);
  }
  return cut_set;
}

TEST(MinSTCutTest, WorksForSmallCases) {
  directed_graph graph(6);
  std::vector<size_t> rev_edge;
  std::vector<long> capacity;

  auto add_edge = [&](size_t s, size_t t, long cap) {
    const auto e1 = graph.add_edge(s, t);
    const auto e2 = graph.add_edge(t, s);
    rev_edge.push_back(e2);
    rev_edge.push_back(e1);
    capacity.push_back(cap);
    capacity.push_back(0);
  };

  add_edge(0, 1, 16);
  add_edge(0, 2, 13);
  add_edge(1, 2, 10);
  add_edge(1, 3, 12); // cut 1, 4th even edge
  add_edge(2, 1, 4);
  add_edge(2, 4, 14);
  add_edge(3, 2, 9);
  add_edge(3, 5, 20);
  add_edge(4, 3, 7); // cut 2, 9th even edge
  add_edge(4, 5, 4); // cut 3, 10th even edge.

  bool_vec source_side;
  EXPECT_EQ(23, min_st_cut(graph, 0, 5, rev_edge, capacity, source_side));
  ASSERT_EQ(bool_vec({1, 1, 1, 0, 1, 0}), source_side);

  const auto cut_set = get_cut_set(graph, source_side);

  ASSERT_EQ(3, cut_set.size());

  EXPECT_EQ(2 * 3, cut_set[0]);
  EXPECT_EQ(2 * 8, cut_set[1]);
  EXPECT_EQ(2 * 9, cut_set[2]);
}

TEST(MinSTCutTest, BidirGraphTest) {
  directed_graph graph(13);
  std::vector<size_t> rev_edge;
  std::vector<long> capacity;

  auto add_bidir_edge = [&](size_t u, size_t v, long cap) {
    const auto e0 = graph.add_edge(u, v);
    const auto e1 = graph.add_edge(v, u);
    rev_edge.push_back(e1);
    rev_edge.push_back(e0);
    capacity.push_back(cap);
    capacity.push_back(cap);
  };

  add_bidir_edge(0, 1, 2);
  add_bidir_edge(0, 2, 2);
  add_bidir_edge(1, 3, 1);
  add_bidir_edge(1, 4, 1);
  add_bidir_edge(2, 3, 1);
  add_bidir_edge(2, 5, 1);
  add_bidir_edge(2, 6, 2);
  add_bidir_edge(3, 4, 1);
  add_bidir_edge(3, 6, 1);
  add_bidir_edge(4, 7, 1); // cut edge, 10th even edge
  add_bidir_edge(5, 8, 1);
  add_bidir_edge(6, 9, 1);
  add_bidir_edge(7, 10, 2);
  add_bidir_edge(7, 12, 2);
  add_bidir_edge(8, 9, 2);
  add_bidir_edge(9, 10, 1); // cut edge, 16th even edge
  add_bidir_edge(10, 11, 2);
  add_bidir_edge(11, 12, 2);

  ASSERT_EQ(18 * 2, graph.num_edges());

  bool_vec source_side;
  EXPECT_EQ(2, min_st_cut(graph, 0, 11, rev_edge, capacity, source_side));
  EXPECT_EQ(bool_vec({1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 0, 0}), source_side);

  const auto cut_set = get_cut_set(graph, source_side);

  ASSERT_EQ(2, cut_set.size());
  EXPECT_EQ(2 * 9, cut_set[0]);
  EXPECT_EQ(2 * 15, cut_set[1]);
}
