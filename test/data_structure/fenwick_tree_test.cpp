//          Copyright Diego Ramirez 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cpl/data_structure/fenwick_tree.hpp>
#include <gtest/gtest.h>

namespace {

using cpl::fenwick_tree;

TEST(FenwickTree, IsZeroInitialized) {
  fenwick_tree<int> ftree(20);
  EXPECT_EQ(0, ftree.get_prefix_sum(20));
}

TEST(FenwickTree, SumPrefixAndIsMutable) {
  const size_t num_elems = 10;
  const int multipler = 42;
  fenwick_tree<int> ftree(num_elems);
  for (size_t i = 0; i < num_elems; ++i)
    ftree.add(i, multipler * static_cast<int>(i + 1));

  for (size_t count = 0; count <= num_elems; ++count) {
    const auto expected = multipler * (count * (count + 1)) / 2;
    EXPECT_EQ(static_cast<int>(expected), ftree.get_prefix_sum(count));
  }
}

} // namespace
