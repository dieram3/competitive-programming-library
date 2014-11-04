//          Copyright Diego Ramírez November 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <djp/string/edit_distance.hpp>
#include <boost/test/unit_test.hpp>
#include <string>
#include <algorithm>

BOOST_AUTO_TEST_SUITE(string)
BOOST_AUTO_TEST_SUITE(edit_distance)

BOOST_AUTO_TEST_CASE(levenshtein_distance) {
  auto lev_dist = [](const std::string& str1, const std::string& str2) {
    return djp::levenshtein_distance(str1.begin(), str1.end(), str2.begin(),
                                     str2.end());
  };

  BOOST_CHECK_EQUAL(lev_dist("kitten", "sitting"), 3u);
  BOOST_CHECK_EQUAL(lev_dist("in...suv...dell", "ins...sub...del"), 3u);
  BOOST_CHECK_EQUAL(lev_dist("hello world", "hallo word"), 2u);
  BOOST_CHECK_EQUAL(lev_dist("equal", "equal"), 0u);
  BOOST_CHECK_EQUAL(lev_dist("non swap no", "oon swap nn"), 2u);
  BOOST_CHECK_EQUAL(lev_dist("max_cost...", "12345678"), 11u);

  std::string str1 = "kitten";
  std::string str2 = "sitting";
  std::reverse(str2.begin(), str2.end());
  // This must compile
  auto ld = djp::levenshtein_distance(str1.begin(), str1.end(), str2.rbegin(),
                                      str2.rend());
  BOOST_CHECK_EQUAL(ld, 3u);
}

BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
