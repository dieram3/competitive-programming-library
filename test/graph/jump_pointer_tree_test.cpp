//          Copyright Diego Ramirez 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cpl/graph/jump_pointer_tree.hpp>
#include <gtest/gtest.h>

#include <cpl/graph/undirected_graph.hpp> // undirected_graph
#include <cassert>                        // assert

using cpl::jump_pointer_tree;
using cpl::undirected_graph;

namespace {
class JumpPointerTreeTest : public ::testing::Test {
protected:
  JumpPointerTreeTest() : tree(11) {
    tree.add_edge(0, 4);
    tree.add_edge(0, 6);
    tree.add_edge(0, 8);
    tree.add_edge(1, 7);
    tree.add_edge(2, 7);
    tree.add_edge(3, 6);
    tree.add_edge(5, 6);
    tree.add_edge(6, 9);
    tree.add_edge(7, 8);
    tree.add_edge(7, 10);
    assert(tree.num_edges() == 10);
  }

protected:
  undirected_graph tree;
};
} // end anonymous namespace

TEST_F(JumpPointerTreeTest, DepthTest) {
  {
    jump_pointer_tree jp(tree, 6);
    EXPECT_EQ(1, jp.depth_of(0));
    EXPECT_EQ(4, jp.depth_of(1));
    EXPECT_EQ(4, jp.depth_of(2));
    EXPECT_EQ(1, jp.depth_of(3));
    EXPECT_EQ(2, jp.depth_of(4));
    EXPECT_EQ(1, jp.depth_of(5));
    EXPECT_EQ(0, jp.depth_of(6));
    EXPECT_EQ(3, jp.depth_of(7));
    EXPECT_EQ(2, jp.depth_of(8));
    EXPECT_EQ(1, jp.depth_of(9));
    EXPECT_EQ(4, jp.depth_of(10));
  }
  {
    jump_pointer_tree jp(tree, 10);
    EXPECT_EQ(3, jp.depth_of(0));
    EXPECT_EQ(2, jp.depth_of(1));
    EXPECT_EQ(2, jp.depth_of(2));
    EXPECT_EQ(5, jp.depth_of(3));
    EXPECT_EQ(4, jp.depth_of(4));
    EXPECT_EQ(5, jp.depth_of(5));
    EXPECT_EQ(4, jp.depth_of(6));
    EXPECT_EQ(1, jp.depth_of(7));
    EXPECT_EQ(2, jp.depth_of(8));
    EXPECT_EQ(5, jp.depth_of(9));
    EXPECT_EQ(0, jp.depth_of(10));
  }
}

TEST_F(JumpPointerTreeTest, KthAncestorTest) {
  {
    jump_pointer_tree jp(tree, 3);
    EXPECT_EQ(3, jp.kth_ancestor(3, 0));
    EXPECT_EQ(3, jp.kth_ancestor(6, 1));
    EXPECT_EQ(6, jp.kth_ancestor(4, 2));

    ASSERT_EQ(5, jp.depth_of(10));
    EXPECT_EQ(10, jp.kth_ancestor(10, 0));
    EXPECT_EQ(7, jp.kth_ancestor(10, 1));
    EXPECT_EQ(8, jp.kth_ancestor(10, 2));
    EXPECT_EQ(0, jp.kth_ancestor(10, 3));
    EXPECT_EQ(6, jp.kth_ancestor(10, 4));
    EXPECT_EQ(3, jp.kth_ancestor(10, 5));
  }
  {
    jump_pointer_tree jp(tree, 8);
    EXPECT_EQ(8, jp.kth_ancestor(9, 3));
    EXPECT_EQ(8, jp.kth_ancestor(8, 0));
    EXPECT_EQ(8, jp.kth_ancestor(10, 2));
    EXPECT_EQ(7, jp.kth_ancestor(7, 0));
    EXPECT_EQ(8, jp.kth_ancestor(7, 1));
    EXPECT_EQ(0, jp.kth_ancestor(5, 2));
  }
  {
    jump_pointer_tree jp(tree, 10);
    EXPECT_EQ(7, jp.kth_ancestor(9, 4));
    EXPECT_EQ(7, jp.kth_ancestor(3, 4));
    EXPECT_EQ(7, jp.kth_ancestor(5, 4));
    EXPECT_EQ(10, jp.kth_ancestor(4, 4));
    EXPECT_EQ(1, jp.kth_ancestor(1, 0));
  }
}

TEST_F(JumpPointerTreeTest, LevelAncestorTest) {
  {
    jump_pointer_tree jp(tree, 3);
    EXPECT_EQ(3, jp.level_ancestor(3, 0));
    EXPECT_EQ(3, jp.level_ancestor(8, 0));
    EXPECT_EQ(4, jp.level_ancestor(4, 3));

    EXPECT_EQ(3, jp.level_ancestor(10, 0));
    EXPECT_EQ(6, jp.level_ancestor(10, 1));
    EXPECT_EQ(0, jp.level_ancestor(10, 2));
    EXPECT_EQ(8, jp.level_ancestor(10, 3));
    EXPECT_EQ(7, jp.level_ancestor(10, 4));
    EXPECT_EQ(10, jp.level_ancestor(10, 5));
  }
  {
    jump_pointer_tree jp(tree, 0);
    EXPECT_EQ(0, jp.level_ancestor(9, 0));
    EXPECT_EQ(6, jp.level_ancestor(9, 1));
    EXPECT_EQ(9, jp.level_ancestor(9, 2));

    EXPECT_EQ(0, jp.level_ancestor(1, 0));
    EXPECT_EQ(8, jp.level_ancestor(1, 1));
    EXPECT_EQ(7, jp.level_ancestor(1, 2));
    EXPECT_EQ(1, jp.level_ancestor(1, 3));

    EXPECT_EQ(0, jp.level_ancestor(0, 0));
  }
}

TEST_F(JumpPointerTreeTest, SingleVertexGraphTest) {
  jump_pointer_tree jp(undirected_graph(1), 0);
  EXPECT_EQ(0, jp.depth_of(0));
  EXPECT_EQ(0, jp.kth_ancestor(0, 0));
  EXPECT_EQ(0, jp.level_ancestor(0, 0));
}
