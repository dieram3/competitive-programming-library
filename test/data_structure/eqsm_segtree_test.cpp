//          Copyright Diego Ramírez November 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <djp/data_structure/eqsm_segtree.hpp>
#include <gtest/gtest.h>

#include <functional> // For std::plus, std::multiplies, std::bit_xor
#include <string>     // For std::string
#include <vector>     // For std::vector
#include <cstddef>    // For std::size_t

using namespace djp;
using std::size_t;

namespace {
class EQSMSegtreeTest : public ::testing::Test {};
} // end anonymous namaspace

TEST_F(EQSMSegtreeTest, SizeIdemCtorTest) {
  using segtree_t = eqsm_segtree<int, std::plus<int>>;
  const int idem = 0; // Identity value for addition.
  segtree_t segtree(3, idem);
  EXPECT_EQ(3, segtree.size());
}

TEST_F(EQSMSegtreeTest, SizeIdemCombCtorTest) {
  using segtree_t = eqsm_segtree<int, std::plus<int>>;
  const int idem = 0; // Identity value for addition.
  segtree_t segtree(3, idem, std::plus<int>());
  EXPECT_EQ(3, segtree.size());
}

TEST_F(EQSMSegtreeTest, StoreTest) {
  using segtree_t = eqsm_segtree<int, std::plus<int>>;
  const int idem = 0; // Identity value for addition.
  segtree_t segtree(1, idem);

  EXPECT_EQ(1, segtree.size());

  std::vector<int> values{};
  segtree.store(values.begin(), values.end());
  EXPECT_EQ(0, segtree.size());

  values = {1, 2, 4, 8, 10};
  segtree.store(values.begin(), values.end());
  EXPECT_EQ(5, segtree.size());

  EXPECT_EQ(1, segtree.get(0));
  EXPECT_EQ(2, segtree.get(1));
  EXPECT_EQ(4, segtree.get(2));
  EXPECT_EQ(8, segtree.get(3));
  EXPECT_EQ(10, segtree.get(4));

  segtree.combine_with(4, 0, 5);
  segtree.combine_with(3, 0, 3);
  segtree.combine_with(7, 2, 5);

  const std::vector<int> new_values = {11, 42, 7, 9, 8, 13};
  segtree.store(new_values.begin(), new_values.end());

  ASSERT_EQ(6, new_values.size());

  EXPECT_EQ(11, segtree.get(0));
  EXPECT_EQ(42, segtree.get(1));
  EXPECT_EQ(7, segtree.get(2));
  EXPECT_EQ(9, segtree.get(3));
  EXPECT_EQ(8, segtree.get(4));
  EXPECT_EQ(13, segtree.get(5));
}

TEST_F(EQSMSegtreeTest, MultiplicationTest) {
  using segtree_t = eqsm_segtree<int, std::multiplies<int>>;
  const int idem = 1; // Identity value for multiplication.
  segtree_t segtree(0, idem);

  const std::vector<int> init_values = {1, 2, 3, 2, 4, 3, 2, 1, 1, 0, 3, 2};
  segtree.store(init_values.begin(), init_values.end());

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
  using segtree_t = eqsm_segtree<bool, std::bit_xor<bool>>;
  const int idem = 0; // Identity value for BitXor.
  segtree_t segtree(0, idem);

  const std::vector<bool> init_values = {1, 0, 0, 1, 1, 1, 0,
                                         0, 0, 1, 1, 0, 1, 1};
  segtree.store(init_values.begin(), init_values.end());
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
