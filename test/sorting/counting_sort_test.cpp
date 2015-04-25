//          Copyright Diego Ramirez April 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <djp/sorting/counting_sort.hpp>
#include <gtest/gtest.h>
#include <iterator>
#include <type_traits>
#include <random>
#include <algorithm>
#include <functional>

namespace {

// Sort ints using LSD radix sort.
template <class RandomIt>
void sort_ints(RandomIt first, RandomIt last) {
  using T = typename std::iterator_traits<RandomIt>::value_type;
  static_assert(std::is_unsigned<T>::value, "T must be unsigned");
  constexpr size_t num_bits = std::numeric_limits<T>::digits;

  for (size_t sh = 0; sh != num_bits; sh += 8) {
    auto ith_byte = [sh](T elem) -> size_t { return (elem >> sh) & 0xFF; };
    djp::counting_sort(first, last, 256, ith_byte);
  }
}

}  // Anonymous namepace

TEST(counting_sort, Sorts) {
  std::vector<int> vec(100000);
  std::default_random_engine gen;
  std::uniform_int_distribution<int> dist(-1000, 1000);
  std::generate(vec.begin(), vec.end(), std::bind(dist, gen));

  auto key = [](int x) -> size_t { return x + 1000; };
  djp::counting_sort(vec.begin(), vec.end(), 2001, key);

  EXPECT_TRUE(std::is_sorted(vec.begin(), vec.end()));
}

TEST(counting_sort, SortsStably) {
  std::vector<unsigned> vec(100000);
  std::default_random_engine gen;
  std::uniform_int_distribution<unsigned> dist(
      0, std::numeric_limits<unsigned>::max());

  std::generate(vec.begin(), vec.end(), std::bind(dist, gen));

  sort_ints(vec.begin(), vec.end());

  EXPECT_TRUE(std::is_sorted(vec.begin(), vec.end()));
}
