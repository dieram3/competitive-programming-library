//          Copyright Diego Ramirez 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cpl/number_theory/basics.hpp>
#include <gtest/gtest.h>

#include <algorithm>        // for_each
#include <cstdint>          // uint_fast64_t, INT_MAX, UINT64_MAX
#include <initializer_list> // initializer_list
#include <tuple>            // tuple, get

using cpl::multiply_less;
using cpl::multiply_greater;
using cpl::ipow;
using std::uint_fast64_t;

namespace {
class BasicsTest : public ::testing::Test {};
} // end namespace

TEST_F(BasicsTest, CeilDivTest) {
  using cpl::ceil_div;
  using test_t = std::tuple<int, int, int>;

  auto expect = [](const int x, const int y, const int result) {
    EXPECT_EQ(result, ceil_div(x, y)) << "x=" << x << ", y=" << y;
  };
  auto check = [expect](const test_t& test) {
    const auto x = std::get<0>(test);
    const auto y = std::get<1>(test);
    const auto result = std::get<2>(test);
    expect(x, y, result);
    expect(-x, -y, result);
  };
  auto check_all = [check](std::initializer_list<test_t> tests) {
    std::for_each(tests.begin(), tests.end(), check);
  };

  // Zero as dividend:
  check_all({{0, 1, 0}, {0, 5, 0}, {0, 42, 0}});

  // Integers with the same sign:
  check_all({
      {1, 1, 1},   {1, 2, 1},      {1, 1000, 1},    {1, INT_MAX, 1},
      {2, 1, 2},   {2, 2, 1},      {2, 3, 1},       {9, 3, 3},
      {10, 3, 4},  {11, 3, 4},     {12, 3, 4},      {13, 3, 5},
      {10, 5, 2},  {10, 2, 5},     {11, 1, 11},     {11, 11, 1},
      {11, 12, 1}, {999, 1000, 1}, {1000, 1000, 1}, {1001, 1000, 2},
  });

  // Integers with opposite signs:
  check_all({
      {1, -1, -1},  {1, -2, 0},      {1, -1000, 0},     {1, -INT_MAX, 0},
      {2, -1, -2},  {2, -2, -1},     {2, -3, 0},        {9, -3, -3},
      {10, -3, -3}, {11, -3, -3},    {12, -3, -4},      {13, -3, -4},
      {10, -5, -2}, {10, -2, -5},    {11, -1, -11},     {11, -11, -1},
      {11, -12, 0}, {999, -1000, 0}, {1000, -1000, -1}, {1001, -1000, -1},
  });

  // Check that no floating-point trick is being used.
  EXPECT_EQ(UINT64_MAX, ceil_div<uint_fast64_t>(UINT64_MAX, 1));
}

TEST_F(BasicsTest, FloorDivTest) {
  using cpl::floor_div;
  using test_t = std::tuple<int, int, int>;

  auto expect = [](const int x, const int y, const int result) {
    EXPECT_EQ(result, floor_div(x, y)) << "x=" << x << ", y=" << y;
  };
  auto check = [expect](const test_t& test) {
    const auto x = std::get<0>(test);
    const auto y = std::get<1>(test);
    const auto result = std::get<2>(test);
    expect(x, y, result);
    expect(-x, -y, result);
  };
  auto check_all = [check](std::initializer_list<test_t> tests) {
    std::for_each(tests.begin(), tests.end(), check);
  };

  // Zero as dividend:
  check_all({{0, 1, 0}, {0, 5, 0}, {0, 42, 0}});

  // Integers with the same sign:
  check_all({
      {1, 1, 1},   {1, 2, 0},      {1, 1000, 0},    {1, INT_MAX, 0},
      {2, 1, 2},   {2, 2, 1},      {2, 3, 0},       {9, 3, 3},
      {10, 3, 3},  {11, 3, 3},     {12, 3, 4},      {13, 3, 4},
      {10, 5, 2},  {10, 2, 5},     {11, 1, 11},     {11, 11, 1},
      {11, 12, 0}, {999, 1000, 0}, {1000, 1000, 1}, {1001, 1000, 1},
  });

  // Integers with opposite signs:
  check_all({
      {1, -1, -1},   {1, -2, -1},      {1, -1000, -1},    {1, -INT_MAX, -1},
      {2, -1, -2},   {2, -2, -1},      {2, -3, -1},       {9, -3, -3},
      {10, -3, -4},  {11, -3, -4},     {12, -3, -4},      {13, -3, -5},
      {10, -5, -2},  {10, -2, -5},     {11, -1, -11},     {11, -11, -1},
      {11, -12, -1}, {999, -1000, -1}, {1000, -1000, -1}, {1001, -1000, -2},
  });

  // Check that no floating-point trick is being used.
  EXPECT_EQ(UINT64_MAX, floor_div<uint_fast64_t>(UINT64_MAX, 1));
}

