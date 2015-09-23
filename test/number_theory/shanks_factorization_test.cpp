//          Copyright Diego Ramírez September 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <djp/number_theory/shanks_factorization.hpp>
#include <gtest/gtest.h>

#include <initializer_list> // For std::initializer_list
#include <stdexcept>        // For std::overflow_error
#include <cassert>          // For assert
#include <cstdint>          // For std::int_fast64_t, std::int64_t

using namespace djp;

namespace {
class ShanksFactorizationTest : public ::testing::Test {
protected:
  using int_t = std::int_fast64_t;

protected:
  static void check(int_t N) {
    assert(N > 1);
    assert(N < (int_t(1) << 58));
    const int_t factor = shanks_factor(N);
    EXPECT_LT(1, factor);
    EXPECT_GT(N, factor);
    EXPECT_EQ(0, N % factor) << "N=" << N << ", found_factor=" << factor;
  }
  static void check(unsigned bits, int_t delta) {
    assert(delta > 0);
    const int_t N = (int_t(1) << bits) - delta;
    check(N);
  }
  static void check(unsigned bits, std::initializer_list<int_t> deltas) {
    for (const auto delta : deltas)
      check(bits, delta);
  }
};
} // end anonymous namespace

TEST_F(ShanksFactorizationTest, DifferentBitsTest) {
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
}

TEST_F(ShanksFactorizationTest, SquaresTest) {
  int_t list[] = {2,      3,      4,      5,       6,         7,        9,
                  10,     124,    304,    634,     873,       1928,     2489,
                  5341,   8934,   12237,  15344,   19587,     24985,    34899,
                  49834,  50749,  64892,  69834,   70128,     101498,   224982,
                  342829, 481287, 892287, 2918312, 151850097, 240095970};

  // Perfect squares
  for (const auto x : list) {
    check(x * x);
    check(4 * x * x);
  }
  // Almost perfect squares
  for (const auto x : list) {
    check(2 * x * x);
    check(3 * x * x);
    check(5 * x * x);
  }

  check(536870911LL * 536870911LL);
}

TEST_F(ShanksFactorizationTest, LargeMultiplierTest) {
  check(281128058351311643); // Needs k=17
  check(248243352878275597); // Needs k=19
  check(113644992211567699); // Needs k=20
  check(238167780628922377); // Needs k=21
  check(264639573038319179); // Needs k=23
  check(138849925491605621); // Needs k=26
}

TEST_F(ShanksFactorizationTest, LargePrimesTest) {
  check(26202761468337431LL * 11LL);
  check(870786634899431LL * 331LL);
  check(290848008225743LL * 991LL);
  check(26067683472163LL * 11057LL);
  check(14448362131019LL * 19949LL);
  check(12031154825383LL * 23957LL);
  check(9400553672473LL * 30661LL);
  check(2763818847523LL * 104287LL);
  check(853694687189LL * 337627LL);
  check(233077484153LL * 1236629LL);
  check(22336190273LL * 12904187LL);
  check(536870909LL * 536870879LL);
}

TEST_F(ShanksFactorizationTest, OverflowDetectionTest) {
  // Note that int64_t is put to force overflow.
  using std::int64_t;
  EXPECT_NO_THROW(shanks_factor<int64_t>(INT64_MAX - 31)); // k=1 is ok
  EXPECT_NO_THROW(shanks_factor<int64_t>(INT64_MAX - 59)); // k=1 is ok
  EXPECT_THROW(shanks_factor<int64_t>(INT64_MAX - 24), std::overflow_error);
  EXPECT_THROW(shanks_factor<int64_t>(INT64_MAX - 81), std::overflow_error);
}

//#include <chrono>
//#include <random>
//#include <djp/number_theory/miller_rabin.hpp>
// TEST_F(ShanksFactorizationTest, Benchmark) {
//  std::mt19937 gen(234872);
//  std::uniform_int_distribution<int_t> dist(2, (1LL << 58) - 1);
//  std::vector<int_t> values(2000);
//  for (auto &elem : values)
//    do {
//      elem = dist(gen);
//    } while (miller_rabin_primality_test(elem));
//
//  using namespace std::chrono;
//  auto start = steady_clock::now();
//  for (size_t i = 0; i < values.size(); ++i)
//    check(values[i]);
//  auto end = steady_clock::now();
//  auto elapsed = duration_cast<milliseconds>(end - start);
//  std::cout << "Ellapsed: " << elapsed.count() << " ms\n";
//}
