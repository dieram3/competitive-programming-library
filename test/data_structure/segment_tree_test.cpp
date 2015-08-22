//          Copyright Diego Ramírez July 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <djp/data_structure/segment_tree.hpp>
#include <gtest/gtest.h>

#include <numeric>    // For std::iota
#include <functional> // For std::plus
#include <vector>     // For std::vector

using namespace djp;
/// \todo Add more tests.

static segment_tree<int, std::plus<int>> make_iota_segment_tree(size_t sz) {
  std::vector<int> vec(sz);
  std::iota(vec.begin(), vec.end(), 1);
  segment_tree<int, std::plus<int>> stree;
  stree.assign(vec.cbegin(), vec.cend());
  return stree;
}

TEST(SegmentTreeTest, HandlesSum) {
  const auto stree = make_iota_segment_tree(10);
  EXPECT_EQ(10, stree.accumulate(0, 4));
  EXPECT_EQ(34, stree.accumulate(6, 10));
  EXPECT_EQ(55, stree.accumulate(0, 10));
}

TEST(SegmentTreeTest, HandlesForeach) {
  auto stree = make_iota_segment_tree(10);
  for (size_t i = 0; i != 5; ++i)
    stree.modify(i, 2 * stree.at(i));
  EXPECT_EQ(2, stree.accumulate(0, 1));
  EXPECT_EQ(10, stree.accumulate(1, 3));
  EXPECT_EQ(30, stree.accumulate(0, 5));
  EXPECT_EQ(70, stree.accumulate(0, 10));
}
