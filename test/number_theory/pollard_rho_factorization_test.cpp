//          Copyright Diego Ramírez September 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cpl/number_theory/pollard_rho_factorization.hpp>
#include <gtest/gtest.h>

#include <initializer_list> // For std::initializer_list
#include <cassert>          // For assert
#include <cstdint>          // For std::uint64_t, UINT64_MAX

using namespace cpl;

namespace {
class PollardRhoFactorTest : public ::testing::Test {
protected:
  using int_t = std::uint64_t;

protected:
  static void check(int_t N) {
    assert(N > 1);
    const int_t factor = pollard_rho_factor(N);
    EXPECT_LT(1, factor) << "N=" << N;
    EXPECT_GT(N, factor) << "N=" << N;
    EXPECT_EQ(0, N % factor) << "N=" << N << ", found_factor=" << factor;
  }
  static void check(std::initializer_list<int_t> ilist) {
    for (const auto x : ilist)
      check(x);
  }
  static void check(unsigned bits, int_t delta) {
    assert(delta > 0);
    const int_t base = bits == 64 ? 0 : int_t(1) << bits;
    const int_t N = base - delta;
    check(N);
  }
  static void check(unsigned bits, std::initializer_list<int_t> deltas) {
    for (const auto delta : deltas)
      check(bits, delta);
  }
};
} // end anonymous namespace

TEST_F(PollardRhoFactorTest, SmallIntegersTest) {
  // Sequence A002808 in OEIS
  check({4, 6, 8, 9, 10, 12, 14, 15, 16, 18, 20, 21, 22, 24, 25, 26, 27, 28, 30,
         32, 33, 34, 35, 36, 38, 39, 40, 42, 44, 45, 46, 48, 49, 50, 51, 52, 54,
         55, 56, 57, 58, 60, 62, 63, 64, 65, 66, 68, 69, 70, 72, 74, 75, 76, 77,
         78, 80, 81, 82, 84, 85, 86, 87, 88, 90, 91, 92, 93, 94, 95, 96, 98, 99,
         100, 102, 104, 105, 106, 108, 110, 111, 112, 114, 115, 116, 117, 118,
         119, 120, 121, 122, 123, 124, 125, 126, 128, 129, 130, 132, 133, 134,
         135, 136, 138, 140, 141, 142, 143, 144, 145, 146, 147, 148, 150});
}

TEST_F(PollardRhoFactorTest, HighlyCompositeNumbersTest) {
  // Starting from 180
  // Sequence A002182 in OEIS
  check({180, 240, 360, 720, 840, 1260, 1680, 2520, 5040, 7560, 10080, 15120,
         20160, 25200, 27720, 45360, 50400, 55440, 83160, 110880, 166320,
         221760, 277200, 332640, 498960, 554400, 665280, 720720, 1081080,
         1441440, 2162160});
}

TEST_F(PollardRhoFactorTest, SquaresTest) {
  int_t list[] = {2,      3,      4,      5,       6,         7,        9,
                  10,     124,    304,    634,     873,       1928,     2489,
                  5341,   8934,   12237,  15344,   19587,     24985,    34899,
                  49834,  50749,  64892,  69834,   70128,     101498,   224982,
                  342829, 481287, 892287, 2918312, 151850097, 240095970};
  for (const auto x : list) {
    check(x * x);     // Perfect square
    check(2 * x * x); // Almost perfect square
    check(3 * x * x); // Almost perfect square
    check(4 * x * x); // Perfect square
    check(5 * x * x); // Almost perfect square
  }
  check(int_t(3037000499) * 3037000499);
}

TEST_F(PollardRhoFactorTest, TwoPrimesTest) {
  check(9223372036854774829); // 9251125413094057 * 997
  check(9223372036854773509); // 777228620279327 * 11867
  check(9223372036854531581); // 34264318463107 * 269183
  check(9223372036849325833); // 1099335888317 * 8389949
  check(9223372036846482341); // 68714427251 * 134227591
  check(9223372033830897791); // 3036944203 * 3037056797
}

TEST_F(PollardRhoFactorTest, DifferentBitsTest) {
  check(10, {1, 4, 12, 34, 97, 182, 464});
  check(20, {128, 493, 139, 382, 555, 893});
  check(30, {198, 269, 511, 689, 743, 899});
  check(40, {192, 238, 341, 472, 944, 127});
  check(50, {98, 102, 489, 589, 848, 983});
  check(51, {14, 14, 198, 238, 984, 413, 589});
  check(52, {12, 129, 187, 237, 478, 599, 730});
  check(53, {3, 15, 24, 124, 378, 918});
  check(54, {4, 12, 128, 378, 623, 934});
  check(55, {1, 4, 12, 42, 244, 258, 389, 549, 899});
  check(56, {31, 129, 238, 518, 798});
  check(57, {14, 38, 47, 89, 239, 589, 689, 983});
  check(58, {13, 25, 39, 43, 58, 173, 512, 912});
  check(59, {16, 34, 49, 192, 249, 591, 689, 798});
  check(60, {17, 24, 48, 73, 125, 259, 631, 991});
  check(61, {18, 30, 59, 94, 207, 349, 513, 615});
  check(62, {9, 12, 41, 98, 239, 398, 511, 797, 911});
  check(63, {7, 13, 15, 192, 227, 393, 439, 789, 993});
  check(64, {1, 2, 3, 4, 5, 6, 7, 10, 15, 310, 399, 712});
}

//#include <chrono>
//#include <random>
//#include <cpl/number_theory/miller_rabin.hpp>
// TEST_F(PollardRhoFactorTest, Benchmark) {
//  std::vector<int_t> values;
//
//  std::mt19937 engine(533439);
//  std::uniform_int_distribution<int_t> gen_value(4, UINT64_MAX);
//  while (values.size() < 3000) {
//    const auto N = gen_value(engine);
//    if (!miller_rabin_primality_test(N))
//      values.push_back(N);
//  }
//  //  for (int_t i = 2; i < 1000000; ++i)
//  //    if (!miller_rabin_primality_test(i))
//  //      values.push_back(i);
//
//  using namespace std::chrono;
//  auto start = steady_clock::now();
//  for (const auto num : values)
//    check(num);
//  auto end = steady_clock::now();
//  auto elapsed = duration_cast<milliseconds>(end - start);
//  std::cout << values.size() << " composite numbers were factored.\n";
//  std::cout << "Ellapsed time: " << elapsed.count() << " ms\n";
//}
