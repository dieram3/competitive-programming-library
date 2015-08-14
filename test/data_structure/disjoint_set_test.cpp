//          Copyright Diego Ramírez May 2015, August 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <djp/data_structure/disjoint_set.hpp>
#include <gtest/gtest.h>

using namespace djp;

TEST(disjoint_set, WorksWell) {
  disjoint_set d(8);

  d.union_set(2, 3);
  d.union_set(0, 4);
  d.union_set(1, 7);
  d.union_set(5, 4);
  d.union_set(1, 0);

  EXPECT_EQ(d.find_set(1), d.find_set(4));
  EXPECT_EQ(d.find_set(4), d.find_set(5));
  EXPECT_EQ(d.find_set(5), d.find_set(7));
  EXPECT_NE(d.find_set(7), d.find_set(2));
  EXPECT_EQ(d.find_set(2), d.find_set(3));
  EXPECT_NE(d.find_set(3), d.find_set(6));
  EXPECT_NE(d.find_set(6), d.find_set(0));
}
