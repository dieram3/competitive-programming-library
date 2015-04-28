#include <djp/strings/z_function.hpp>
#include <gtest/gtest.h>

#include <string>
#include <functional>
#include <algorithm>
#include <numeric>

TEST(z_function, AllCharactersEqual) {
  std::string str = "aaaaaaa";
  auto z = djp::z_function(str);
  EXPECT_TRUE(
      std::is_sorted(std::next(begin(z)), end(z), std::greater<size_t>()));
  auto n = str.size() - 1;
  EXPECT_EQ(std::accumulate(begin(z), end(z), 0ull), n * (n + 1) / 2);
}

TEST(z_function, AllCharactersDifferent) {
  std::string str = "abcdefghg";
  auto z = djp::z_function(str);
  EXPECT_EQ(std::accumulate(begin(z), end(z), 0ull), 0ull);
}

TEST(z_function, AllCharactersEqualBigInputs) {
  std::string str(100000, 'a');
  auto z = djp::z_function(str);
  EXPECT_TRUE(
      std::is_sorted(std::next(begin(z)), end(z), std::greater<size_t>()));
  auto n = str.size() - 1;
  EXPECT_EQ(std::accumulate(begin(z), end(z), 0ull), n * (n + 1) / 2);
}
