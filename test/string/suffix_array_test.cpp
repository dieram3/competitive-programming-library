#include <djp/string/suffix_array.hpp>
#include <boost/test/unit_test.hpp>
#include <iostream>

BOOST_AUTO_TEST_SUITE(string)
BOOST_AUTO_TEST_SUITE(suffix_array)

BOOST_AUTO_TEST_CASE(make_suffix_array) {
  std::string str = "banana";
  const auto actual_sa = djp::make_suffix_array(str);
  std::vector<size_t> expected_sa = {5, 3, 1, 0, 4, 2};
  BOOST_CHECK(actual_sa == expected_sa);
}

BOOST_AUTO_TEST_CASE(make_lcp_array) {
  std::string str = "banana";
  const auto sa = djp::make_suffix_array(str);
  const auto actual_lcp = djp::make_lcp_array(str, sa);
  std::vector<size_t> expected_lcp = {1, 3, 0, 0, 2};
  BOOST_CHECK(actual_lcp == expected_lcp);
}

BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
