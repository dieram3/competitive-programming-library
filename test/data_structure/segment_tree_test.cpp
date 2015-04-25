//          Copyright Diego Ramírez July 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <djp/data_structure/segment_tree.hpp>
#include <gtest/gtest.h>
#include <array>
#include <functional>

class segment_tree_test : public ::testing::Test {
 protected:
  segment_tree_test() : stree(arr.begin(), arr.end()) {}

  std::array<int, 10> arr = {{1, 2, 3, 4, 5, 6, 7, 8, 9, 10}};
  djp::segment_tree<int, std::plus<int>> stree;
};

TEST_F(segment_tree_test, HandlesSum) {
  EXPECT_EQ(10, stree.accumulate(0, 4));
  EXPECT_EQ(34, stree.accumulate(6, 10));
  EXPECT_EQ(55, stree.accumulate(0, 10));
}

TEST_F(segment_tree_test, HandlesForeach) {
  auto multiply_by_2 = [](int& elem) { elem *= 2; };
  stree.for_each(0, 5, multiply_by_2);
  EXPECT_EQ(2, stree.accumulate(0, 1));
  EXPECT_EQ(10, stree.accumulate(1, 3));
  EXPECT_EQ(30, stree.accumulate(0, 5));
  EXPECT_EQ(70, stree.accumulate(0, 10));
}
