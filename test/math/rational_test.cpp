//          Copyright Diego Ramírez October 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cpl/math/rational.hpp>
#include <gtest/gtest.h>

#include <iterator> // For std::back_inserter
#include <vector>   // For std::vector
#include <cstddef>  // for std::int_fast64_t

using namespace cpl;

namespace {
template <typename T>
struct raw_ratio {
  raw_ratio(T a, T b) : num{a}, den{b} {}
  T num, den;
};

template <typename T>
static bool operator==(const raw_ratio<T> &lhs, const rational<T> &rhs) {
  return lhs.num == rhs.numerator() && lhs.den == rhs.denominator();
}

template <typename T>
static std::ostream &operator<<(std::ostream &os, const raw_ratio<T> &r) {
  return os << r.num << '/' << r.den;
}
} // end anonymous namespace

namespace cpl {
template <typename T>
static std::ostream &operator<<(std::ostream &os, const rational<T> &r) {
  return os << r.numerator() << '/' << r.denominator();
}
} // end namespace cpl

namespace {
class RationalTest : public ::testing::Test {
protected:
  using int_t = std::int_fast64_t;
  using rational_t = rational<int_t>;
  using raw_ratio_t = raw_ratio<int_t>;
  using vec_t = std::vector<int_t>;

protected:
  static std::vector<int_t> cfrac(int_t a, int_t b = 1) {
    std::vector<int_t> coeffs;
    continued_fraction(rational_t(a, b), std::back_inserter(coeffs));
    return coeffs;
  }
};
} // end anonymous namespace

TEST_F(RationalTest, IntegerConstructorTest) {
  EXPECT_EQ(raw_ratio_t(5, 1), rational_t(5));
  EXPECT_EQ(raw_ratio_t(11, 1), rational_t(11));
  EXPECT_EQ(raw_ratio_t(0, 1), rational_t(0));
  EXPECT_EQ(raw_ratio_t(-5, 1), rational_t(-5));
  EXPECT_EQ(raw_ratio_t(-23, 1), rational_t(-23));
}

TEST_F(RationalTest, RatioConstructorTest) {
  EXPECT_EQ(raw_ratio_t(0, 1), rational_t(0, 1));
  EXPECT_EQ(raw_ratio_t(0, 1), rational_t(0, 3));
  EXPECT_EQ(raw_ratio_t(0, 1), rational_t(0, -1));
  EXPECT_EQ(raw_ratio_t(0, 1), rational_t(0, -4));
  EXPECT_EQ(raw_ratio_t(5, 4), rational_t(15, 12));
  EXPECT_EQ(raw_ratio_t(-2, 1), rational_t(-2, 1));
  EXPECT_EQ(raw_ratio_t(-3, 2), rational_t(12, -8));
  EXPECT_EQ(raw_ratio_t(3, 2), rational_t(-15, -10));
}

TEST_F(RationalTest, ReciprocalTest) {
  EXPECT_EQ(raw_ratio_t(5, 13), reciprocal(rational_t(13, 5)));
  EXPECT_EQ(raw_ratio_t(-5, 6), reciprocal(rational_t(12, -10)));
  EXPECT_EQ(raw_ratio_t(-2, 1), reciprocal(rational_t(-5, 10)));
  EXPECT_EQ(raw_ratio_t(5, 7), reciprocal(rational_t(-7, -5)));
}

TEST_F(RationalTest, AdditionTest) {
  EXPECT_EQ(raw_ratio_t(0, 1), rational_t(-2, 3) + rational_t(4, 6));
  EXPECT_EQ(raw_ratio_t(-4, 3), rational_t(2, 3) + rational_t(-2));
  EXPECT_EQ(raw_ratio_t(9, 7), rational_t(4, 7) + rational_t(5, 7));
  EXPECT_EQ(raw_ratio_t(1, 1), rational_t(8, 11) + rational_t(3, 11));
  EXPECT_EQ(raw_ratio_t(13, 2), rational_t(14, 4) + rational_t(9, 3));
  EXPECT_EQ(raw_ratio_t(47, 24), rational_t(7, 12) + rational_t(11, 8));
  EXPECT_EQ(raw_ratio_t(-49, 8), rational_t(-13, 8) + rational_t(-9, 2));
}

TEST_F(RationalTest, SubtractionTest) {
  EXPECT_EQ(raw_ratio_t(0, 1), rational_t(4, 3) - rational_t(8, 6));
  EXPECT_EQ(raw_ratio_t(-4, 3), rational_t(-2, 3) - rational_t(4, 6));
  EXPECT_EQ(raw_ratio_t(8, 3), rational_t(2, 3) - rational_t(-2));
  EXPECT_EQ(raw_ratio_t(-1, 7), rational_t(4, 7) - rational_t(5, 7));
  EXPECT_EQ(raw_ratio_t(5, 11), rational_t(8, 11) - rational_t(3, 11));
  EXPECT_EQ(raw_ratio_t(1, 2), rational_t(14, 4) - rational_t(9, 3));
  EXPECT_EQ(raw_ratio_t(-19, 24), rational_t(7, 12) - rational_t(11, 8));
  EXPECT_EQ(raw_ratio_t(23, 8), rational_t(-13, 8) - rational_t(-9, 2));
}

