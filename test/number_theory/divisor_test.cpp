//          Copyright Diego Ramírez June 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <djp/number_theory/divisor.hpp>
#include <gtest/gtest.h>

#include <djp/number_theory/basics.hpp>       // for djp::multiply_less
#include <djp/number_theory/eratosthenes.hpp> // for djp::sieve_of_eratosthenes
#include <djp/utility/basics.hpp>             // for djp::repeat

#include <algorithm> // for std::sort
#include <random>    // for std::mt19937
#include <stdexcept> // for std::logic_error
#include <vector>    // for std::vector
#include <cstddef>   // for std::size_t
#include <cstdint>   //for std::uint32_t

using namespace djp;

template <class T>
static std::size_t naive_count_divisors(T n) {
  T d = 1;
  std::size_t ans = 0;
  for (; multiply_less(d, d, n); ++d) {
    if (n % d == 0)
      ans += 2;
  }
  if (n / d == d)
    ++ans;
  return ans;
}

TEST(count_divisors, WorksWell) {
  const auto sieve = sieve_of_eratosthenes<uint32_t>(66000);

  for (uint32_t n = 1; n < 1000; ++n) {
    const auto count0 = naive_count_divisors(n);
    const auto count1 = count_divisors(n);
    const auto count2 = count_divisors(n, sieve);
    EXPECT_EQ(count0, count1) << "n=" << n;
    EXPECT_EQ(count0, count2) << "n=" << n;
  }

  std::mt19937 gen;
  repeat(1000, [&gen, &sieve] {
    std::uniform_int_distribution<uint32_t> dist;
    const auto n = dist(gen);
    const auto count1 = count_divisors(n);
    const auto count2 = count_divisors(n, sieve);
    EXPECT_EQ(count1, count2) << "n=" << n;
  });

  const auto small_sieve = sieve_of_eratosthenes<uint32_t>(30);
  for (uint32_t n = 1; n < 842; ++n)
    EXPECT_EQ(naive_count_divisors(n), count_divisors(n, small_sieve));
  EXPECT_EQ(2, count_divisors(827, small_sieve));
  EXPECT_EQ(2, count_divisors(829, small_sieve));
  EXPECT_EQ(2, count_divisors(839, small_sieve));
  EXPECT_THROW(count_divisors(853, small_sieve), std::logic_error);
}

TEST(find_divisors, WorksWell) {
  auto sieve = sieve_of_eratosthenes<uint32_t>(66000);
  using vec = std::vector<std::uint32_t>;

  auto divisors_of = [&sieve](std::uint32_t n) {
    auto divs = djp::find_divisors(n, sieve);
    std::sort(divs.begin(), divs.end());
    return divs;
  };

  EXPECT_EQ(vec({1, 2}), divisors_of(2));
  EXPECT_EQ(vec({1, 2, 3, 4, 6, 12}), divisors_of(12));
  EXPECT_EQ(vec({1, 2, 3, 4, 6, 8, 12, 24}), divisors_of(24));
  EXPECT_EQ(vec({1, 4729, 21379, 101101291}), divisors_of(101101291));

  EXPECT_EQ(vec({1, INT32_MAX}), divisors_of(INT32_MAX));
  EXPECT_EQ(vec({1, 2, 4, 13, 26, 52}), divisors_of(52));
  EXPECT_EQ(32, divisors_of(UINT32_MAX).size());
}
