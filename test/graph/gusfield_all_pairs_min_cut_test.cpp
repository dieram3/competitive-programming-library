//          Copyright Diego Ramírez August 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cpl/graph/gusfield_all_pairs_min_cut.hpp>
#include <gtest/gtest.h>

#include <cpl/graph/directed_graph.hpp>
#include <cpl/utility/matrix.hpp>

using namespace cpl;

TEST(GusfieldAllPairsMinCutTest, SmallGraphTest) {
  directed_graph graph(6);
  std::vector<size_t> rev_edge;
  std::vector<long> capacity;

  auto add_edge = [&](size_t u, size_t v, long cap) {
    const auto e0 = graph.add_edge(u, v);
    const auto e1 = graph.add_edge(v, u);
    rev_edge.push_back(e1);
    rev_edge.push_back(e0);
    capacity.push_back(cap);
    capacity.push_back(cap);
  };

  add_edge(0, 1, 1);
  add_edge(0, 2, 7);
  add_edge(1, 2, 1);
  add_edge(1, 3, 3);
  add_edge(1, 4, 2);
  add_edge(2, 4, 4);
  add_edge(3, 4, 1);
  add_edge(3, 5, 6);
  add_edge(4, 5, 2);

  const auto cut = gusfield_all_pairs_min_cut(graph, rev_edge, capacity);
  ASSERT_EQ(6, cut.rows());
  ASSERT_EQ(6, cut.cols());

  for (size_t i = 0; i != cut.rows(); ++i)
    for (size_t j = i + 1; j != cut.cols(); ++j)
      EXPECT_TRUE((cut[{i, j}] == cut[{j, i}])) << " at (" << i << ", " << j
                                                << ")";

  EXPECT_EQ(6, (cut[{0, 1}]));
  EXPECT_EQ(8, (cut[{0, 2}]));
  EXPECT_EQ(6, (cut[{0, 3}]));
  EXPECT_EQ(6, (cut[{0, 4}]));
  EXPECT_EQ(6, (cut[{0, 5}]));

  EXPECT_EQ(6, (cut[{1, 2}]));
  EXPECT_EQ(6, (cut[{1, 3}]));
  EXPECT_EQ(7, (cut[{1, 4}]));
  EXPECT_EQ(6, (cut[{1, 5}]));

  EXPECT_EQ(6, (cut[{2, 3}]));
  EXPECT_EQ(6, (cut[{2, 4}]));
  EXPECT_EQ(6, (cut[{2, 5}]));

  EXPECT_EQ(6, (cut[{3, 4}]));
  EXPECT_EQ(8, (cut[{3, 5}]));

  EXPECT_EQ(6, (cut[{4, 5}]));
}
