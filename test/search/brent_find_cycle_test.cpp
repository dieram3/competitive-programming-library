//          Copyright Diego Ramírez September 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cpl/search/brent_find_cycle.hpp>
#include <gtest/gtest.h>

#include <utility> // For std::pair
#include <cstddef> // For std::size_t
#include <cstdint> // For std::uint32_t

using namespace cpl;
using pair = std::pair<size_t, size_t>;
using std::uint32_t;

TEST(BrentFindCycleTest, TableFunctionTest) {
  int table[9] = {6, 6, 0, 1, 4, 3, 3, 4, 0};
  auto f = [&](int x) { return table[x]; };

  EXPECT_EQ(pair(3, 1), brent_find_cycle(f, 0));
  EXPECT_EQ(pair(3, 0), brent_find_cycle(f, 1));
  EXPECT_EQ(pair(3, 2), brent_find_cycle(f, 2));
  EXPECT_EQ(pair(3, 0), brent_find_cycle(f, 3));
  EXPECT_EQ(pair(1, 0), brent_find_cycle(f, 4));
  EXPECT_EQ(pair(3, 1), brent_find_cycle(f, 5));
  EXPECT_EQ(pair(3, 0), brent_find_cycle(f, 6));
  EXPECT_EQ(pair(1, 1), brent_find_cycle(f, 7));
  EXPECT_EQ(pair(3, 2), brent_find_cycle(f, 8));
}

TEST(BrentFindCycleTest, PRNGTest1) {
  auto f = [](uint32_t x) { return (5173 * x + 3849) % 3279; };
  EXPECT_EQ(pair(546, 0), brent_find_cycle(f, uint32_t{1511}));
  EXPECT_EQ(pair(546, 0), brent_find_cycle(f, uint32_t{1312}));
  EXPECT_EQ(pair(546, 0), brent_find_cycle(f, uint32_t{2}));
  EXPECT_EQ(pair(546, 1), brent_find_cycle(f, uint32_t{3279}));
  EXPECT_EQ(pair(546, 1), brent_find_cycle(f, uint32_t{4102}));
  EXPECT_EQ(pair(546, 1), brent_find_cycle(f, uint32_t{5989}));
  EXPECT_EQ(pair(1, 0), brent_find_cycle(f, uint32_t{942}));
  EXPECT_EQ(pair(1, 0), brent_find_cycle(f, uint32_t{2035}));
  EXPECT_EQ(pair(1, 0), brent_find_cycle(f, uint32_t{3128}));
}

TEST(BrentFindCycleTest, PRNGTest2) {
  auto f = [](uint32_t x) { return (9111 * x + 5309) % 6000; };
  EXPECT_EQ(pair(500, 1), brent_find_cycle(f, uint32_t{1234}));
  EXPECT_EQ(pair(500, 0), brent_find_cycle(f, uint32_t{653}));
  EXPECT_EQ(pair(500, 1), brent_find_cycle(f, uint32_t{1012}));
  EXPECT_EQ(pair(500, 0), brent_find_cycle(f, uint32_t{2129}));
  EXPECT_EQ(pair(500, 1), brent_find_cycle(f, uint32_t{657}));
  EXPECT_EQ(pair(500, 0), brent_find_cycle(f, uint32_t{146}));
  EXPECT_EQ(pair(500, 1), brent_find_cycle(f, uint32_t{0}));
  EXPECT_EQ(pair(500, 0), brent_find_cycle(f, uint32_t{5537}));
}

TEST(BrentFindCycleTest, PRNGTest3) {
  auto f = [](uint32_t x) { return (1079 * x + 2136) % 9999; };
  EXPECT_EQ(pair(220, 0), brent_find_cycle(f, 537));
  EXPECT_EQ(pair(220, 0), brent_find_cycle(f, 1237));
  EXPECT_EQ(pair(220, 0), brent_find_cycle(f, 8333));
  EXPECT_EQ(pair(220, 0), brent_find_cycle(f, 8299));
  EXPECT_EQ(pair(220, 1), brent_find_cycle(f, 9999));
  EXPECT_EQ(pair(220, 1), brent_find_cycle(f, 11966));
  EXPECT_EQ(pair(220, 1), brent_find_cycle(f, 13344));
  EXPECT_EQ(pair(220, 1), brent_find_cycle(f, 14987));

  EXPECT_EQ(pair(22, 0), brent_find_cycle(f, 836));
  EXPECT_EQ(pair(22, 0), brent_find_cycle(f, 6896));
  EXPECT_EQ(pair(22, 1), brent_find_cycle(f, 13562));
  EXPECT_EQ(pair(22, 1), brent_find_cycle(f, 14976));

  EXPECT_EQ(pair(11, 0), brent_find_cycle(f, 735));
  EXPECT_EQ(pair(11, 0), brent_find_cycle(f, 7098));
  EXPECT_EQ(pair(11, 1), brent_find_cycle(f, 11643));
  EXPECT_EQ(pair(11, 1), brent_find_cycle(f, 14370));
}
