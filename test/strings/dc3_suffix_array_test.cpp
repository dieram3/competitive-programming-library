//          Copyright Patricio Beckmann, Diego Ramírez August, September 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cpl/strings/dc3_suffix_array.hpp>
#include <gtest/gtest.h>

#include <algorithm> // For std::sort, std::lexicographical_compare
#include <cstddef>   // For std::size_t
#include <numeric>   // For std::iota
#include <string>    // For std::string
#include <vector>    // For std::vector

using namespace cpl;

namespace {
class DC3SuffixArrayTest : public ::testing::Test {
protected:
  using vec_t = std::vector<std::size_t>;

protected:
  static vec_t naive_sa(const std::string &str) {
    vec_t sa(str.size());
    std::iota(sa.begin(), sa.end(), std::size_t{0});

    auto suffix_less = [&str](const size_t lhs, const size_t rhs) {
      return std::lexicographical_compare(str.begin() + lhs, str.end(),
                                          str.begin() + rhs, str.end());
    };
    std::sort(sa.begin(), sa.end(), suffix_less);
    return sa;
  }
  static void check(const std::string &str) {
    EXPECT_EQ(naive_sa(str), dc3_suffix_array(str)) << "str=" << str;
  }
};
} // end anonymous namespace

TEST_F(DC3SuffixArrayTest, WorksOnEmptyStrings) {
  EXPECT_TRUE(dc3_suffix_array("").empty());
}

TEST_F(DC3SuffixArrayTest, ExplicitTest) {
  EXPECT_EQ(vec_t({0}), dc3_suffix_array("x"));
  EXPECT_EQ(vec_t({5, 3, 1, 0, 4, 2}), dc3_suffix_array("banana"));
  EXPECT_EQ(vec_t({10, 7, 4, 1, 0, 9, 8, 6, 3, 5, 2}),
            dc3_suffix_array("mississippi"));
}

TEST_F(DC3SuffixArrayTest, EnglishWordsTest) {
  check("Accustom");
  check("Aeronautics");
  check("Ebullience");
  check("Effervescent");
  check("Efflorescence");
  check("Tintinnabulation");
}

TEST_F(DC3SuffixArrayTest, SeveralWordsTest) {
  check("Tintinnabulation_Tintinnabulation-TintinnabulationTintinnabulation");
  check("Lissome Lissome Lissome Lisssssome Lissomeee Lissome!Lissome!!");
  check("Dissemble Dissemble EloquenceEphemeral");
}

TEST_F(DC3SuffixArrayTest, BananaMixTest) {
  check("banananabanananabanabanababanananabananababanana");
  check("bananabananabananaD__bananabananabananaC_banana_bannnana");
  check("bananabananalbananalbananabanbanbanbanabanabanabanabanababababa");
}

TEST_F(DC3SuffixArrayTest, OnLetterTest) {
  check("aaaaaaaaaaaa");
  check("bbbbbbbb");
  check("CCCCCCCCCCCCC");
}

TEST_F(DC3SuffixArrayTest, TwoLettersTest) {
  check("aaaaaaaaaaaaaaaab");
  check("abababababababababab");
  check("aaaaaaaaaaabbbbbbbbbb");
  check("aaaabaaabaabaabaabbabababbaaaaaaaabaaaab");
}

TEST_F(DC3SuffixArrayTest, NullCharacterTest) {
  std::string str(10, '\0');
  check(str);
  str[2] = str[5] = str[8] = '\n';
  check(str);
}

//#include <chrono>
//#include <random>
// TEST_F(DC3SuffixArrayTest, Benchmark) {
//  std::mt19937 engine;
//  std::bernoulli_distribution gen(0.7);
//  std::string str(1000000, '\0');
//  for (auto &letter : str)
//    letter = gen(engine) ? 'a' : 'b';
//
//  using namespace std::chrono;
//  auto start = steady_clock::now();
//  const vec_t sa = dc3_suffix_array(str);
//  auto end = steady_clock::now();
//  auto elapsed = duration_cast<milliseconds>(end - start);
//
//  unsigned long long checksum = 0;
//  for (const size_t suffix : sa) {
//    checksum *= 991;
//    checksum += suffix;
//    checksum %= 1000000007;
//  }
//
//  std::cout << "Elapsed time: " << elapsed.count() << " ms\n";
//  std::cout << "Checksum: " << checksum << '\n';
//}
