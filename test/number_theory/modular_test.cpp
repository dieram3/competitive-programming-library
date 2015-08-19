//          Copyright Diego Ramírez November 2014, May 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <djp/number_theory/modular.hpp>
#include <gtest/gtest.h>

#include <djp/utility/basics.hpp> // for djp::repeat

#include <random>  // for mt19937, uniform_int_distribution
#include <cstdint> // for uint32_t, SIZE_MAX

using namespace djp;

TEST(ModularTest, HandlesArithmeticOps) {
  using modular_t = modular<unsigned, 7>;

  const modular_t a = 4;
  const modular_t b = 12;

  EXPECT_TRUE(congruent(a, 4));
  EXPECT_TRUE(congruent(b, 5));
  EXPECT_TRUE(congruent(a + b, 2));
  EXPECT_TRUE(congruent(a - b, 6));
  EXPECT_TRUE(congruent(b - a, 1));
  EXPECT_TRUE(congruent(a * b, 6));
  EXPECT_TRUE(congruent(pow(a, 7 - 2), 2));
  EXPECT_TRUE(congruent(pow(b, 7 - 2), 3));
  EXPECT_TRUE(congruent(inverse(a), 2));
  EXPECT_TRUE(congruent(inverse(b), 3));
  EXPECT_TRUE(congruent(a / b, 5));
  EXPECT_TRUE(congruent(b / a, 3));
  EXPECT_EQ(4, static_cast<int>(a));
  EXPECT_EQ(5, static_cast<int>(b));
}

TEST(ModMulTest, WorksWell) {
  std::mt19937 gen;
  std::uniform_int_distribution<uint32_t> dist;

  /// \todo Improve this test, random data based testing is not a good idea and
  /// makes it slow.
  repeat(1234, [&gen, &dist] {
    const uint32_t a = dist(gen);
    const uint32_t b = dist(gen);
    constexpr uint32_t mod = 1000000000 + 7;
    EXPECT_EQ(uint64_t(a) * b % mod, mod_mul(a, b, mod));
  });
}

TEST(ModPowTest, WorksWhenOverflowDoesNotNeedControl) {
  EXPECT_EQ(90, mod_pow(2342, 14233, 2011));
  EXPECT_EQ(10817, mod_pow(2017, 1238912398, 65536));
  EXPECT_EQ(12, mod_pow(22342, 1238912398, 13));
  EXPECT_EQ(8001, mod_pow(25237, 131312, 65536));
  EXPECT_EQ(370, mod_pow(292, SIZE_MAX, 41202));
}

TEST(ModInverseTest, WorksWithPrimeModules) {

  EXPECT_EQ(1, mod_inverse(1, 2));

  EXPECT_EQ(2, mod_inverse(2, 3));

  EXPECT_EQ(2, mod_inverse(4, 7));
  EXPECT_EQ(4, mod_inverse(2, 7));

  EXPECT_EQ(8, mod_inverse(6, 47));
  EXPECT_EQ(6, mod_inverse(8, 47));

  EXPECT_EQ(22, mod_inverse(17, 373));
  EXPECT_EQ(17, mod_inverse(22, 373));

  EXPECT_EQ(144, mod_inverse(55, 7919));
  EXPECT_EQ(55, mod_inverse(144, 7919));

  EXPECT_EQ(1984127, mod_inverse(504, 1000000007));
  EXPECT_EQ(504, mod_inverse(1984127, 1000000007));

  EXPECT_EQ(32768, mod_inverse(65536, 2147483647));
  EXPECT_EQ(65536, mod_inverse(32768, 2147483647));
}
