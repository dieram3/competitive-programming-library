#include <djp/math/common_factor.hpp>
#include <boost/test/unit_test.hpp>

using djp::gcd;
using djp::lcm;

BOOST_AUTO_TEST_SUITE(math)

BOOST_AUTO_TEST_CASE(common_factor) {

  BOOST_CHECK_EQUAL(gcd(0, 5), 5);
  BOOST_CHECK_EQUAL(gcd(10, 0), 10);
  BOOST_CHECK_EQUAL(gcd(10, 5), 5);
  BOOST_CHECK_EQUAL(gcd(10, 13), 1);
  BOOST_CHECK_EQUAL(gcd(11, 22), 11);
  BOOST_CHECK_EQUAL(gcd(15, 15), 15);

  BOOST_CHECK_EQUAL(lcm(0, 5), 0);
  BOOST_CHECK_EQUAL(lcm(5, 0), 0);
  BOOST_CHECK_EQUAL(lcm(0, 0), 0);
  BOOST_CHECK_EQUAL(lcm(2, 5), 10);
  BOOST_CHECK_EQUAL(lcm(3, 6), 6);
  BOOST_CHECK_EQUAL(lcm(7, 3), 21);
  BOOST_CHECK_EQUAL(lcm(24, 24), 24);
}

BOOST_AUTO_TEST_SUITE_END()
