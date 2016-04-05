//          Copyright Diego Ramírez November 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cpl/combinatorics/catalan_numbers.hpp>
#include <gtest/gtest.h>

#include <vector>  // For std::vector
#include <cstddef> // For std::size_t
#include <cstdint> // For std::int32_t, std::uint64_t

using namespace cpl;
using std::size_t;
using std::int32_t;
using std::uint64_t;

TEST(CatalanNumbersTest, EmptySequenceTest) {
  const std::vector<int> cat = generate_catalan_numbers<int>(0);
  EXPECT_TRUE(cat.empty());
}

TEST(CatalanNumbersTest, Int32Test) {
  std::vector<int32_t> expected_cat = {
      1,       1,       2,        5,         14,        42,        132,
      429,     1430,    4862,     16796,     58786,     208012,    742900,
      2674440, 9694845, 35357670, 129644790, 477638700, 1767263190};

  ASSERT_EQ(20, expected_cat.size());

  const auto cat = generate_catalan_numbers<int32_t>(20);
  ASSERT_EQ(expected_cat.size(), cat.size());
  for (size_t i = 0; i < cat.size(); ++i)
    EXPECT_EQ(expected_cat[i], cat[i]) << "for i=" << i;
}

TEST(CatalanNumbersTest, Uint64Test) {
  std::vector<uint64_t> expected_cat = {
      1, 1, 2, 5, 14, 42, 132, 429, 1430, 4862, 16796, 58786, 208012, 742900,
      2674440, 9694845, 35357670, 129644790, 477638700, 1767263190, 6564120420,
      24466267020, 91482563640, 343059613650, 1289904147324, 4861946401452,
      18367353072152, 69533550916004, 263747951750360, 1002242216651368,
      3814986502092304, 14544636039226909, 55534064877048198,
      212336130412243110, 812944042149730764, 3116285494907301262,
      11959798385860453492u};

  ASSERT_EQ(37, expected_cat.size());

  const auto cat = generate_catalan_numbers<uint64_t>(37);
  ASSERT_EQ(expected_cat.size(), cat.size());
  for (size_t i = 0; i < cat.size(); ++i)
    EXPECT_EQ(expected_cat[i], cat[i]) << "for i=" << i;
}
