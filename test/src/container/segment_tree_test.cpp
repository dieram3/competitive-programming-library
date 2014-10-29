//          Copyright Diego Ramírez July 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <djp/container/segment_tree.hpp>
#include <boost/test/unit_test.hpp>
#include <array>
#include <functional>

BOOST_AUTO_TEST_SUITE(container)

BOOST_AUTO_TEST_CASE(segment_tree) {

  std::array<int, 10> arr = {{1, 2, 3, 4, 5, 6, 7, 8, 9, 10}};
  djp::segment_tree<int, std::plus<int>> stree(arr.begin(), arr.end());

  BOOST_CHECK_EQUAL(stree.accumulate(0, 4), 10);
  BOOST_CHECK_EQUAL(stree.accumulate(6, 10), 34);
  BOOST_CHECK_EQUAL(stree.accumulate(0, 10), 55);

  auto multiply_by_2 = [](int& elem) { elem *= 2; };
  stree.for_each(0, 5, multiply_by_2);

  BOOST_CHECK_EQUAL(stree.accumulate(0, 1), 2);
  BOOST_CHECK_EQUAL(stree.accumulate(1, 3), 10);
  BOOST_CHECK_EQUAL(stree.accumulate(0, 5), 30);
  BOOST_CHECK_EQUAL(stree.accumulate(0, 10), 70);
}

BOOST_AUTO_TEST_SUITE_END()
