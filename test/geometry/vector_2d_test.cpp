//          Copyright Diego Ramirez 2016
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cpl/geometry/vector_2d.hpp>
#include <gtest/gtest.h>

#include <cmath>   // sqrt
#include <sstream> // ostringstream

using cpl::vector2d;

namespace {
class Vector2DTest : public ::testing::Test {
protected:
  using vec = vector2d<int>;
};
} // end anonymous namespace

TEST_F(Vector2DTest, ValueConstructorTest) {
  vec v{};
  EXPECT_EQ(0, v.x);
  EXPECT_EQ(0, v.y);
}

TEST_F(Vector2DTest, PairConstructorTest) {
  {
    vec v{0, 0};
    EXPECT_EQ(0, v.x);
    EXPECT_EQ(0, v.y);
  }
  {
    vec v{0, 2};
    EXPECT_EQ(0, v.x);
    EXPECT_EQ(2, v.y);
  }
  {
    vec v{3, 0};
    EXPECT_EQ(3, v.x);
    EXPECT_EQ(0, v.y);
  }
  {
    vec v{4, 5};
    EXPECT_EQ(4, v.x);
    EXPECT_EQ(5, v.y);
  }
}

TEST_F(Vector2DTest, NegateOperatorTest) {
  EXPECT_EQ(vec(0, 0), -vec(0, 0));

  EXPECT_EQ(vec(0, -1), -vec(0, 1));
  EXPECT_EQ(vec(-1, 0), -vec(1, 0));
  EXPECT_EQ(vec(0, 1), -vec(0, -1));
  EXPECT_EQ(vec(1, 0), -vec(-1, 0));

  EXPECT_EQ(vec(-3, -5), -vec(3, 5));
  EXPECT_EQ(vec(-3, 5), -vec(3, -5));
  EXPECT_EQ(vec(3, -5), -vec(-3, 5));
  EXPECT_EQ(vec(3, 5), -vec(-3, -5));
}

TEST_F(Vector2DTest, AdditionTest) {
  EXPECT_EQ(vec(0, 2), vec(2, 3) + vec(-2, -1));
  EXPECT_EQ(vec(7, 13), vec(4, 6) + vec(3, 7));
  EXPECT_EQ(vec(-2, 5), vec(0, 0) + vec(-2, 5));
  EXPECT_EQ(vec(0, 0), vec(12, -53) + vec(-12, 53));
}

TEST_F(Vector2DTest, SubtractionTest) {
  EXPECT_EQ(vec(4, 4), vec(2, 3) - vec(-2, -1));
  EXPECT_EQ(vec(1, -1), vec(4, 6) - vec(3, 7));
  EXPECT_EQ(vec(2, -5), vec(0, 0) - vec(-2, 5));
  EXPECT_EQ(vec(-2, 5), vec(-2, 5) - vec(0, 0));
  EXPECT_EQ(vec(24, -106), vec(12, -53) - vec(-12, 53));
  EXPECT_EQ(vec(0, 0), vec(12, -53) - vec(12, -53));
}

TEST_F(Vector2DTest, ScalarProductTest) {
  EXPECT_EQ(vec(8, 12), vec(2, 3) * 4);
  EXPECT_EQ(vec(-14, 35), vec(-2, 5) * 7);
  EXPECT_EQ(vec(-15, 40), vec(-3, 8) * 5);
  EXPECT_EQ(vec(0, 84), vec(0, 42) * 2);
  EXPECT_EQ(vec(84, 0), vec(42, 0) * 2);
  EXPECT_EQ(vec(0, 0), vec(0, 0) * 2);

  EXPECT_EQ(vec(8, 12), 4 * vec(2, 3));
  EXPECT_EQ(vec(-14, 35), 7 * vec(-2, 5));
  EXPECT_EQ(vec(-15, 40), 5 * vec(-3, 8));
  EXPECT_EQ(vec(0, 84), 2 * vec(0, 42));
  EXPECT_EQ(vec(84, 0), 2 * vec(42, 0));
  EXPECT_EQ(vec(0, 0), 2 * vec(0, 0));
}

TEST_F(Vector2DTest, DivisonTest) {
  using vecf = vector2d<float>;
  {
    const auto v = vecf{0, 0} / 2.0f;
    EXPECT_FLOAT_EQ(0, v.x);
    EXPECT_FLOAT_EQ(0, v.y);
  }
  {
    const auto v = vecf{7, 11} / 2.0f;
    EXPECT_FLOAT_EQ(3.5f, v.x);
    EXPECT_FLOAT_EQ(5.5f, v.y);
  }
  {
    const auto v = vecf{12, 36} / 30.0f;
    EXPECT_FLOAT_EQ(0.4f, v.x);
    EXPECT_FLOAT_EQ(1.2f, v.y);
  }
  {
    const auto v = vecf{-4.746f, 22.764f} / 4.2f;
    EXPECT_FLOAT_EQ(-1.13f, v.x);
    EXPECT_FLOAT_EQ(5.42f, v.y);
  }
}

