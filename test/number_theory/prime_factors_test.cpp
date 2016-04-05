//          Copyright Diego Ramírez September 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cpl/number_theory/prime_factors.hpp>
#include <gtest/gtest.h>

#include <cpl/number_theory/miller_rabin.hpp>
#include <cpl/number_theory/shanks_factorization.hpp>
#include <algorithm>        // For std::is_sorted
#include <initializer_list> // For std::initializer_list
#include <vector>           // For std::vector
#include <cstdint>          // For std::int_fast64_t

using namespace cpl;

namespace {
class PrimeFactorsTest : public ::testing::Test {
protected:
  using int_t = std::int_fast64_t;
  using vec_t = std::vector<int_t>;

protected:
  static vec_t factor(int_t N) {
    auto is_prime = [](int_t n) { return miller_rabin_primality_test(n); };
    auto get_factor = [](int_t n) { return shanks_factor(n); };
    vec_t primes = find_prime_factors(N, is_prime, get_factor);
    EXPECT_TRUE(std::is_sorted(primes.begin(), primes.end()));
    return primes;
  }

  static void check(int_t N) {
    int_t rem = N;
    for (const auto p : factor(N)) {
      EXPECT_EQ(0, rem % p);
      EXPECT_TRUE(miller_rabin_primality_test(p)) << p << " is not a prime";
      rem /= p;
    }
    EXPECT_EQ(1, rem) << "N was not totally factored, where N=" << N;
  }

  static void check(std::initializer_list<int_t> ilist) {
    for (const auto n : ilist)
      check(n);
  }
};
} // End anonymous namespace

TEST_F(PrimeFactorsTest, ExplicitTest) {
  EXPECT_EQ(vec_t(), factor(1));
  EXPECT_EQ(vec_t({7}), factor(7));
  EXPECT_EQ(vec_t({7, 7}), factor(49));
  EXPECT_EQ(vec_t({7, 7, 7, 7}), factor(2401));
  EXPECT_EQ(vec_t({2, 2, 3}), factor(12));
  EXPECT_EQ(vec_t({37, 41, 101}), factor(153217));
  EXPECT_EQ(vec_t({2, 3, 5, 7, 7, 13, 13, 17}), factor(4223310));
}

TEST_F(PrimeFactorsTest, SmallNumbersTest) {
  check({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
  check({12, 78, 135, 293, 512, 785, 1100, 2234});
}

TEST_F(PrimeFactorsTest, MediumNumbersTest) {
  check({1287238, 10981298, 2347873, 129098, 13487287, 2827344, 87289821,
         2874324, 238782347, 2387483, 7314134});
}

TEST_F(PrimeFactorsTest, PerfectSquaresTest) {
  check({4, 9, 16, 25, 36, 49, 64, 81, 100});
  check({152324964, 9783781569});
}

//#include <chrono>
//#include <random>
// TEST_F(PrimeFactorsTest, Benchmark) {
//  std::mt19937 gen(187238);
//  std::uniform_int_distribution<int_t> dist(1, (1LL << 58) - 1);
//  std::vector<int_t> values(1000);
//  for (auto &elem : values)
//    elem = dist(gen);
//  using namespace std::chrono;
//  auto begin = steady_clock::now();
//  for (const auto num : values)
//    check(num);
//  auto end = steady_clock::now();
//  auto elapsed = duration_cast<milliseconds>(end - begin);
//  std::cout << "Elapsed: " << elapsed.count() << " ms\n";
//}
