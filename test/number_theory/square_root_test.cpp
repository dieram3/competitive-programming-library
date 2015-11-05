//          Copyright Diego Ramírez September 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <djp/number_theory/square_root.hpp>
#include <gtest/gtest.h>

#include <initializer_list> // For std::initializer_list
#include <iostream>         // For std::ostream
#include <cassert>          // For assert
#include <cstdint>          // For std::uint_fast32_t, std::uint_fast64_t

using namespace djp;
using std::uint_fast32_t;
using std::uint_fast64_t;

namespace {
class rare_int {
  unsigned value;

public:
  constexpr rare_int(unsigned init_value = 0) : value{init_value} {}

  constexpr explicit rare_int(int init_value)
      : value{static_cast<unsigned>(init_value)} {}

  constexpr explicit rare_int(double init_value)
      : value{static_cast<unsigned>(init_value)} {}

  constexpr unsigned get() const { return value; }

  rare_int &operator--() {
    --value;
    return *this;
  }
  rare_int &operator++() {
    ++value;
    return *this;
  }
};

static constexpr rare_int operator+(rare_int lhs, rare_int rhs) {
  return lhs.get() + rhs.get();
}
static constexpr rare_int operator-(rare_int lhs, rare_int rhs) {
  return lhs.get() - rhs.get();
}
static constexpr rare_int operator*(rare_int lhs, rare_int rhs) {
  return lhs.get() * rhs.get();
}
static constexpr rare_int operator<<(rare_int num, int b) {
  return num.get() << b;
}
static constexpr bool operator==(rare_int lhs, rare_int rhs) {
  return lhs.get() == rhs.get();
}
static constexpr bool operator<(rare_int lhs, rare_int rhs) {
  return lhs.get() < rhs.get();
}
static constexpr bool operator<=(rare_int lhs, rare_int rhs) {
  return lhs.get() <= rhs.get();
}
static constexpr bool operator>(rare_int lhs, rare_int rhs) {
  return lhs.get() > rhs.get();
}

static double sqrt(rare_int x) {
  if (x.get() < 1000)
    return std::sqrt(x.get());
  if (x.get() % 2 == 0)
    return std::sqrt(x.get()) + 5;
  return std::sqrt(x.get()) - 5;
}

static std::ostream &operator<<(std::ostream &os, rare_int num) {
  return os << num.get();
}
} // end anonymous namespace

namespace std {
template <>
struct numeric_limits<rare_int> {
  static constexpr int digits = numeric_limits<unsigned>::digits;
};
template <>
struct make_unsigned<rare_int> {
  using type = rare_int;
};
} // end namespace std