TEST_F(RationalTest, MultiplicationTest) {
  EXPECT_EQ(raw_ratio_t(0, 1), rational_t(0) * rational_t(0));
  EXPECT_EQ(raw_ratio_t(0, 1), rational_t(2, 3) * rational_t(0));
  EXPECT_EQ(raw_ratio_t(0, 1), rational_t(0) * rational_t(-5, 9));
  EXPECT_EQ(raw_ratio_t(1, 1), rational_t(3, 4) * rational_t(4, 3));
  EXPECT_EQ(raw_ratio_t(-2, 1), rational_t(8, 5) * rational_t(-10, 8));
  EXPECT_EQ(raw_ratio_t(35, 96), rational_t(5, 12) * rational_t(7, 8));
  EXPECT_EQ(raw_ratio_t(10, 21), rational_t(5, 12) * rational_t(8, 7));
  EXPECT_EQ(raw_ratio_t(-3, 1), rational_t(-4) * rational_t(3, 4));
  EXPECT_EQ(raw_ratio_t(4, 3), rational_t(-4, 5) * rational_t(-10, 6));
  EXPECT_EQ(raw_ratio_t(-2, 1), rational_t(6, 7) * rational_t(-7, 3));
}

TEST_F(RationalTest, DivisionTest) {
  EXPECT_EQ(raw_ratio_t(0, 1), rational_t(0) / rational_t(1));
  EXPECT_EQ(raw_ratio_t(0, 1), rational_t(0) / rational_t(-1));
  EXPECT_EQ(raw_ratio_t(-1, 1), rational_t(2, 3) / rational_t(-2, 3));
  EXPECT_EQ(raw_ratio_t(-5, 6), rational_t(-2, 3) / rational_t(4, 5));
  EXPECT_EQ(raw_ratio_t(9, 25), rational_t(-3, 5) / rational_t(-5, 3));
  EXPECT_EQ(raw_ratio_t(2, 5), rational_t(4, 15) / rational_t(2, 3));
  EXPECT_EQ(raw_ratio_t(59, 58), rational_t(59, 60) / rational_t(58, 60));
  EXPECT_EQ(raw_ratio_t(-1, 2), rational_t(49, 60) / rational_t(-49, 30));
  EXPECT_EQ(raw_ratio_t(-15, 1), rational_t(5) / rational_t(-1, 3));
  EXPECT_EQ(raw_ratio_t(5, 4), rational_t(15) / rational_t(12));
}

TEST_F(RationalTest, EvaluateContinuedFractionTest) {
  auto evaluate = [](const std::vector<int_t> &coeffs) {
    return evaluate_continued_fraction(coeffs);
  };
  EXPECT_EQ(raw_ratio_t(0, 1), evaluate({0}));
  EXPECT_EQ(raw_ratio_t(15, 1), evaluate({15}));
  EXPECT_EQ(raw_ratio_t(14, 5), evaluate({2, 1, 4}));
  EXPECT_EQ(raw_ratio_t(93, 101), evaluate({0, 1, 11, 1, 1, 1, 2}));
  EXPECT_EQ(raw_ratio_t(513, 254), evaluate({2, 50, 1, 4}));
  EXPECT_EQ(raw_ratio_t(70, 13), evaluate({5, 2, 1, 1, 2}));
  EXPECT_EQ(raw_ratio_t(-513, 254), evaluate({-2, -50, -1, -4}));
  EXPECT_EQ(raw_ratio_t(-70, 13), evaluate({-5, -2, -1, -1, -2}));
}

TEST_F(RationalTest, ContinuedFractionTest) {
  EXPECT_EQ(vec_t({0}), cfrac(0));
  EXPECT_EQ(vec_t({5}), cfrac(5));
  EXPECT_EQ(vec_t({2, 1, 4}), cfrac(14, 5));
  EXPECT_EQ(vec_t({0, 1, 11, 1, 1, 1, 2}), cfrac(93, 101));
  EXPECT_EQ(vec_t({2, 50, 1, 4}), cfrac(513, 254));
  EXPECT_EQ(vec_t({5, 2, 1, 1, 2}), cfrac(70, 13));
  EXPECT_EQ(vec_t({-2, -50, -1, -4}), cfrac(-513, 254));
  EXPECT_EQ(vec_t({-5, -2, -1, -1, -2}), cfrac(-70, 13));
  EXPECT_EQ(vec_t({1, 7, 1, 2, 2, 18}), cfrac(1124, 995));

  EXPECT_EQ(vec_t({0, -1, -3, -10, -1, -1, -11, -2, -1, -2, -2, -1, -4}),
            cfrac(-98178, 129899));
  EXPECT_EQ(vec_t({3, 14, 1, 19, 1, 1, 1, 1, 3, 1, 1, 5, 1, 92, 1, 3, 5, 6, 4}),
            cfrac(12874195198, 4197810981));
}
