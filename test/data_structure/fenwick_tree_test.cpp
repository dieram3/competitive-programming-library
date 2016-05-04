//          Copyright Diego Ramirez 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cpl/data_structure/fenwick_tree.hpp>
#include <gtest/gtest.h>

using cpl::fenwick_tree;
using cpl::prefix_adder;

TEST(fenwick_tree, IsSizeable) {
  fenwick_tree<unsigned> ftree(23);
  ASSERT_EQ(23u, ftree.size());
}

TEST(fenwick_tree, IsZeroConstructed) {
  fenwick_tree<int> ftree(20);
  EXPECT_EQ(0, ftree.sum(ftree.size() - 1));
}

TEST(fenwick_tree, SumPrefixAndIsMutable) {
  fenwick_tree<long> ftree(10);
  for (size_t i = 0; i < ftree.size(); ++i)
    ftree.increase(i, 10 * i);
  for (size_t i = 0, acc = 0; i < ftree.size(); ++i, acc += i * 10)
    EXPECT_EQ(acc, ftree.sum(i));

  EXPECT_EQ(80 + 90, ftree.sum(8, 9));
  EXPECT_EQ(30 + 40 + 50, ftree.sum(3, 5));

  ftree.increase(8, 10);
  EXPECT_EQ(70, ftree.sum(7, 7));
  EXPECT_EQ(80 + 10, ftree.sum(8, 8));
  EXPECT_EQ(90, ftree.sum(9, 9));
}

TEST(prefix_adder, IsSizeable) {
  prefix_adder<int> adder(17);
  EXPECT_EQ(17u, adder.size());
}

TEST(prefix_adder, SumPrefixAndIsMutable) {
  prefix_adder<int> adder(10);
  adder.increase(0, -10);
  adder.increase(1, -5);
  EXPECT_EQ(-15, adder.sum(0, 1));
  adder.replace(1, -10);
  EXPECT_EQ(-20, adder.sum(0, 1));
}

TEST(prefix_adder, HasFastAccessToSingleElements) {
  prefix_adder<int> adder(10);
  EXPECT_EQ(0, adder.at(5));
  adder.increase(5, 30);
  EXPECT_EQ(30, adder.at(5));
}
