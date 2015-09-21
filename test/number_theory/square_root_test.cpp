//          Copyright Diego Ramírez September 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <djp/number_theory/square_root.hpp>
#include <gtest/gtest.h>

#include <initializer_list> // For std::initializer_list
#include <cassert>          // For assert
#include <cstdint>          // For std::uint_fast32_t, std::uint_fast64_t

using namespace djp;
using std::uint_fast32_t;
using std::uint_fast64_t;

namespace {

class IsqrtTest : public ::testing::Test {
protected:
  template <typename T>
  void check_sqrt(const T x) const {
    const T squared_x = x * x;
    assert(squared_x != 0);
    EXPECT_EQ(x - 1, isqrt(squared_x - 1));
    EXPECT_EQ(x, isqrt(squared_x));
    EXPECT_EQ(x, isqrt(squared_x + 1));
  }

  template <typename T>
  void check_sqrts(std::initializer_list<T> ilist) const {
    for (T x : ilist)
      check_sqrt(x);
  }
};

} // end anonymous namespace

TEST_F(IsqrtTest, ZeroTest) { EXPECT_EQ(0, isqrt(0u)); }

TEST_F(IsqrtTest, SmallIntegersTest) {
  check_sqrts<unsigned>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
}

TEST_F(IsqrtTest, Uint32Test) {
  check_sqrts<uint_fast32_t>({25, 79, 106, 349, 781, 1284, 1982, 2490, 3128,
                              5189, 10423, 15981, 22389, 29898, 35891, 37981,
                              43912, 48981, 52390, 55891, 59892, 60242, 65535});
}

TEST_F(IsqrtTest, Uint64Test) {
  check_sqrts<uint_fast64_t>(
      {65536, 69140, 123189, 251989, 499189, 798923, 1290423, 1928398, 3298148,
       7124982, 15892329, 47298125, 67108864, 123892412, 258981428, 378128124,
       512489214, 982587347, 1590239537, 2147483647, 2147483648, 3513187382,
       3928982489, 4294967295});
}

TEST_F(IsqrtTest, LimitsTest) {
  // If implemented as std::floor(std::sqrt(x)) this test probably will fail.
  EXPECT_EQ(4294967295, isqrt(18446744073709550592u));
  EXPECT_EQ(4294967295, isqrt(18446744073709551126u));
  EXPECT_EQ(4294967295, isqrt(18446744073709551518u));
  EXPECT_EQ(4294967295, isqrt(18446744073709551615u));
}

//#include <chrono>
//#include <random>
// TEST_F(IsqrtTest, Benchmark) {
//  std::mt19937 gen;
//  std::uniform_int_distribution<uint64_t> dist;
//  std::vector<uint64_t> values(10000000);
//  for (auto &elem : values)
//    elem = dist(gen);
//
//  using namespace std;
//  using namespace chrono;
//  auto start = steady_clock::now();
//  uint64_t checksum = 0;
//  for (const auto x : values)
//    checksum += isqrt(x);
//  auto end = steady_clock::now();
//  auto elapsed = duration_cast<milliseconds>(end - start);
//  std::cout << sizeof(double) << '\n';
//  std::cout << sizeof(long double) << '\n';
//  std::cout << "Elapsed: " << elapsed.count() << " ms\n";
//  std::cout << "Checksum: " << checksum << '\n';
//}
