//          Copyright Diego Ramírez November 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <djp/data_structure/eqsm_segtree.hpp>
#include <gtest/gtest.h>

#include <functional> // For std::plus, std::bit_xor
#include <string>     // For std::string
#include <vector>     // For std::vector
#include <cstddef>    // For std::size_t

using namespace djp;
using std::size_t;

namespace {
class EQSMSegtreeTest : public ::testing::Test {};
} // end anonymous namaspace

TEST_F(EQSMSegtreeTest, RangeIdemCtorTest) {
  std::vector<int> values{};
  using segtree_t = eqsm_segtree<int, std::plus<int>>;
  const int idem = 0; // Identity value for addition.

  segtree_t segtree(values.begin(), values.end(), idem);
  EXPECT_EQ(0, segtree.size());

  values = {1, 2, 4, 8, 10};
  segtree = segtree_t(values.begin(), values.end(), idem);
  EXPECT_EQ(5, segtree.size());

  EXPECT_EQ(1, segtree.get(0));
  EXPECT_EQ(2, segtree.get(1));
  EXPECT_EQ(4, segtree.get(2));
  EXPECT_EQ(8, segtree.get(3));
  EXPECT_EQ(10, segtree.get(4));
}

TEST_F(EQSMSegtreeTest, RangeIdemCombinerCtorTest) {
  std::vector<int> values{};
  auto mult = [](int x, int y) { return x * y; };
  using segtree_t = eqsm_segtree<int, decltype(mult)>;
  const int idem = 1; // Identity value for multiplication.

  segtree_t segtree(values.begin(), values.end(), idem, mult);
  EXPECT_EQ(0, segtree.size());

  values = {2, 3, 8, 1, 4};
  segtree = segtree_t(values.begin(), values.end(), idem, mult);

  EXPECT_EQ(5, segtree.size());

  EXPECT_EQ(2, segtree.get(0));
  EXPECT_EQ(3, segtree.get(1));
  EXPECT_EQ(8, segtree.get(2));
  EXPECT_EQ(1, segtree.get(3));
  EXPECT_EQ(4, segtree.get(4));
}

TEST_F(EQSMSegtreeTest, MultiplicationTest) {
  const std::vector<int> values = {1, 2, 3, 2, 4, 3, 2, 1, 1, 0, 3, 2};
  auto mult = [](int x, int y) { return x * y; };
  const int idem = 1; // Identity value for multiplication.
  using segtree_t = eqsm_segtree<int, decltype(mult)>;

  segtree_t segtree(values.begin(), values.end(), idem, mult);
  ASSERT_EQ(12, segtree.size());

  // 1, 2, 3, 2, 4, 3, 2, 1, 1, 0, 3, 2
  segtree.combine_with(3, 0, 4);
  // 3, 6, 9, 6, 4, 3, 2, 1, 1, 0, 3, 2
  segtree.combine_with(2, 5, 10);
  // 3, 6, 9, 6, 4, 6, 4, 2, 2, 0, 3, 2
  segtree.combine_with(5, 8, 12);
  // 3, 6, 9, 6, 4, 6, 4, 2,10, 0,15,10
  segtree.combine_with(11, 4, 5);
  // 3, 6, 9, 6,44, 6, 4, 2,10, 0,15,10

  std::vector<int> expected = {3, 6, 9, 6, 44, 6, 4, 2, 10, 0, 15, 10};

  std::vector<int> actual(12);
  for (size_t i = 0; i < actual.size(); ++i)
    actual[i] = segtree.get(i);

  EXPECT_EQ(expected, actual);
}

TEST_F(EQSMSegtreeTest, BitXorTest) {
  const std::vector<bool> values = {1, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 0, 1, 1};
  const int idem = 0; // Identity value for BitXor.
  using segtree_t = eqsm_segtree<bool, std::bit_xor<bool>>;

  segtree_t segtree(values.begin(), values.end(), idem);
  ASSERT_EQ(14, segtree.size());

  // 1,0,0,1,1,1,0,0,0,1,1,0,1,1
  segtree.combine_with(false, 0, 10); // Does nothing
  // 1,0,0,1,1,1,0,0,0,1,1,0,1,1
  segtree.combine_with(true, 5, 12);
  // 1,0,0,1,1,0,1,1,1,0,0,1,1,1
  segtree.combine_with(true, 0, 6);
  // 0,1,1,0,0,1,1,1,1,0,0,1,1,1
  segtree.combine_with(true, 13, 14);
  // 0,1,1,0,0,1,1,1,1,0,0,1,1,0

  std::string expected = "01100111100110";

  std::string actual(14, '\0');
  for (size_t i = 0; i < actual.size(); ++i)
    actual[i] = segtree.get(i) ? '1' : '0';

  EXPECT_EQ(expected, actual);
}
