//          Copyright Diego Ramírez May 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <djp/graph/lowest_common_ancestor.hpp>
#include <djp/graph/undirected_graph.hpp>
#include <gtest/gtest.h>

#include <vector>
#include <cstddef>
#include <cassert>

using namespace djp;

using undigraph_t = undirected_graph<>;

TEST(RMQLowestCommonAncestor, WorksForDifferentRoots) {
  undigraph_t tree(13);

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

  {
    const rmq_lca lca(tree, 0); // The root will be vertex 0
    EXPECT_EQ(2, lca(8, 11));
    EXPECT_EQ(9, lca(11, 12));
    EXPECT_EQ(0, lca(12, 3));
    EXPECT_EQ(0, lca(3, 12));
    EXPECT_EQ(6, lca(10, 11));

    EXPECT_EQ(0, lca.depth_of(0));
    EXPECT_EQ(4, lca.depth_of(11));
    EXPECT_EQ(2, lca.depth_of(5));
    EXPECT_EQ(1, lca.depth_of(1));
  }
  {
    const rmq_lca lca(tree, 9); // Now, the root will be vertex 9
    EXPECT_EQ(9, lca(11, 9));
    EXPECT_EQ(9, lca(9, 3));
    EXPECT_EQ(6, lca(4, 10));
    EXPECT_EQ(2, lca(1, 8));
    EXPECT_EQ(0, lca(1, 3));
    EXPECT_EQ(5, lca(7, 8));
    EXPECT_EQ(2, lca(5, 4));
    EXPECT_EQ(2, lca(4, 8));

    EXPECT_EQ(0, lca.depth_of(9));
    EXPECT_EQ(1, lca.depth_of(11));
    EXPECT_EQ(2, lca.depth_of(10));
    EXPECT_EQ(4, lca.depth_of(1));
  }
  {
    const rmq_lca lca(tree, 6); // And finally, the root will be vertex 6.
    EXPECT_EQ(6, lca(11, 3));
    EXPECT_EQ(6, lca(3, 11));
    EXPECT_EQ(2, lca(4, 8));
    EXPECT_EQ(6, lca(8, 11));
    EXPECT_EQ(6, lca(3, 12));
    EXPECT_EQ(2, lca(3, 8));

    EXPECT_EQ(0, lca.depth_of(6));
    EXPECT_EQ(3, lca.depth_of(1));
    EXPECT_EQ(2, lca.depth_of(4));
    EXPECT_EQ(1, lca.depth_of(2));
  }
}
