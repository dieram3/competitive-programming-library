#include <djp/math/efloat.hpp>
#include <boost/test/unit_test.hpp>
#include <sstream>

BOOST_AUTO_TEST_SUITE(math)
BOOST_AUTO_TEST_SUITE(efloat)

BOOST_AUTO_TEST_CASE(comparison) {
  using float_t = djp::efloat<float>;
  float_t x = 3;
  BOOST_CHECK(x == 3.000000032);
  BOOST_CHECK(x != 3.00012312);
  BOOST_CHECK(x < 3.000123);
  BOOST_CHECK(x > 2.9991413);
  BOOST_CHECK(x <= 2.9999999232);
  BOOST_CHECK(x >= 3.00000000123);
}

BOOST_AUTO_TEST_CASE(arithmetic) {
  using float_t = djp::efloat<double>;
  float_t x = 3, y = 4;
  BOOST_CHECK_EQUAL(+x, 3);
  BOOST_CHECK_EQUAL(-x, -3);
  BOOST_CHECK_EQUAL(x + y, 7);
  BOOST_CHECK_EQUAL(x - y, -1);
  BOOST_CHECK_EQUAL(x * y, 12);
  BOOST_CHECK_EQUAL(x / y, 0.75);
}

BOOST_AUTO_TEST_CASE(assign) {
  using float_t = djp::efloat<long double>;
  float_t x = 3;
  BOOST_CHECK_EQUAL(&(x = 4), &x);
  BOOST_CHECK_EQUAL(x, 4);
  BOOST_CHECK_EQUAL(&(x += 4), &x);
  BOOST_CHECK_EQUAL(x, 8);
  BOOST_CHECK_EQUAL(&(x -= 2), &x);
  BOOST_CHECK_EQUAL(x, 6);
  BOOST_CHECK_EQUAL(&(x *= 3), &x);
  BOOST_CHECK_EQUAL(x, 18);
  BOOST_CHECK_EQUAL(&(x /= 2), &x);
  BOOST_CHECK_EQUAL(x, 9);
}

BOOST_AUTO_TEST_CASE(iostream) {
  std::istringstream is("4.3");
  std::ostringstream os;
  djp::efloat<float> x;
  is >> x;
  BOOST_CHECK_EQUAL(x, 4.3);
  os << x;
  BOOST_CHECK_EQUAL(os.str(), "4.3");
}

BOOST_AUTO_TEST_CASE(cast) {
  djp::efloat<double> x = 1;
  BOOST_CHECK_EQUAL(static_cast<double>(x), 1);
  BOOST_CHECK_EQUAL(static_cast<int>(x), 1);
  BOOST_CHECK_EQUAL(static_cast<bool>(x), true);
  BOOST_CHECK_EQUAL(static_cast<float>(x), 1);
}

BOOST_AUTO_TEST_CASE(math_functions) {
  djp::efloat<float> x = 3;
  BOOST_CHECK_EQUAL(abs(-x), 3);
  BOOST_CHECK_EQUAL(sqrt(x), 1.732050808);
  BOOST_CHECK_EQUAL(exp(x), 20.08553692);
  BOOST_CHECK_EQUAL(log(x), 1.098612289);
  BOOST_CHECK_EQUAL(pow(x, 2.5), 15.58845727);
}

BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
