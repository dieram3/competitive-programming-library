//          Copyright Diego Ramírez July 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <djp/data_structure/segment_tree.hpp>
#include <gtest/gtest.h>
#include <array>
#include <numeric>
#include <functional>

using djp::segment_tree;

namespace {

static inline segment_tree<int, std::plus<int>>
make_iota_segment_tree(size_t sz) {
  std::vector<int> vec(sz);
  std::iota(vec.begin(), vec.end(), 1);
  return segment_tree<int, std::plus<int>>(vec.cbegin(), vec.cend());
}

} // End anonymous namespace

TEST(segment_tree, HandlesSum) {
  const auto stree = make_iota_segment_tree(10);
  EXPECT_EQ(10, stree.accumulate(0, 4));
  EXPECT_EQ(34, stree.accumulate(6, 10));
  EXPECT_EQ(55, stree.accumulate(0, 10));
}

TEST(segment_tree, HandlesForeach) {
  auto stree = make_iota_segment_tree(10);
  auto multiply_by_2 = [](int &elem) { elem *= 2; };
  stree.for_each(0, 5, multiply_by_2);
  EXPECT_EQ(2, stree.accumulate(0, 1));
  EXPECT_EQ(10, stree.accumulate(1, 3));
  EXPECT_EQ(30, stree.accumulate(0, 5));
  EXPECT_EQ(70, stree.accumulate(0, 10));
}

//#include <djp/utility/basics.hpp>
//#include <algorithm>
//#include <chrono>
//#include <random>
//#include <cstdint>
//#include <iostream>
// TEST(segment_tree, Benchmark) {
//  std::vector<uint64x_t> vec(1000000);
//  std::random_device rd;
//  std::mt19937_64 gen(rd());
//  {
//    std::uniform_int_distribution<uint64_t> dist;
//    std::generate(vec.begin(), vec.end(), std::bind(dist, std::ref(gen)));
//  }
//
//  segment_tree<uint64_t, std::plus<uint64_t>> stree(vec.begin(), vec.end());
//  using dist_t = std::uniform_int_distribution<size_t>;
//  using param_t = dist_t::param_type;
//
//  using namespace std::chrono;
//  const auto start = steady_clock::now();
//  uint64_t sum = 0;
//  size_t range_len = 0;
//  djp::repeat(1000000, [&gen, &stree, &sum, &range_len] {
//    dist_t dist;
//    const auto first = dist(gen, param_t(0, stree.size() - 1));
//    const auto last = dist(gen, param_t(first + 1, stree.size()));
//    range_len += last - first;
//    sum += stree.accumulate(first, last);
//  });
//  const auto end = steady_clock::now();
//  duration<double, std::milli> elapsed(end - start);
//  std::cout << sum << ' ' << elapsed.count() << " ms\n";
//  std::cout << range_len / 1000000.0 << '\n';
//}
