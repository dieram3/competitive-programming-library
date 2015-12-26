//          Copyright Diego Ramírez 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <djp/strings/suffix_array.hpp>
#include <gtest/gtest.h>

#include <algorithm> // sort, lexicographical_compare
#include <cstddef>   // size_t
#include <numeric>   // iota
#include <string>    // string
#include <vector>    // vector

using namespace djp;
using std::size_t;

namespace {
class MakeSuffixArrayTest : public ::testing::Test {
protected:
  using suffix_array = std::vector<size_t>;

protected:
  static suffix_array naive_sa(const std::string &str) {
    suffix_array sa(str.size());
    std::iota(sa.begin(), sa.end(), size_t{0});

    auto suffix_less = [&str](const size_t lhs, const size_t rhs) {
      return std::lexicographical_compare(str.begin() + lhs, str.end(),
                                          str.begin() + rhs, str.end());
    };
    std::sort(sa.begin(), sa.end(), suffix_less);
    return sa;
  }
  static void check(const std::string &str) {
    EXPECT_EQ(naive_sa(str), make_suffix_array(str)) << "str=" << str;
  }
};
} // end anonymous namespace

TEST_F(MakeSuffixArrayTest, WorksOnEmptyStrings) {
  EXPECT_TRUE(make_suffix_array("").empty());
}

TEST_F(MakeSuffixArrayTest, ExplicitTest) {
  EXPECT_EQ(suffix_array({0}), make_suffix_array("x"));
  EXPECT_EQ(suffix_array({5, 3, 1, 0, 4, 2}), make_suffix_array("banana"));
  EXPECT_EQ(suffix_array({10, 7, 4, 1, 0, 9, 8, 6, 3, 5, 2}),
            make_suffix_array("mississippi"));
}

TEST_F(MakeSuffixArrayTest, EnglishWordsTest) {
  check("Accustom");
  check("Aeronautics");
  check("Ebullience");
  check("Effervescent");
  check("Efflorescence");
  check("Tintinnabulation");
}

TEST_F(MakeSuffixArrayTest, SeveralWordsTest) {
  check("Tintinnabulation_Tintinnabulation-TintinnabulationTintinnabulation");
  check("Lissome Lissome Lissome Lisssssome Lissomeee Lissome!Lissome!!");
  check("Dissemble Dissemble EloquenceEphemeral");
}

TEST_F(MakeSuffixArrayTest, BananaMixTest) {
  check("banananabanananabanabanababanananabananababanana");
  check("bananabananabananaD__bananabananabananaC_banana_bannnana");
  check("bananabananalbananalbananabanbanbanbanabanabanabanabanababababa");
}

TEST_F(MakeSuffixArrayTest, OnLetterTest) {
  check("aaaaaaaaaaaa");
  check("bbbbbbbb");
  check("CCCCCCCCCCCCC");
}

TEST_F(MakeSuffixArrayTest, TwoLettersTest) {
  check("aaaaaaaaaaaaaaaab");
  check("abababababababababab");
  check("aaaaaaaaaaabbbbbbbbbb");
  check("aaaabaaabaabaabaabbabababbaaaaaaaabaaaab");
}

TEST_F(MakeSuffixArrayTest, NullCharacterTest) {
  std::string str(10, '\0');
  check(str);
  str[2] = str[5] = str[8] = '\n';
  check(str);
}
