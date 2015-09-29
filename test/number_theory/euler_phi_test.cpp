//          Copyright Diego Ramírez September 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <djp/number_theory/euler_phi.hpp>
#include <gtest/gtest.h>

#include <djp/number_theory/eratosthenes.hpp>
#include <djp/number_theory/trial_division.hpp>
#include <vector>  // For std::vector
#include <cstdint> // For std::uint_fast32_t

using namespace djp;

namespace {
class EulerPhiTest : public ::testing::Test {
  using int_t = std::uint_fast32_t;
  using vec_t = std::vector<int_t>;

protected:
  void set_sieve_limit(const int_t limit) {
    sieve = sieve_of_eratosthenes(limit);
  }

  int_t totient(int_t n) const {
    const auto prime_factors = trial_division(n, sieve);
    return euler_phi(n, prime_factors);
  }

private:
  vec_t sieve;
};
} // end anonymous namespace

TEST_F(EulerPhiTest, SmallIntegersTest) {
  set_sieve_limit(100);
  EXPECT_EQ(1, totient(1));
  EXPECT_EQ(1, totient(2));
  EXPECT_EQ(2, totient(3));
  EXPECT_EQ(2, totient(4));
  EXPECT_EQ(4, totient(5));
  EXPECT_EQ(2, totient(6));
  EXPECT_EQ(6, totient(7));
  EXPECT_EQ(4, totient(8));
  EXPECT_EQ(6, totient(9));
  EXPECT_EQ(4, totient(10));
  EXPECT_EQ(10, totient(11));
  EXPECT_EQ(16, totient(32));
  EXPECT_EQ(32, totient(64));
  EXPECT_EQ(32, totient(120));
  EXPECT_EQ(1296, totient(1417));
  EXPECT_EQ(5986, totient(5987));
  EXPECT_EQ(5264, totient(7899));
  EXPECT_EQ(2400, totient(9000));
}

TEST_F(EulerPhiTest, MediumIntegersTest) {
  set_sieve_limit(11000); // Covers at least upto 100M
  EXPECT_EQ(18432, totient(59982));
  EXPECT_EQ(96960, totient(298398));
  EXPECT_EQ(127008, totient(444528)); // 2^4 * 3^4 * 7^3
  EXPECT_EQ(1580128, totient(1598173));
  EXPECT_EQ(23598612, totient(23598613));
  EXPECT_EQ(66897672, totient(78047291));
}
