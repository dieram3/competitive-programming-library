//          Copyright Diego Ramirez 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cpl/sorting/counting_sort.hpp>
#include <gtest/gtest.h>

#include <algorithm> // minmax_element
#include <cstddef>   // size_t
#include <utility>   // pair
#include <vector>    // vector

using cpl::counting_sort;
using std::size_t;

TEST(CountingSortTest, Sorts) {
  using vec_t = std::vector<int>;

  vec_t vec = {2, 4, 1, 5, 14, 3, 0, 23, 7, -1, -8, -5, 5, 2, 2};
  ASSERT_EQ(15, vec.size());

  const auto minmax = std::minmax_element(vec.begin(), vec.end());
  const int base = *minmax.first;
  const size_t num_keys = *minmax.second - *minmax.first + 1;
  auto get_key = [base](const int x) -> size_t { return x - base; };

  counting_sort(vec.begin(), vec.end(), num_keys, get_key);

  EXPECT_EQ(vec_t({-8, -5, -1, 0, 1, 2, 2, 2, 3, 4, 5, 5, 7, 14, 23}), vec);
}

TEST(CountingSortTest, SortsStably) {
  using value_t = std::pair<int, char>;
  using vec_t = std::vector<value_t>;

  vec_t vec = {{2, 'x'}, {3, 'b'}, {2, 'b'}, {2, 'b'}, {5, 'a'}, {9, 'c'},
               {1, 'w'}, {3, 'a'}, {9, 'p'}, {0, 'z'}, {3, 'm'}, {4, 's'},
               {7, 'd'}, {9, 'f'}, {4, 'q'}, {6, 'h'}, {4, 'z'}, {4, 'g'}};

  ASSERT_EQ(18, vec.size());

  const vec_t expected = {
      {0, 'z'}, {1, 'w'}, {2, 'x'}, {2, 'b'}, {2, 'b'}, {3, 'b'},
      {3, 'a'}, {3, 'm'}, {4, 's'}, {4, 'q'}, {4, 'z'}, {4, 'g'},
      {5, 'a'}, {6, 'h'}, {7, 'd'}, {9, 'c'}, {9, 'p'}, {9, 'f'},
  };

  auto get_key = [](const value_t& val) -> size_t { return val.first; };
  const size_t num_keys = 10;

  ASSERT_NE(expected, vec);
  counting_sort(vec.begin(), vec.end(), num_keys, get_key);
  EXPECT_EQ(expected, vec);
}
