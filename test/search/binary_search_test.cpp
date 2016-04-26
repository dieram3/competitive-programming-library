//          Copyright Diego Ramirez 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cpl/search/binary_search.hpp>
#include <gtest/gtest.h>

#include <cpl/number_theory/basics.hpp> // multiply_less, multiply_greater
#include <climits>                      // INT_MAX
#include <cstdint>                      // uint32_t

using cpl::int_binary_search;
using cpl::multiply_less;
using cpl::multiply_greater;
using std::uint32_t;

static int ceil_sqrt(uint32_t num) {
  return int_binary_search<uint32_t>(0, num + 1, [num](uint32_t x) {
    return multiply_less(x, x, num); // safely checks x * x < num
  });
}

static int floor_sqrt(uint32_t num) {
  return int_binary_search<uint32_t>(0, num + 1, [num](uint32_t x) {
    return !multiply_greater(x + 1, x + 1, num); // (x + 1) * (x + 1) <= num;
  });
}

TEST(BinarySearchTest, SqrtTest) {
  EXPECT_EQ(0, ceil_sqrt(0));
  EXPECT_EQ(1, ceil_sqrt(1));
  EXPECT_EQ(2, ceil_sqrt(2));
  EXPECT_EQ(2, ceil_sqrt(3));
  EXPECT_EQ(2, ceil_sqrt(4));
  EXPECT_EQ(3, ceil_sqrt(5));
  EXPECT_EQ(4, ceil_sqrt(10));
  EXPECT_EQ(7, ceil_sqrt(49));
  EXPECT_EQ(8, ceil_sqrt(50));
  EXPECT_EQ(112, ceil_sqrt(12345));
  EXPECT_EQ(1234, ceil_sqrt(1522755));
  EXPECT_EQ(1234, ceil_sqrt(1522756));
  EXPECT_EQ(1235, ceil_sqrt(1522757));

  EXPECT_EQ(0, floor_sqrt(0));
  EXPECT_EQ(1, floor_sqrt(1));
  EXPECT_EQ(1, floor_sqrt(2));
  EXPECT_EQ(1, floor_sqrt(3));
  EXPECT_EQ(2, floor_sqrt(4));
  EXPECT_EQ(2, floor_sqrt(5));
  EXPECT_EQ(3, floor_sqrt(10));
  EXPECT_EQ(7, floor_sqrt(49));
  EXPECT_EQ(7, floor_sqrt(50));
  EXPECT_EQ(111, floor_sqrt(12345));
  EXPECT_EQ(1233, floor_sqrt(1522755));
  EXPECT_EQ(1234, floor_sqrt(1522756));
  EXPECT_EQ(1234, floor_sqrt(1522757));

  EXPECT_EQ(65535, ceil_sqrt(4294836224));
  EXPECT_EQ(65535, ceil_sqrt(4294836225));
  EXPECT_EQ(65536, ceil_sqrt(4294836226));

  EXPECT_EQ(65534, floor_sqrt(4294836224));
  EXPECT_EQ(65535, floor_sqrt(4294836225));
  EXPECT_EQ(65535, floor_sqrt(4294836226));
}

TEST(BinarySearchTest, EmptyRangeTest) {
  EXPECT_EQ(2, int_binary_search(2, 2, [](int) { return true; }));
  EXPECT_EQ(-3, int_binary_search(-3, -3, [](int) { return false; }));
}

TEST(BinarySearchTest, AllTrueTest) {
  EXPECT_EQ(100, int_binary_search(0, 100, [](int) { return true; }));
  EXPECT_EQ(-500, int_binary_search(-800, -500, [](int) { return true; }));
}

TEST(BinarySearchTest, AllFalseTest) {
  EXPECT_EQ(20, int_binary_search(20, 500, [](int) { return false; }));
  EXPECT_EQ(-150, int_binary_search(-150, 150, [](int) { return false; }));
}

TEST(BinarySearchTest, LimitsTest) {
  auto less_than_20 = [](int x) { return x < 20; };
  auto all_true = [](int) { return true; };
  auto all_false = [](int) { return false; };

  EXPECT_EQ(20, int_binary_search(INT_MIN / 2, INT_MAX / 2, less_than_20));

  EXPECT_EQ(INT_MIN / 2,
            int_binary_search(INT_MIN / 2, INT_MAX / 2, all_false));

  EXPECT_EQ(INT_MAX / 2, int_binary_search(INT_MIN / 2, INT_MAX / 2, all_true));
}
