//          Copyright Diego Ramírez May 2015, August 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <djp/data_structure/disjoint_set.hpp>
#include <gtest/gtest.h>

#include <array>   // for std::array
#include <cstddef> // for std::size_t

using namespace djp;

TEST(disjoint_set, WorksWell) {
  disjoint_set dset(8);
  for (size_t i = 0; i != 8; ++i)
    dset.make_set(i);

  dset.union_set(2, 3);
  dset.union_set(0, 4);
  dset.union_set(1, 7);
  dset.union_set(5, 4);
  dset.union_set(1, 0);

  std::array<size_t, 8> leader;
  for (size_t i = 0; i != leader.size(); ++i)
    leader[i] = dset.find_set(i);

  EXPECT_EQ(leader[0], leader[1]);
  EXPECT_EQ(leader[1], leader[4]);
  EXPECT_EQ(leader[4], leader[5]);
  EXPECT_EQ(leader[5], leader[7]);
  EXPECT_NE(leader[7], leader[2]);
  EXPECT_EQ(leader[2], leader[3]);
  EXPECT_NE(leader[3], leader[6]);
  EXPECT_NE(leader[6], leader[0]);
}
