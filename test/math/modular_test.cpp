//          Copyright Diego Ramírez November 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <djp/math/modular.hpp>
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(math)
BOOST_AUTO_TEST_SUITE(modular)

BOOST_AUTO_TEST_CASE(modular_test) {
  using modular_t = djp::modular<unsigned, 7>;

  const modular_t a = 4;
  const modular_t b = 12;

  BOOST_CHECK(congruent(a, 4));
  BOOST_CHECK(congruent(b, 5));
  BOOST_CHECK(congruent(a + b, 2));
  BOOST_CHECK(congruent(a - b, 6));
  BOOST_CHECK(congruent(b - a, 1));
  BOOST_CHECK(congruent(a * b, 6));
  BOOST_CHECK(congruent(pow(a, 7 - 2), 2));
  BOOST_CHECK(congruent(pow(b, 7 - 2), 3));
  BOOST_CHECK(congruent(inverse(a), 2));
  BOOST_CHECK(congruent(inverse(b), 3));
  BOOST_CHECK(congruent(a / b, 5));
  BOOST_CHECK(congruent(b / a, 3));
  BOOST_CHECK_EQUAL(static_cast<int>(a), 4);
  BOOST_CHECK_EQUAL(static_cast<int>(b), 5);
}

BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