namespace {
class IsqrtTest : public ::testing::Test {
protected:
  template <typename T>
  void check_sqrt(const T x) const {
    assert(x > 0);
    const T squared_x = x * x;
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

TEST_F(IsqrtTest, ZeroTest) { EXPECT_EQ(0, isqrt(0)); }

TEST_F(IsqrtTest, SmallIntegersTest) {
  check_sqrts({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
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

TEST_F(IsqrtTest, SqrtCorrectionTest) {
  rare_int even_int = 64232;
  rare_int odd_int = 45981;
  // Correct a computed sqrt a bit greater than actual.
  EXPECT_EQ(253, isqrt(even_int));
  // Correct a computed sqrt a bit smaller than actual.
  EXPECT_EQ(214, isqrt(odd_int));
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
//
//  auto start = steady_clock::now();
//  uint64_t checksum = 0;
//  for (const auto x : values)
//    checksum += isqrt(x);
//  auto end = steady_clock::now();
//  auto elapsed = duration_cast<milliseconds>(end - start);
//
//  std::cout << "Elapsed: " << elapsed.count() << " ms\n";
//  std::cout << "Checksum: " << checksum << '\n';
//}

namespace {
class IsSquareTest : public ::testing::Test {
protected:
  template <typename T>
  void check_squares(std::initializer_list<T> ilist) {
    for (const T x : ilist)
      EXPECT_TRUE(is_square(x)) << " where x=" << x;
  }
  template <typename T>
  void check_non_squares(std::initializer_list<T> ilist) {
    for (const T x : ilist)
      EXPECT_FALSE(is_square(x)) << " where x=" << x;
  }
};
} // End anonymous namespace

TEST_F(IsSquareTest, ZeroTest) { check_squares({0}); }

TEST_F(IsSquareTest, SmallSquaresTest) {
  check_squares({1, 4, 9, 16, 25, 36, 49, 64, 81, 100, 121, 144, 169, 196, 225,
                 256, 289, 324, 361, 400, 441, 484, 529, 576, 625, 676, 729});
}

TEST_F(IsSquareTest, SmallNonSquaresTest) {
  check_non_squares({2, 3, 5, 6, 7, 8, 12, 15, 24, 30, 50, 65, 80, 99, 120, 140,
                     160, 190, 250, 300, 330, 390, 490, 514, 584, 631, 715});
}

TEST_F(IsSquareTest, Squares32Bits) {
  check_squares<uint_fast32_t>(
      {1156, 6241, 35721, 65536, 107584, 265225, 1595169, 12652249, 50751376,
       184660921, 463670089, 849780801, 1073676289, 1073741824, 2098830969,
       3587890201, 3807260209, 4294836225});
}

TEST_F(IsSquareTest, NonSquares32Bits) {
  check_non_squares<uint_fast32_t>(
      {1242, 3892, 5981, 19249, 35128, 48981, 67812, 78127, 101247, 148928,
       198289, 258785, 347818, 498278, 578127, 727244, 927408, 1257378, 1827483,
       19278378, 38478278, 122387187, 389157837, 678127512, 988341783,
       1992478347, 2389128482, 3489128948, 4294967295});
}

TEST_F(IsSquareTest, Squares64Bits) {
  check_squares<uint_fast64_t>(
      {4294967296, 6306265744, 7943800384, 9173999961, 31784471524,
       134804996964, 392785479076, 3177545109489, 13480507039561,
       54286215654724, 307190891567044, 2226567952829529, 9658135224259344,
       21840446422538496, 143335789873013956, 334242852122485201,
       798653098523688489, 1557187490833536969, 3931430320629776484,
       4611686014132420609, 4611686018427387904, 10481323195339531929u,
       18445618199572250625u});
}

TEST_F(IsSquareTest, NonSquares64Bits) {
  check_non_squares<uint_fast64_t>(
      {4294967297, 7337443117, 16641371412, 39524825001, 70228785496,
       129679814943, 264258476444, 462638789494, 1107698386094, 2655340299442,
       6304191970155, 13747369683052, 32785933598869, 55339029812777,
       123875570740775, 227636667257887, 365089784773493, 616991372406873,
       1302968973220290, 3000404658016918, 6009600928231760, 13451547522234477,
       25410446778492566, 44964788421034278, 96453411465667567,
       185093210767717305, 405820158479085247, 803445567972462505,
       1556812490099874719, 2707440361250768273, 4985834006240492906,
       11157418661094814491u, 18446744073709551615u});
}

//#include <chrono>
//#include <random>
// TEST_F(IsSquareTest, Benchmark) {
//  std::mt19937 gen;
//  std::uniform_int_distribution<uint64_t> dist;
//  std::vector<uint64_t> values(10000000);
//  for (auto &elem : values)
//    elem = dist(gen);
//
//  values.push_back(307190891567044);
//  values.push_back(2226567952829529);
//  values.push_back(9658135224259344);
//  values.push_back(21840446422538496);
//
//  using namespace std;
//  using namespace chrono;
//
//  auto start = steady_clock::now();
//  uint64_t checksum = 0;
//  for (const auto x : values)
//    checksum += is_square(x);
//  auto end = steady_clock::now();
//  auto elapsed = duration_cast<milliseconds>(end - start);
//
//  std::cout << "Elapsed: " << elapsed.count() << " ms\n";
//  std::cout << "Checksum: " << checksum << '\n';
//}
