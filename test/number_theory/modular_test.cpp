//          Copyright Diego Ramírez November 2014, May 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <djp/number_theory/modular.hpp>
#include <gtest/gtest.h>

#include <djp/utility/basics.hpp> // for djp::repeat
#include <random>
#include <cstdint> // for std::uint32_t, SIZE_MAX

TEST(modular, HandlesArithmeticOps) {
  using modular_t = djp::modular<unsigned, 7>;

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

TEST(mod_mul, WorksWell) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<std::uint32_t> dist;

  djp::repeat(12345, [&gen, &dist] {
    const std::uint32_t a = dist(gen);
    const std::uint32_t b = dist(gen);
    constexpr std::uint32_t mod = 1000000000 + 7;
    EXPECT_EQ(std::uint64_t(a) * b % mod, djp::mod_mul(a, b, mod));
  });
}

TEST(mod_pow, WorksWhenOverflowDoesNotNeedControl) {
  EXPECT_EQ(90, djp::mod_pow<uint32_t>(2342, 14233, 2011));
  EXPECT_EQ(10817, djp::mod_pow<uint32_t>(2017, 1238912398, 65536));
  EXPECT_EQ(12, djp::mod_pow<uint32_t>(22342, 1238912398, 13));
  EXPECT_EQ(8001, djp::mod_pow<uint32_t>(25237, 131312, 65536));
  EXPECT_EQ(370, djp::mod_pow<uint32_t>(292, SIZE_MAX, 41202));
}
