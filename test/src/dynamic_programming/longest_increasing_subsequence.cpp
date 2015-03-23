//          Copyright Diego Ramírez March 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <djp/dynamic_programming/longest_increasing_subsequence.hpp>
#include <boost/test/unit_test.hpp>
#include <vector>
#include <numeric>

BOOST_AUTO_TEST_SUITE(dynamic_programming)
BOOST_AUTO_TEST_SUITE(longest_increasing_subsequence)

BOOST_AUTO_TEST_CASE(longest_increasing_subsequence_size) {
  std::vector<int> seq = {0, 8, 4, 12, 2, 10, 6, 14, 1, 9, 5, 13, 3, 11, 7, 15};
  BOOST_CHECK_EQUAL(djp::calc_lis_size(seq.begin(), seq.end()), 6U);

  std::vector<int> vec(4096);
  std::iota(vec.begin(), vec.end(), 0);
  BOOST_CHECK_EQUAL(djp::calc_lis_size(vec.begin(), vec.end()), vec.size());
  BOOST_CHECK_EQUAL(djp::calc_lis_size(vec.rbegin(), vec.rend()), 1U);
}

BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