TEST_F(Vector2DTest, DotProductTest) {
  EXPECT_EQ(0, dot(vec{0, 0}, vec{0, 0}));
  EXPECT_EQ(0, dot(vec{2, 0}, vec{0, 3}));
  EXPECT_EQ(0, dot(vec{0, 5}, vec{4, 0}));

  EXPECT_EQ(47, dot(vec{3, 5}, vec{4, 7}));
  EXPECT_EQ(-47, dot(vec{-3, 5}, vec{4, -7}));
  EXPECT_EQ(47, dot(vec{-3, -5}, vec{-4, -7}));

  EXPECT_EQ(55, dot(vec{0, 5}, vec{4, 11}));
  EXPECT_EQ(52, dot(vec{13, 0}, vec{4, 11}));

  EXPECT_EQ(0, dot(vec{12, 4}, vec{5, -15}));
}

TEST_F(Vector2DTest, CrossProductTest) {
  EXPECT_EQ(0, cross(vec{0, 0}, vec{0, 0}));
  EXPECT_EQ(0, cross(vec{0, 0}, vec{1, 3}));
  EXPECT_EQ(0, cross(vec{4, 5}, vec{0, 0}));

  EXPECT_EQ(0, cross(vec{4, 5}, vec{8, 10}));
  EXPECT_EQ(0, cross(vec{4, 5}, vec{-12, -15}));

  EXPECT_EQ(-80, cross(vec{4, 5}, vec{8, -10}));
  EXPECT_EQ(41, cross(vec{4, 5}, vec{-5, 4}));

  EXPECT_EQ(-36, cross(vec{5, 13}, vec{7, 11}));
  EXPECT_EQ(36, cross(vec{7, 11}, vec{5, 13}));
}

TEST_F(Vector2DTest, NormTest) {
  EXPECT_EQ(0, norm(vec{0, 0}));

  EXPECT_EQ(25, norm(vec{5, 0}));
  EXPECT_EQ(16, norm(vec{0, 4}));
  EXPECT_EQ(170, norm(vec{11, 7}));

  EXPECT_EQ(9, norm(vec{-3, 0}));
  EXPECT_EQ(49, norm(vec{0, -7}));
  EXPECT_EQ(52, norm(vec{-6, -4}));

  EXPECT_EQ(34, norm(vec{3, -5}));
  EXPECT_EQ(17, norm(vec{-1, 4}));
}

TEST_F(Vector2DTest, AbsTest) {
  using vecf = vector2d<float>;

  EXPECT_FLOAT_EQ(0, abs(vecf{0, 0}));

  EXPECT_FLOAT_EQ(2, abs(vecf{2, 0}));
  EXPECT_FLOAT_EQ(3, abs(vecf{-3, 0}));
  EXPECT_FLOAT_EQ(5, abs(vecf{0, 5}));
  EXPECT_FLOAT_EQ(7, abs(vecf{0, -7}));

  EXPECT_FLOAT_EQ(5, abs(vecf{3, 4}));
  EXPECT_FLOAT_EQ(5, abs(vecf{-3, 4}));
  EXPECT_FLOAT_EQ(5, abs(vecf{3, -4}));
  EXPECT_FLOAT_EQ(5, abs(vecf{-3, -4}));

  EXPECT_FLOAT_EQ(std::sqrt(32.0f), abs(vecf{4, 4}));
  EXPECT_FLOAT_EQ(std::sqrt(74.0f), abs(vecf{5, -7}));
  EXPECT_FLOAT_EQ(std::sqrt(109.0f), abs(vecf{-3, 10}));
  EXPECT_FLOAT_EQ(std::sqrt(370.0f), abs(vecf{-17, -9}));
}

TEST_F(Vector2DTest, EqualityOperatorTest) {
  EXPECT_TRUE(vec(0, 0) == vec(0, 0));
  EXPECT_TRUE(vec(1, 2) == vec(1, 2));
  EXPECT_TRUE(vec(4, 7) == vec(4, 7));

  EXPECT_FALSE(vec(0, 0) == vec(0, 1));
  EXPECT_FALSE(vec(0, 0) == vec(1, 0));
  EXPECT_FALSE(vec(1, 2) == vec(2, 1));
  EXPECT_FALSE(vec(4, 7) == vec(7, 4));
}

TEST_F(Vector2DTest, InequalityOperatorTest) {
  EXPECT_FALSE(vec(0, 0) != vec(0, 0));
  EXPECT_FALSE(vec(1, 2) != vec(1, 2));
  EXPECT_FALSE(vec(4, 7) != vec(4, 7));

  EXPECT_TRUE(vec(0, 0) != vec(0, 1));
  EXPECT_TRUE(vec(0, 0) != vec(1, 0));
  EXPECT_TRUE(vec(1, 2) != vec(2, 1));
  EXPECT_TRUE(vec(4, 7) != vec(7, 4));
}

TEST_F(Vector2DTest, SerializationTest) {
  auto serialize = [](const vec& v) {
    std::ostringstream oss;
    oss << v;
    return oss.str();
  };

  EXPECT_EQ("(0,0)", serialize({0, 0}));
  EXPECT_EQ("(0,-1)", serialize({0, -1}));
  EXPECT_EQ("(4,0)", serialize({4, 0}));

  EXPECT_EQ("(2,3)", serialize({2, 3}));
  EXPECT_EQ("(-2,3)", serialize({-2, 3}));
  EXPECT_EQ("(2,-3)", serialize({2, -3}));
  EXPECT_EQ("(-2,-3)", serialize({-2, -3}));
}
