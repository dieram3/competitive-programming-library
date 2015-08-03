//          Copyright Diego Ramírez May 2015, August 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <djp/graph/lowest_common_ancestor.hpp>
#include <gtest/gtest.h>

#include <djp/graph/undirected_graph.hpp>

using namespace djp;

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
  undirected_graph<> tree;
};

} // End anonymous namespace

TEST_F(RMQLowestCommonAncestorTest, FindLCATest) {
  {
    const rmq_lca querier(tree, 0); // The root will be vertex 0
    EXPECT_EQ(2, querier.find_lca(8, 11));
    EXPECT_EQ(9, querier.find_lca(11, 12));
    EXPECT_EQ(0, querier.find_lca(12, 3));
    EXPECT_EQ(0, querier.find_lca(3, 12));
    EXPECT_EQ(6, querier.find_lca(10, 11));
  }
  {
    const rmq_lca querier(tree, 9); // Now, the root will be vertex 9
    EXPECT_EQ(9, querier.find_lca(11, 9));
    EXPECT_EQ(9, querier.find_lca(9, 3));
    EXPECT_EQ(6, querier.find_lca(4, 10));
    EXPECT_EQ(2, querier.find_lca(1, 8));
    EXPECT_EQ(0, querier.find_lca(1, 3));
    EXPECT_EQ(5, querier.find_lca(7, 8));
    EXPECT_EQ(2, querier.find_lca(5, 4));
    EXPECT_EQ(2, querier.find_lca(4, 8));
  }
  {
    const rmq_lca querier(tree, 6); // And finally, the root will be vertex 6.
    EXPECT_EQ(6, querier.find_lca(11, 3));
    EXPECT_EQ(6, querier.find_lca(3, 11));
    EXPECT_EQ(2, querier.find_lca(4, 8));
    EXPECT_EQ(6, querier.find_lca(8, 11));
    EXPECT_EQ(6, querier.find_lca(3, 12));
    EXPECT_EQ(2, querier.find_lca(3, 8));
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
