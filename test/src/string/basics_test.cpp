//          Copyright Diego Ramirez October 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <array>
#include <djp/string/basics.hpp>
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(string)
BOOST_AUTO_TEST_SUITE(basics)

BOOST_AUTO_TEST_CASE(split) {
  const std::string str = "hello world, friend of mine!";
  const std::string sep = " ,!";
  const auto tokens = djp::split(str, sep);
  const std::array<std::string, 5> expected =
    {{"hello","world","friend","of","mine"}};

  BOOST_REQUIRE_EQUAL(tokens.size(), expected.size());
  BOOST_CHECK(std::equal(expected.begin(),expected.end(),
                         tokens.begin()));
}

BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