TEST(MultiplyLessTest, WorksWell) {
  EXPECT_TRUE(multiply_less(0, 0, 1));
  EXPECT_TRUE(multiply_less(0, 1, 1));
  EXPECT_TRUE(multiply_less(1, 0, 1));
  EXPECT_TRUE(multiply_less(3, 3, 10));
  EXPECT_TRUE(multiply_less(5, 5, 26));
  EXPECT_TRUE(multiply_less(5, 10, 54));
  EXPECT_TRUE(multiply_less(10, 5, 54));

  EXPECT_FALSE(multiply_less(0, 0, 0));
  EXPECT_FALSE(multiply_less(0, 1, 0));
  EXPECT_FALSE(multiply_less(1, 0, 0));
  EXPECT_FALSE(multiply_less(65536u, 65536u, 4294967295u));
  EXPECT_FALSE(multiply_less(3, 3, 9));
  EXPECT_FALSE(multiply_less(4, 3, 10));
  EXPECT_FALSE(multiply_less(7, 5, 35));
  EXPECT_FALSE(multiply_less(5, 7, 35));
}

TEST(MultiplyGreaterTest, WorksWell) {
  EXPECT_TRUE(multiply_greater(65536u, 65536u, 4294967295u));
  EXPECT_TRUE(multiply_greater(4, 3, 10));
  EXPECT_TRUE(multiply_greater(4, 2, 7));
  EXPECT_TRUE(multiply_greater(5, 2, 9));
  EXPECT_TRUE(multiply_greater(2, 5, 9));
  EXPECT_TRUE(multiply_greater(10, 5, 46));
  EXPECT_TRUE(multiply_greater(5, 10, 46));

  EXPECT_FALSE(multiply_greater(0, 0, 0));
  EXPECT_FALSE(multiply_greater(0, 1, 0));
  EXPECT_FALSE(multiply_greater(1, 0, 0));
  EXPECT_FALSE(multiply_greater(0, 0, 1));
  EXPECT_FALSE(multiply_greater(0, 1, 1));
  EXPECT_FALSE(multiply_greater(1, 0, 1));
  EXPECT_FALSE(multiply_greater(3, 3, 9));
  EXPECT_FALSE(multiply_greater(3, 3, 10));
}

TEST(IPowTest, WorksWell) {
  using int_t = std::uint_fast64_t;
  EXPECT_EQ(1, ipow<int_t>(1, 0));
  EXPECT_EQ(1, ipow<int_t>(2, 0));
  EXPECT_EQ(1, ipow<int_t>(29, 0));
  EXPECT_EQ(29, ipow<int_t>(29, 1));
  EXPECT_EQ(841, ipow<int_t>(29, 2));
  EXPECT_EQ(129140163, ipow<int_t>(3, 17));
  EXPECT_EQ(476837158203125, ipow<int_t>(5, 21));
  EXPECT_EQ(819628286980801, ipow<int_t>(31, 10));

  // Special cases
  EXPECT_EQ(1, ipow(0, 0));
  EXPECT_EQ(0, ipow(0, 1));
  EXPECT_EQ(0, ipow(0, 2));
}
