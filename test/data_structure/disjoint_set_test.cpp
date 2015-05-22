//          Copyright Diego Ramírez May 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <djp/data_structure/disjoint_set.hpp>
#include <gtest/gtest.h>
#include <array>   // for std::array
#include <cstddef> // for std::size_t
#include <cmath>   // for std::log2, std::lround

TEST(disjoint_set, WorksOnSmallSets) {
  djp::disjoint_set dset(8);
  using node_t = djp::disjoint_set::node;

  dset.make_union(2, 3);
  dset.make_union(0, 4);
  dset.make_union(1, 7);
  dset.make_union(5, 4);
  dset.make_union(1, 0);

  std::array<const node_t *, 8> roots;
  for (size_t i = 0; i != roots.size(); ++i)
    roots[i] = dset.find_root(i);

  EXPECT_EQ(roots[0], roots[1]);
  EXPECT_EQ(roots[1], roots[4]);
  EXPECT_EQ(roots[4], roots[5]);
  EXPECT_EQ(roots[5], roots[7]);
  EXPECT_NE(roots[7], roots[2]);
  EXPECT_EQ(roots[2], roots[3]);
  EXPECT_NE(roots[3], roots[6]);
  EXPECT_NE(roots[6], roots[0]);

  EXPECT_EQ(5, roots[0]->num_elements);
  EXPECT_EQ(2, roots[2]->num_elements);
  EXPECT_EQ(1, roots[6]->num_elements);
}

TEST(disjoint_set, WorksOnBigSets) {
  const size_t num_sets = 10000;
  const size_t max_accepted_depth = std::lround(2 * std::log2(num_sets));
  djp::disjoint_set dset(num_sets);
  for (size_t i = 1; i < num_sets; ++i) {
    dset.make_union(0, i);
    auto root_0 = dset.find_root(0);
    EXPECT_EQ(root_0, dset.find_root(i));
    EXPECT_EQ(i + 1, root_0->num_elements);
    EXPECT_LT(root_0->depth, max_accepted_depth);
  }
}
