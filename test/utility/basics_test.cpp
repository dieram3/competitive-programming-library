//          Copyright Diego Ramírez June 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cpl/utility/basics.hpp>
#include <gtest/gtest.h>

TEST(repeat, WorksWell) {
  int value = 0;
  cpl::repeat(13, [&value] { ++value; });
  EXPECT_EQ(13, value);
  cpl::repeat(0, [&value] { --value; });
  EXPECT_EQ(13, value);
  cpl::repeat(3, [&value] { --value; });
  EXPECT_EQ(10, value);
}
