//          Copyright Diego Ramírez May 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <djp/graph/lowest_common_ancestor.hpp>
#include <djp/graph/adjacency_list.hpp>
#include <gtest/gtest.h>

#include <vector>

#include <cstddef>
#include <cassert>

TEST(rmq_lca, WorksOnSmallCases) {
  djp::adjacency_list<> tree(13);

  tree.add_edge(0, 1);
  tree.add_edge(0, 2);
  tree.add_edge(0, 3);
  tree.add_edge(2, 4);
  tree.add_edge(2, 5);
  tree.add_edge(2, 6);
  tree.add_edge(5, 7);
  tree.add_edge(5, 8);
  tree.add_edge(6, 9);
  tree.add_edge(6, 10);
  tree.add_edge(9, 11);
  tree.add_edge(9, 12);

  ASSERT_EQ(12u, tree.num_edges());

  auto children_of = [&tree](size_t v) {
    std::vector<size_t> children(tree.out_degree(v));
    size_t num_child = 0;
    for (auto edge : tree.out_edges(v)) {
      const size_t child = edge->target;
      children[num_child++] = child;
    }
    assert(num_child == children.size());
    return children;
  };

  const size_t root = 0;
  const djp::rmq_lca find_lca(tree.num_vertices(), root, children_of);

  EXPECT_EQ(2, (int)find_lca(8, 11));
  EXPECT_EQ(9, (int)find_lca(11, 12));
  EXPECT_EQ(0, (int)find_lca(12, 3));
  EXPECT_EQ(0, (int)find_lca(3, 12));
  EXPECT_EQ(6, (int)find_lca(10, 11));
}
