//          Copyright Diego Ramírez May 2015, August 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cpl/graph/lowest_common_ancestor.hpp>
#include <gtest/gtest.h>

#include <cpl/graph/undirected_graph.hpp>

using namespace cpl;

namespace {

class RMQLowestCommonAncestorTest : public ::testing::Test {
public:
  RMQLowestCommonAncestorTest() : tree(13) {
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

    EXPECT_EQ(12u, tree.num_edges());
  }

protected:
  undirected_graph tree;
};

} // End anonymous namespace

TEST_F(RMQLowestCommonAncestorTest, FindLCATest) {
  {
    const rmq_lca querier(tree, 0); // The root will be vertex 0
    EXPECT_EQ(2, querier.lca(8, 11));
    EXPECT_EQ(9, querier.lca(11, 12));
    EXPECT_EQ(0, querier.lca(12, 3));
    EXPECT_EQ(0, querier.lca(3, 12));
    EXPECT_EQ(6, querier.lca(10, 11));
  }
  {
    const rmq_lca querier(tree, 9); // Now, the root will be vertex 9
    EXPECT_EQ(9, querier.lca(11, 9));
    EXPECT_EQ(9, querier.lca(9, 3));
    EXPECT_EQ(6, querier.lca(4, 10));
    EXPECT_EQ(2, querier.lca(1, 8));
    EXPECT_EQ(0, querier.lca(1, 3));
    EXPECT_EQ(5, querier.lca(7, 8));
    EXPECT_EQ(2, querier.lca(5, 4));
    EXPECT_EQ(2, querier.lca(4, 8));
  }
  {
    const rmq_lca querier(tree, 6); // And finally, the root will be vertex 6.
    EXPECT_EQ(6, querier.lca(11, 3));
    EXPECT_EQ(6, querier.lca(3, 11));
    EXPECT_EQ(2, querier.lca(4, 8));
    EXPECT_EQ(6, querier.lca(8, 11));
    EXPECT_EQ(6, querier.lca(3, 12));
    EXPECT_EQ(2, querier.lca(3, 8));
  }
}

TEST_F(RMQLowestCommonAncestorTest, DepthOfTest) {
  {
    const rmq_lca querier(tree, 0);
    EXPECT_EQ(0, querier.depth_of(0));
    EXPECT_EQ(4, querier.depth_of(11));
    EXPECT_EQ(2, querier.depth_of(5));
    EXPECT_EQ(1, querier.depth_of(1));
  }
  {
    const rmq_lca querier(tree, 9);
    EXPECT_EQ(0, querier.depth_of(9));
    EXPECT_EQ(1, querier.depth_of(11));
    EXPECT_EQ(2, querier.depth_of(10));
    EXPECT_EQ(4, querier.depth_of(1));
  }
  {
    const rmq_lca querier(tree, 6);
    EXPECT_EQ(0, querier.depth_of(6));
    EXPECT_EQ(3, querier.depth_of(1));
    EXPECT_EQ(2, querier.depth_of(4));
    EXPECT_EQ(1, querier.depth_of(2));
  }
}

TEST_F(RMQLowestCommonAncestorTest, DistanceTest) {
  {
    const rmq_lca querier(tree, 1);
    EXPECT_EQ(0, querier.distance(1, 1));
    EXPECT_EQ(0, querier.distance(0, 0));
    EXPECT_EQ(0, querier.distance(9, 9));
    EXPECT_EQ(1, querier.distance(11, 9));
    EXPECT_EQ(2, querier.distance(11, 12));
    EXPECT_EQ(5, querier.distance(12, 1));
  }
  {
    const rmq_lca querier(tree, 2);
    EXPECT_EQ(1, querier.distance(0, 2));
    EXPECT_EQ(2, querier.distance(9, 10));
    EXPECT_EQ(5, querier.distance(12, 1));
    EXPECT_EQ(5, querier.distance(11, 3));
    EXPECT_EQ(5, querier.distance(12, 8));
    EXPECT_EQ(2, querier.distance(7, 8));
  }
  {
    const rmq_lca querier(tree, 5);
    EXPECT_EQ(2, querier.distance(1, 2));
    EXPECT_EQ(2, querier.distance(2, 1));
    EXPECT_EQ(1, querier.distance(0, 2));
    EXPECT_EQ(0, querier.distance(9, 9));
    EXPECT_EQ(3, querier.distance(3, 5));
    EXPECT_EQ(5, querier.distance(12, 8));
    EXPECT_EQ(5, querier.distance(12, 7));
  }
}

TEST_F(RMQLowestCommonAncestorTest, VisitsTest) {
  {
    // Check corner cases
    const rmq_lca querier(tree, 0);
    EXPECT_TRUE(querier.visits(0, 0, 0));
    EXPECT_TRUE(querier.visits(3, 3, 3));
    EXPECT_FALSE(querier.visits(0, 0, 1));
    EXPECT_FALSE(querier.visits(4, 4, 3));

    EXPECT_TRUE(querier.visits(0, 1, 1));
    EXPECT_TRUE(querier.visits(0, 1, 0));
    EXPECT_TRUE(querier.visits(2, 9, 2));
    EXPECT_TRUE(querier.visits(2, 9, 9));
  }
  {
    // Check normal cases.
    const rmq_lca querier(tree, 6);
    EXPECT_TRUE(querier.visits(1, 2, 0));
    EXPECT_TRUE(querier.visits(2, 1, 0));
    EXPECT_FALSE(querier.visits(1, 2, 3));
    EXPECT_FALSE(querier.visits(1, 12, 10));
    EXPECT_FALSE(querier.visits(4, 10, 9));
    EXPECT_TRUE(querier.visits(4, 10, 2));
    EXPECT_TRUE(querier.visits(10, 4, 6));
  }
  {
    // Check a short path.
    const rmq_lca querier(tree, 2);
    EXPECT_FALSE(querier.visits(2, 9, 0));
    EXPECT_FALSE(querier.visits(2, 9, 1));
    EXPECT_TRUE(querier.visits(2, 9, 2));
    EXPECT_FALSE(querier.visits(2, 9, 3));
    EXPECT_FALSE(querier.visits(2, 9, 4));
    EXPECT_FALSE(querier.visits(2, 9, 5));
    EXPECT_TRUE(querier.visits(2, 9, 6));
    EXPECT_FALSE(querier.visits(2, 9, 7));
    EXPECT_FALSE(querier.visits(2, 9, 8));
    EXPECT_TRUE(querier.visits(2, 9, 9));
    EXPECT_FALSE(querier.visits(2, 9, 10));
    EXPECT_FALSE(querier.visits(2, 9, 11));
    EXPECT_FALSE(querier.visits(2, 9, 12));
  }
  {
    // Check a long path.
    const rmq_lca querier(tree, 7);
    EXPECT_TRUE(querier.visits(1, 12, 0));
    EXPECT_TRUE(querier.visits(1, 12, 1));
    EXPECT_TRUE(querier.visits(1, 12, 2));
    EXPECT_FALSE(querier.visits(1, 12, 3));
    EXPECT_FALSE(querier.visits(1, 12, 4));
    EXPECT_FALSE(querier.visits(1, 12, 5));
    EXPECT_TRUE(querier.visits(1, 12, 6));
    EXPECT_FALSE(querier.visits(1, 12, 7));
    EXPECT_FALSE(querier.visits(1, 12, 8));
    EXPECT_TRUE(querier.visits(1, 12, 9));
    EXPECT_FALSE(querier.visits(1, 12, 10));
    EXPECT_FALSE(querier.visits(1, 12, 11));
    EXPECT_TRUE(querier.visits(1, 12, 12));
  }
}
