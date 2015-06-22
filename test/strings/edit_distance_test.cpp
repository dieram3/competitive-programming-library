//          Copyright Diego Ramírez November 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <djp/strings/edit_distance.hpp>
#include <gtest/gtest.h>
#include <string>
#include <algorithm>

TEST(levenshtein_distance, HandlesSmallInput) {
  auto lev_dist = [](const std::string &str1, const std::string &str2) {
    return djp::levenshtein_distance(str1.begin(), str1.end(), str2.begin(),
                                     str2.end());
  };

  EXPECT_EQ(3, lev_dist("kitten", "sitting"));
  EXPECT_EQ(3, lev_dist("in...suv...dell", "ins...sub...del"));
  EXPECT_EQ(2, lev_dist("hello world", "hallo word"));
  EXPECT_EQ(0, lev_dist("equal", "equal"));
  EXPECT_EQ(2, lev_dist("non swap no", "oon swap nn"));
  EXPECT_EQ(11, lev_dist("max_cost...", "12345678"));

  std::string str1 = "kitten";
  std::string str2 = "sitting";
  std::reverse(str2.begin(), str2.end());
  // This must compile
  const auto distance = djp::levenshtein_distance(str1.begin(), str1.end(),
                                                  str2.rbegin(), str2.rend());
  EXPECT_EQ(3, distance);
}

//#include <algorithm>
//#include <functional>
//#include <iostream>
//#include <chrono>
//#include <random>
//#include <string>
// TEST(levenshtein_distance, benchmark) {
//  std::string str1(5000, '\0');
//  std::string str2(6000, '\0');
//  std::default_random_engine urng(12387123);
//  std::uniform_int_distribution<char> dist('a', 'z');
//  auto gen = std::bind(dist, std::ref(urng));
//  std::generate(str1.begin(), str1.end(), gen);
//  std::generate(str2.begin(), str2.end(), gen);
//
//  using namespace std::chrono;
//  auto start = steady_clock::now();
//  auto distance = djp::levenshtein_distance(str1.begin(), str1.end(),
//                                            str2.begin(), str2.end());
//  auto end = steady_clock::now();
//  duration<double, std::milli> elapsed = end - start;
//  std::cout << distance << '\n'; // With libc++ 3.6 should be 4957
//  std::cout << elapsed.count() << "ms\n";
//}
